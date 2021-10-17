#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/miscdevice.h>
#include<linux/uaccess.h>
#include<linux/slab.h>
#include<linux/string.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abdun Nihaal");
MODULE_DESCRIPTION("Misc char driver");
MODULE_VERSION("1.0");

// Name of the device
#define DEVICE_NAME "eudyptula"
// Permissions : Allow read and writes from anyone (User, Group, Others)
#define DEVICE_PERMISSIONS (S_IRUGO|S_IWUGO)
// ID returned by device when read
#define ID	"nihaal"

static char *k_id;
static int k_idlen;

ssize_t eudyptula_read (struct file *filp, char __user * buf, size_t n, loff_t * f_pos) {
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

ssize_t eudyptula_write (struct file *filp, const char __user * buf, size_t n, loff_t * off) {
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

static struct file_operations eudyptula_fops = {
	.read	= &eudyptula_read,
	.write	= &eudyptula_write,
};

static struct miscdevice eudyptula_device = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= DEVICE_NAME,
	.fops	= &eudyptula_fops,
	.mode	= DEVICE_PERMISSIONS,
};

static int __init eudyptula_init(void)
{
	printk(KERN_DEBUG "eudyptula: Vanakkam!\n");

	// Allocate and Initialize kernel buffer with ID
	k_id = (char*) kzalloc(sizeof(char)*strlen(ID), GFP_KERNEL);
	if (!k_id)
		return -ENOMEM;
	strcpy (k_id, ID);
	k_idlen = strlen(k_id);

	// Register misc char device
	if(misc_register(&eudyptula_device)) {
		return -ENODEV;
	}
	return 0;
}

static void __exit eudyptula_exit(void)
{
	printk(KERN_DEBUG "eudyptula: Ta Ta\n");
	kfree(k_id);
	// Unregister misc char device
	misc_deregister(&eudyptula_device);
}

module_init(eudyptula_init);
module_exit(eudyptula_exit);
