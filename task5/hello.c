#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/usb/input.h>
#include<linux/hid.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abdun Nihaal");
MODULE_DESCRIPTION("Hello world USB driver");
MODULE_VERSION("1.0");

static int hello_probe (struct usb_interface *iface,
			 const struct usb_device_id *id) {
	pr_info("hello USB driver: Vanakkam!\n");
	return 0;
}

static void hello_disconnect (struct usb_interface *intf) {
	pr_info("hello USB driver: Ta Ta\n");
	return;
}

static const struct usb_device_id hello_id_table[] = {
	{ USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID, USB_INTERFACE_SUBCLASS_BOOT,
		USB_INTERFACE_PROTOCOL_KEYBOARD) },
	{ }						/* Terminating entry */
};

static struct usb_driver hello_driver = {
	.name =		"my_usb_driver",
	.probe =	hello_probe,
	.disconnect =	hello_disconnect,
	.id_table =	hello_id_table,
};

module_usb_driver(hello_driver);
