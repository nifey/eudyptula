#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/debugfs.h>
#include<linux/slab.h>
#include<linux/rwsem.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abdun Nihaal");
MODULE_DESCRIPTION("Playing with DebugFS");
MODULE_VERSION("1.0");

// Name of the debugfs directory
#define DIR_NAME "eudyptula"

// Filenames
#define ID_FILENAME "id"
#define JIFFIES_FILENAME "jiffies"
#define FOO_FILENAME "foo"

// ID returned by device when read
#define ID	"nihaal"
// id file Permissions : Allow read and writes from anyone (User, Group, Others)
#define ID_PERMISSIONS (S_IRUGO|S_IWUGO)
// jiffies file Permissions : Allow only read from anyone (User, Group, Others)
#define JIFFIES_PERMISSIONS S_IRUGO
// foo file Permissions : Allow write from root and reads from anyone (User, Group, Others)
#define FOO_PERMISSIONS S_IRUGO

static char *k_id, *foo_buf;
static int k_idlen;
static struct dentry *root_dentry;
static DECLARE_RWSEM(foo_rwlock);

ssize_t id_read (struct file *filp, char __user * buf, size_t n, loff_t * f_pos) {
	size_t len, ret;

	if ((*f_pos) > k_idlen) {
		// We have already copied the ID to userspace
		return 0;
	}

	len = (k_idlen > n)? n: (k_idlen + 1);
	ret = copy_to_user(buf, k_id, len);
	len = len - ret;
	*f_pos += len;
	return len;
}

ssize_t id_write (struct file *filp, const char __user * buf, size_t n, loff_t * off) {
	// Copy the data from userspace
	char* kbuf = (char*) kzalloc(sizeof(char)*n, GFP_KERNEL);
	if (!kbuf) {
		return -ENOMEM;
	}
	if ((n != k_idlen) || copy_from_user(kbuf, buf, n)) {
		return -EINVAL;
	}
	
	// Check if it matches the ID
	if (strncmp(kbuf, k_id, k_idlen) == 0) {
		kfree(kbuf);
		return n;
	}

	kfree(kbuf);
	return -EINVAL;
}

static struct file_operations id_fops = {
	.read	= &id_read,
	.write	= &id_write,
};

ssize_t foo_read (struct file *filp, char __user * buf, size_t n, loff_t * f_pos) {
	size_t len, ret;

	if ((*f_pos) >= PAGE_SIZE) {
		return 0;
	}

	down_read(&foo_rwlock);
	len = (n > PAGE_SIZE)? PAGE_SIZE: n;
	ret = copy_to_user(buf, foo_buf, len);
	len = len - ret;
	*f_pos += len;
	up_read(&foo_rwlock);
	return len;
}

ssize_t foo_write (struct file *filp, const char __user * buf, size_t n, loff_t * f_pos) {
	size_t len, ret;
	if ((*f_pos) >= PAGE_SIZE) {
		return -ENOMEM;
	}

	down_write(&foo_rwlock);
	memset(foo_buf, 0, PAGE_SIZE);
	len = (n > PAGE_SIZE)? PAGE_SIZE: n;
	ret = copy_from_user(foo_buf, buf, n);
	len = len - ret;
	*f_pos += len;
	up_write(&foo_rwlock);
	return len;
}

static struct file_operations foo_fops = {
	.read	= &foo_read,
	.write	= &foo_write,
};


static int __init eudyptula_init(void)
{
	printk(KERN_DEBUG "eudyptula: Vanakkam!\n");

	// Create "eudyptula" directory
	root_dentry = debugfs_create_dir(DIR_NAME, NULL);
	if (IS_ERR(root_dentry))
		return -ENODEV;

	// Create files under root directory
	// 1. id file
	k_id = (char*) kzalloc(sizeof(char)*strlen(ID), GFP_KERNEL);
	if (k_id) {
		strcpy (k_id, ID);
		k_idlen = strlen(k_id);

		if (IS_ERR(debugfs_create_file(ID_FILENAME, ID_PERMISSIONS, root_dentry, NULL, &id_fops))) {
			printk(KERN_DEBUG "eudyptula: Cannot create id file\n");
		}
	} else {
		printk(KERN_DEBUG "eudyptula: Cannot allocate memory for id file\n");
	}

	// 2. jiffies file
	debugfs_create_u64(JIFFIES_FILENAME, JIFFIES_PERMISSIONS, root_dentry, (u64*)&jiffies);

	// 3. foo file
	foo_buf = (char*) kzalloc(PAGE_SIZE, GFP_KERNEL);
	if (foo_buf) {
		if (IS_ERR(debugfs_create_file(FOO_FILENAME, FOO_PERMISSIONS, root_dentry, NULL, &foo_fops))) {
			printk(KERN_DEBUG "eudyptula: Cannot create foo file\n");
		}
	} else {
		printk(KERN_DEBUG "eudyptula: Cannot allocate memory for foo file\n");
	}

	return 0;
}

static void __exit eudyptula_exit(void)
{
	printk(KERN_DEBUG "eudyptula: Ta Ta\n");
	if (k_id)
		kfree(k_id);
	if (foo_buf)
		kfree(foo_buf);

	// Recursively remove debugfs entries
	debugfs_remove_recursive(root_dentry);
}

module_init(eudyptula_init);
module_exit(eudyptula_exit);
