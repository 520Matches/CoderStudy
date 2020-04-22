#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/interrupt.h>

#define TASKLET_MAJOR	258
#define TASKLET_MINOR	0
#define TASKLET_DEV_CNT	1
#define TASKLET_DEV_NAME	"tasklet"

struct cdev tasklet_cdev;

static void fun_tasklet(unsigned long arg)
{
	static int num = 0;
	printk("num=%d,fun tasklet arg=%lld\n",num,arg);
	num++;
}

DECLARE_TASKLET(Tasklet,fun_tasklet,(unsigned long)8888);

static int tasklet_open(struct inode *inode,struct file *filp)
{
	return 0;
}

static int tasklet_release(struct inode *inode,struct file *filp)
{
	return 0;
}

static irqreturn_t tasklet_handler(int irq,void *dev_id)
{
	tasklet_schedule(&Tasklet);

	return IRQ_HANDLED;
}

static struct file_operations tasklet_ops = {
	.owner = THIS_MODULE,
	.open = tasklet_open,
	.release = tasklet_release,
};

static int __init my_tasklet_init(void)
{
	int ret;
	dev_t dev;

	printk("tasklet init...\n");
	dev = MKDEV(TASKLET_MAJOR,TASKLET_MINOR);
	ret = register_chrdev_region(dev,TASKLET_DEV_CNT,TASKLET_DEV_NAME);
	if(ret)
	{
		goto reg_err;
	}

	cdev_init(&tasklet_cdev,&tasklet_ops);
	ret = cdev_add(&tasklet_cdev,dev,TASKLET_DEV_CNT);
	if(ret)
	{
		goto add_err;
	}

	ret = request_irq(19,tasklet_handler,IRQF_TRIGGER_HIGH | IRQF_SHARED,TASKLET_DEV_NAME,&tasklet_cdev);
	
	return 0;

add_err:

reg_err:
	unregister_chrdev_region(&tasklet_cdev,TASKLET_DEV_CNT);

	return ret;
}

static void __exit my_tasklet_exit(void)
{
	dev_t dev;

	printk("tasklet exit...");

	dev = MKDEV(TASKLET_MAJOR,TASKLET_MINOR);
	free_irq(19,&tasklet_cdev);
	cdev_del(&tasklet_cdev);
	unregister_chrdev_region(dev,TASKLET_DEV_CNT);
}

module_init(my_tasklet_init);
module_exit(my_tasklet_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SHD <HengDi_Sun@163.com>");
