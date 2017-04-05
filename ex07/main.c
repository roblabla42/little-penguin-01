#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/jiffies.h>
#include "id_fops.c"
#include "foo_fops.c"

static struct dentry *debugfs_root;

#if BITS_PER_LONG == 32
#define debugfs_create_long(name, mode, parent, value) debugfs_create_u32(name, mode, parent, (u32*)(value))
#else
#define debugfs_create_long(name, mode, parent, value) debugfs_create_u64(name, mode, parent, (u64*)(value))
#endif

int init_module(void)
{
	int res = 0;

	printk(KERN_INFO "Hello world!\n");
	debugfs_root = debugfs_create_dir("fourty_two", NULL);
	if (!debugfs_root) {
		res = -ENOENT;
		goto end;
	}
	mutex_init(&foo_mutex);
	if (!debugfs_create_file("id", 0666, debugfs_root, NULL, &id_fops)
		|| !debugfs_create_long("jiffies", 0444, debugfs_root, &jiffies)
		|| !debugfs_create_file("foo", 0644, debugfs_root, NULL, &foo_fops)
		) {
		res = -ENOENT;
		goto end;
	}
end:
	return res;
}

void cleanup_module(void)
{
	printk(KERN_INFO "Cleaning up module.\n");
	debugfs_remove_recursive(debugfs_root);
}

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("roblabla");
MODULE_DESCRIPTION("An hello world module");
