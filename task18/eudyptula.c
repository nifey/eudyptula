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
#include<linux/delay.h>
#include<linux/completion.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abdun Nihaal");

struct identity {
	struct list_head node;
	char  name[20];
	int   id;
	bool  busy;
};

LIST_HEAD(identity_list);
DECLARE_WAIT_QUEUE_HEAD(wee_wait);
DECLARE_COMPLETION(all_items_processed);
static int terminate = 0;
static int id = 0;
char *kbuf;

int identity_create(char *name)
{
	struct identity *new;

	new = kmalloc(sizeof(struct identity), GFP_KERNEL);
	if (!new)
		return -ENOMEM;
	strcpy(new->name, name);
	new->id = id++;
	new->busy = false;
	list_add(&new->node, &identity_list);
	return 0;
}

struct identity *identity_get(void)
{
	struct identity *iter;

	if (list_empty(&identity_list))
		return NULL;
	iter = list_last_entry(&identity_list, struct identity, node);
	list_del(&iter->node);
	return iter;
}

static int process_item(void) {
	struct identity *iter = identity_get();
	if (!iter)
		return -1;
	msleep_interruptible(5000);
	pr_debug("eudyptula: %d %s\n", iter->id, iter->name);
	kfree(iter);
	return 0;
}

static int kthread_function(void *data) {
	pr_debug("eudyptula: Hello from thread\n");

	while (1) {
		wait_event_interruptible(wee_wait, !list_empty(&identity_list));
		if (terminate == 1) {
			while (process_item() == 0);
			break;
		} else {
			process_item();
		}
	}

	complete(&all_items_processed);
	pr_debug("eudyptula: Bye from thread\n");
	return 0;
}

ssize_t eudyptula_write (struct file *filp, const char __user * buf, size_t n, loff_t * off) {
	// Copy string from userspace
	if(copy_from_user(kbuf, buf, (n > 19)? 19: n))
		return -EINVAL;
	kbuf[19] = '\0';

	// Create identity and append to list
	identity_create(kbuf);

	// Wake up kthread
	wake_up_interruptible(&wee_wait);

	return n;
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

static int __init eudyptula_init(void)
{
	pr_debug("eudyptula: Vanakkam!\n");

	// Allocate buffer
	kbuf = (char*) kzalloc(sizeof(char)*20, GFP_KERNEL);
	if (!kbuf)
		return -ENOMEM;

	// Register misc char device
	if(misc_register(&eudyptula_device)) {
		kfree(kbuf);
		return -ENODEV;
	}

	// Create kernel thread
	if (IS_ERR(kthread_run(kthread_function, NULL, "eudyptula"))) {
		pr_debug("eudyptula: Cannot create kernel thread\n");
		kfree(kbuf);
		misc_deregister(&eudyptula_device);
		return -ENOMEM;
	}
	return 0;
}

static void __exit eudyptula_exit(void)
{
	pr_debug("eudyptula: Ta Ta\n");
	kfree(kbuf);
	// Unregister misc char device
	misc_deregister(&eudyptula_device);
	// Wake up and Wait for kthread to complete
	terminate = 1;
	wake_up_interruptible(&wee_wait);
	while(wait_for_completion_interruptible(&all_items_processed) != 0) {
		msleep_interruptible(5000);
	}
}

module_init(eudyptula_init);
module_exit(eudyptula_exit);
