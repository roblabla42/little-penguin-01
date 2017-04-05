#include <linux/kernel.h>
#include <linux/uaccess.h>

static size_t foo_size;
static char foo_tmp[PAGE_SIZE];
static struct mutex foo_mutex;

static ssize_t foo_op_read(struct file *f, char *buffer, size_t length, loff_t *offset)
{
	int res;
	char *read_from = foo_tmp + *offset;
	size_t read_num;

	res = mutex_lock_interruptible(&foo_mutex);
	if (res)
		return res;

	read_num = length < (foo_size - *offset) ? length : (foo_size - *offset);
	if (read_num == 0) {
		res = 0;
		goto cleanup;
	}
	res = copy_to_user(buffer, read_from, read_num);
	if (res == read_num) {
		res = -EIO;
	} else {
		res = read_num - res;
		*offset += res;
	}
cleanup:
	mutex_unlock(&foo_mutex);
	return (res);
}

static ssize_t foo_op_write(struct file *f, const char *buf, size_t len, loff_t *offset)
{
	int res;

	if (*offset + len >= PAGE_SIZE)
		return (-EINVAL);
	
	res = mutex_lock_interruptible(&foo_mutex);
	if (res)
		return res;

	res = copy_from_user(foo_tmp + *offset, buf, len);
	if (res == len) {
		res = -EINVAL;
		goto cleanup;
	}
	res = len - res;
	*offset += res;
	foo_size = *offset;
cleanup:
	mutex_unlock(&foo_mutex);
	return (res);
}

static struct file_operations foo_fops = {
  .read = foo_op_read,
  .write = foo_op_write,
};
