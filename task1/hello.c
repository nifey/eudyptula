#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abdun Nihaal");
MODULE_DESCRIPTION("Hello world kernel module");
MODULE_VERSION("1.0");

static int __init hello_init(void)
{
	printk(KERN_DEBUG "Vanakkam!\n");
	return 0;
}

static void __exit hello_exit(void)
{
	printk(KERN_DEBUG "Ta Ta\n");
}

module_init(hello_init);
module_exit(hello_exit);
