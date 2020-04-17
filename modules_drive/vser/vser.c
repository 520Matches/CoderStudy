#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/kfifo.h>

#include <linux/ioctl.h>
#include <linux/uaccess.h>
#include <uapi/linux/uio.h>

#include "vser.h"

#define   VSER_MAJOR	256
#define   VSER_MINOR	0
#define   VSER_DEV_CNT	1
#define   VSER_DEV_NAME	"vser"


struct vser_dev
{
	unsigned int baud;
	struct option opt;
	struct cdev cdev;
};

static struct vser_dev vsdev;
DEFINE_KFIFO(vsfifo,char,32);

static int vser_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static int vser_release(struct inode *inode,struct file *filp)
{
	return 0;
}

static ssize_t vser_read(struct file *filp, char __user *buf,size_t count, loff_t *pos)
{
	unsigned int copied = 0;
	kfifo_to_user(&vsfifo, buf, count, &copied);
	return copied;
}

static ssize_t vser_write(struct file *filp, const char __user *buf, size_t count, loff_t *pos)
{
	unsigned int copied = 0;
	kfifo_from_user(&vsfifo,buf,count,&copied);
	return copied;
}
//
// static ssize_t vser_aio_read(struct kiocb *iocb, const struct iovec *iov,unsigned long nr_segs, loff_t pos)
// {
	// size_t read = 0 ;
	// unsigned long i ;
	// ssize_t ret     ;
//
	// for(i=0;i<nr_segs;i++)
	// {
		// ret = vser_read(iocb->ki_filp,iov[i].iov_base,iov[i].iov_len,&pos);
		// if(ret < 0)
		// {
			// break;
		// }
		// read += ret;
	// }
//
	// return read ? read : -EFAULT;
// }
//
// static ssize_t vser_aio_write(struct kiocb *iocb,const struct iovec *iov,unsigned long nr_segs,loff_t pos)
// {
	// size_t written = 0 ;
	// unsigned long i    ;
	// ssize_t ret        ;
//
	// for(i = 0;i<nr_segs;i++)
	// {
		// ret = vser_write(iocb->ki_filp,iov[i].iov_base,iov[i].iov_len,&pos);
		// if(ret < 0)
		// {
			// break;
		// }
		// written += ret;
	// }
//
	// return written ? written : -EFAULT;
// }

static long vser_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	if(_IOC_TYPE(cmd) != VS_MAGIC)
	{
		return -ENOTTY;
	}

	switch(cmd)
	{
		case VS_SET_BAUD:
			vsdev.baud = arg;
			break;
		case VS_GET_BAUD:
			arg = vsdev.baud;
			break;
		case VS_SET_FFMT:
			if(copy_from_user(&vsdev.opt, (struct option __user*)arg, sizeof(struct option)))
			{
				return -ENOTTY;
			}
			break;
		case VS_GET_FFMT:
			if(copy_to_user((struct option __user*)arg, &vsdev.opt, sizeof(struct option)))
			{
				return -ENOTTY;
			}
			break;
		default:
			return -ENOTTY;
	}
}

static struct file_operations vser_ops = {
	.owner          = THIS_MODULE    ,
	.open           = vser_open      ,
	.release        = vser_release   ,
	.read           = vser_read      ,
	.write          = vser_write     ,
	.unlocked_ioctl = vser_ioctl     ,
	// .aio_read       = vser_aio_read  ,
	// .aio_write      = vser_aio_write ,
};

static int __init vser_init(void)
{
	int     ret;
	dev_t   dev;

	printk("vser init\n");

	dev = MKDEV(VSER_MAJOR,VSER_MINOR);
	ret = register_chrdev_region(dev,VSER_DEV_CNT,VSER_DEV_NAME);
	if(ret){
		goto reg_err;
	}

	cdev_init(&vsdev.cdev,&vser_ops);

	vsdev.cdev.owner = THIS_MODULE;
	vsdev.opt.datab  = 8;
	vsdev.opt.parity = 0;
	vsdev.opt.stopb  = 1;
	
	ret = cdev_add(&vsdev.cdev,dev,VSER_DEV_CNT);
	if(ret)
		goto add_err;

	return 0;

add_err:
	unregister_chrdev_region(&vsdev,VSER_DEV_CNT);
reg_err:
	return ret;
}

static void __exit vser_exit(void)
{
	dev_t dev;

	printk("vser_exit\n");

	dev = MKDEV(VSER_MAJOR,VSER_MINOR);

	cdev_del(&vsdev.cdev);
	unregister_chrdev_region(dev,VSER_DEV_CNT);
}

module_init(vser_init);
module_exit(vser_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SHD <HengDi_Sun@163.com>");
