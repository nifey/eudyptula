#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/usb/input.h>
#include<linux/hid.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abdun Nihaal");
MODULE_DESCRIPTION("Hello world kernel module that loads automatically when USB keyboard is plugged in");
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

/*
 * This creates the table containing the ids corresponding to the USB keyboard device
 * The id table was derived from drivers/hid/usbhid/usbkbd.c
 * MODULE_DEVICE_TABLE macro is used to register the alias for this module.
 */
static const struct usb_device_id hello_id_table[] = {
	{ USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID, USB_INTERFACE_SUBCLASS_BOOT,
		USB_INTERFACE_PROTOCOL_KEYBOARD) },
	{ }						/* Terminating entry */
};
MODULE_DEVICE_TABLE(usb, hello_id_table);

module_init(hello_init);
module_exit(hello_exit);
