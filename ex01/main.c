#include <linux/module.h>
#include <linux/kernel.h>

int init_module(void)
{
	printk(KERN_INFO "Hello world!\n");
	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "Cleaning up module.\n");
}

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("roblabla");
MODULE_DESCRIPTION("An hello world module");
