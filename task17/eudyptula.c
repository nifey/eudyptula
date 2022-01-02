#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/miscdevice.h>
#include<linux/uaccess.h>
#include<linux/slab.h>
#include<linux/string.h>
#include<linux/sched.h>
#include<linux/wait.h>
#include<linux/kthread.h>

MODULE_LICENSE("GPL");
// ID checked during write
#define ID	"1234567"

// Waitqueue for kernel thread
DECLARE_WAIT_QUEUE_HEAD(wee_wait);
static int flag = 0;

static char *k_id;
static int k_idlen;

ssize_t eudyptula_write (struct file *filp, const char __user * buf, size_t n, loff_t * off) {
	// Copy the data from userspace
	char* kbuf = (char*) kzalloc(sizeof(char)*n, GFP_KERNEL);
	if (!kbuf) {
		return -ENOMEM;
	}
	if ((n != k_idlen) || copy_from_user(kbuf, buf, n)) {
		kfree(kbuf);
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
	.write	= &eudyptula_write,
};

static struct miscdevice eudyptula_device = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= "eudyptula",
	.fops	= &eudyptula_fops,
	.mode	= S_IWUGO,
};

static int kthread_function(void *data) {
	pr_debug("eudyptula: Hello from thread\n");
	wait_event_interruptible(wee_wait, flag != 0);
	pr_debug("eudyptula: Bye from thread\n");
	return 0;
}

static int __init eudyptula_init(void)
{
	pr_debug("eudyptula: Vanakkam!\n");

	// Allocate and Initialize kernel buffer with ID
	k_id = (char*) kzalloc(sizeof(char)*strlen(ID), GFP_KERNEL);
	if (!k_id)
		return -ENOMEM;
	strcpy (k_id, ID);
	k_idlen = strlen(k_id);

	// Register misc char device
	if(misc_register(&eudyptula_device)) {
		kfree(k_id);
		return -ENODEV;
	}

	// Create kernel thread
	if (IS_ERR(kthread_run(kthread_function, NULL, "eudyptula"))) {
		pr_debug("eudyptula: Cannot create kernel thread\n");
		kfree(k_id);
		misc_deregister(&eudyptula_device);
		return -ENOMEM;
	}
	return 0;
}

static void __exit eudyptula_exit(void)
{
	pr_debug("eudyptula: Ta Ta\n");
	kfree(k_id);
	// Kill kthread
	flag = 1;
	wake_up_interruptible(&wee_wait);
	// Unregister misc char device
	misc_deregister(&eudyptula_device);
}

module_init(eudyptula_init);
module_exit(eudyptula_exit);
