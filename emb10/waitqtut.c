#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/err.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/kthread.h>
#include <linux/wait.h> // this is required for the wait queues
#include <linux/err.h>

uint32_t read_count = 0;
static struct task_struct *wait_thread;

DECLARE_WAIT_QUEUE_HEAD(wait_queue_an);

dev_t dev = 0;
static struct class *dev_class;
static struct cdev an_cdev;
int wait_queue_flag = 0;

static int an_open(struct inode *inode, struct file *file)
{
    pr_info("The waitqueue device is opened! \n");
    return 0;
}

static int an_release(struct inode *inode, struct file *file)
{
    pr_info("The waitqueue device is closed! \n");
    return 0;
}

static ssize_t an_read(struct file *fptr, char __user *buf, size_t len, loff_t *off)
{
    pr_info("The read function of waitqueue is opened!");

    wait_queue_flag = 1;
    wake_up_interruptible(&wait_queue_an);
    return len;
}

static ssize_t an_write(struct file *fptr, const char __user *buf, size_t len, loff_t *off)
{
    pr_info("The write function of waitqueue is closed!");
    return len;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = an_open,
    .release = an_release,
    .read = an_read,
    .write = an_write,
};

static int
wait_func(void *unused)
{
    while (1)
    {
        pr_info("Waiting for an Event>>...\n");

        wait_event_interruptible(wait_queue_an, wait_queue_flag != 0);
        if (wait_queue_flag == 2)
        {
            pr_info("Event came from exit Function \n");
            return 0;
        }

        pr_info("Event came from Read function - %d\n", ++read_count);
        wait_queue_flag = 0;
    }

    do_exit(0);
    return 0;
}

static int __init an_driver_init(void)
{

    if (alloc_chrdev_region(&dev, 0, 1, "an_dev_waitq") < 0)
    {
        pr_info("Cannot allocate major number \n");
        return -1;
    }
    pr_info("Major = %d, Minor = %d \n", MAJOR(dev), MINOR(dev));

    cdev_init(&an_cdev, &fops);
    an_cdev.owner = THIS_MODULE;
    an_cdev.ops = &fops;

    if (cdev_add(&an_cdev, dev, 1) < 0)
    {
        pr_info("Cannot add the device to the system \n");
        goto r_class;
    }

    if (IS_ERR(dev_class = class_create("an_class_waitq")))
    {
        pr_info("Cannot create the struct class \n");
        goto r_class;
    }

    if (IS_ERR(device_create(dev_class, NULL, dev, NULL, "an_device_waitq")))
    {
        pr_info("cannot create the Device \n");
        goto r_device;
    }

    wait_thread = kthread_create(wait_func, NULL, "WaitThread");
    if (wait_thread)
    {
        pr_info("Thread Created successfully \n");
        wake_up_process(wait_thread);
    }
    else
    {
        pr_info("Thread creation failed \n");
    }

    pr_info("Device driver (waitq) inserted :) \n");
    return 0;

r_device:
    class_destroy(dev_class);
r_class:
    unregister_chrdev_region(dev, 1);
    return -1;
}

static void __exit an_driver_exit(void)
{
    wait_queue_flag = 2;
    wake_up_interruptible(&wait_queue_an);

    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&an_cdev);

    unregister_chrdev_region(dev, 1);
    pr_info("Device Driver (waitq) Removed :)\n");
}

module_init(an_driver_init);
module_exit(an_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Adith Narein");
MODULE_DESCRIPTION("Simple module to implement and understand the working of workqueue.");
MODULE_VERSION("1.10");
