#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/hid.h>

static int hello_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
	printk(KERN_INFO "Hello, little probe\n");
	return 0;
}

static void hello_disconnect(struct usb_interface *intf)
{
	printk(KERN_INFO "USB device disconnected\n");
}

static const struct usb_device_id hello_table[] = {
	{ USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID, USB_INTERFACE_SUBCLASS_BOOT, USB_INTERFACE_PROTOCOL_KEYBOARD) },
	{ },
};

MODULE_DEVICE_TABLE(usb, hello_table);

static struct usb_driver hello_driver = {
	.name = "hello_world_driver",
	.id_table = hello_table,
	.probe = hello_probe,
	.disconnect = hello_disconnect,
};

int init_module(void)
{
	printk(KERN_INFO "Hello world!\n");
	return usb_register(&hello_driver);
}

void cleanup_module(void)
{
	printk(KERN_INFO "Cleaning up module.\n");
	usb_deregister(&hello_driver);
}

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("roblabla");
MODULE_DESCRIPTION("An hello world module");
