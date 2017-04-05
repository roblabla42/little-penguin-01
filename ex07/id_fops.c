#include <linux/kernel.h>
#include <linux/uaccess.h>

#define BUF "rlambert"
#define BUF_LEN 8

static ssize_t id_op_read(struct file *f, char *buffer, size_t length, loff_t *offset)
{
	int res;
	char *read_from = BUF + *offset;
	size_t read_num = length < (BUF_LEN - *offset) ? length : (BUF_LEN - *offset);

	if (read_num == 0) {
		res = 0;
		goto end;
	}

	res = copy_to_user(buffer, read_from, read_num);
	if (res == read_num) {
		res = -EIO;
	} else {
		*offset = BUF_LEN - res;
		res = read_num - res;
	}
end:
	return (res);
}

static ssize_t id_op_write(struct file *f, const char *buf, size_t len, loff_t *offset)
{
	char mybuf[BUF_LEN];
	ssize_t res;

	if (len != BUF_LEN) {
		res = -EINVAL;
		goto end;
	}
	copy_from_user(mybuf, buf, BUF_LEN);
	if (strncmp(mybuf, BUF, BUF_LEN) == 0)
		res = BUF_LEN;
	else
		res = -EINVAL;

end:
	return (res);
}

static struct file_operations id_fops = {
  .read = id_op_read,
  .write = id_op_write,
};
