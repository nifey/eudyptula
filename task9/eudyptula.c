#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/sysfs.h>
#include<linux/slab.h>
#include<linux/rwsem.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abdun Nihaal");
MODULE_DESCRIPTION("Playing with SysFS");
MODULE_VERSION("1.0");

// ID returned by device when read
#define ID	"1234567"

static char *k_id, *foo_buf;
static int k_idlen;
static DECLARE_RWSEM(foo_rwlock);

static ssize_t id_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf)
{
	strncpy(buf, k_id, k_idlen);
	return k_idlen;
}

static ssize_t id_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count)
{
	// Check if the count matches ID string length
	if (count != k_idlen) {
		return -EINVAL;
	}

	// Check if it matches the ID
	if (strncmp(buf, k_id, k_idlen) == 0) {
		return count;
	}

	return -EINVAL;
}

// id file Permissions : Allow read and writes from anyone : 0666
// Since sysfs attributes cannot be world-writable we use 0664 instead
static struct kobj_attribute id_attribute =
	__ATTR(id, 0664, id_show, id_store);

static ssize_t jiffies_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf)
{
	return sprintf(buf, "%ld\n", jiffies);
}

// jiffies file Permissions : Allow only read from anyone : 0444
static struct kobj_attribute jiffies_attribute =
	__ATTR(jiffies, 0444, jiffies_show, NULL);

static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf)
{
	down_read(&foo_rwlock);
	strncpy(buf, foo_buf, PAGE_SIZE);
	up_read(&foo_rwlock);
	return PAGE_SIZE;
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count)
{
	int len;
	down_write(&foo_rwlock);
	memset(foo_buf, 0, PAGE_SIZE);
	len = (count > PAGE_SIZE)? PAGE_SIZE: count;
	strncpy(foo_buf, buf, len);
	up_write(&foo_rwlock);
	return len;
}

// foo file Permissions : Allow write from root and reads from anyone : 0644
static struct kobj_attribute foo_attribute =
	__ATTR(foo, 0644, foo_show, foo_store);

// Create attribute group
static struct attribute *attrs[] = {
	&id_attribute.attr,
	&jiffies_attribute.attr,
	&foo_attribute.attr,
	NULL,
};
static struct attribute_group attr_group = {
	.attrs = attrs,
};

static struct kobject *eudyptula_kobj;

static int __init eudyptula_init(void)
{
	int retval;
	printk(KERN_DEBUG "eudyptula: Vanakkam!\n");

	// Create "eudyptula" kobject
	eudyptula_kobj = kobject_create_and_add("eudyptula", kernel_kobj);
	if (!eudyptula_kobj)
		return -ENOMEM;

	// Allocate space for k_id and store ID
	k_id = (char*) kzalloc(sizeof(char)*strlen(ID), GFP_KERNEL);
	if (k_id) {
		strcpy (k_id, ID);
		k_idlen = strlen(k_id);
	} else {
		printk(KERN_ERR "eudyptula: Cannot allocate memory for id file\n");
		return -ENOMEM;
	}

	// Allocate space for foo_buf
	foo_buf = (char*) kzalloc(PAGE_SIZE, GFP_KERNEL);
	if (!foo_buf) {
		printk(KERN_ERR "eudyptula: Cannot allocate memory for foo file\n");
		return -ENOMEM;
	}

	retval = sysfs_create_group(eudyptula_kobj, &attr_group);
	if (retval)
		kobject_put(eudyptula_kobj);

	return 0;
}

static void __exit eudyptula_exit(void)
{
	printk(KERN_DEBUG "eudyptula: Ta Ta\n");
	if (k_id)
		kfree(k_id);
	if (foo_buf)
		kfree(foo_buf);

	// Remove kobject
	kobject_put(eudyptula_kobj);
}

module_init(eudyptula_init);
module_exit(eudyptula_exit);
