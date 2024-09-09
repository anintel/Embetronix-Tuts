
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/err.h>

#include <linux/proc_fs.h> // for the proc operations


#define WR_VALUE _IOW('a', 'a', int32_t *)
#define RD_VALUE _IOR('a', 'b', int32_t *)

int32_t value = 0;
char an_array[20] = "try_proc_array\n";
static int len = 1;

dev_t dev = 0;
static struct class *dev_class;
static struct cdev an_cdev;
static struct proc_dir_entry *parent;

static int __init an_driver_init(void);
static void __exit an_driver_exit(void);

static int an_open(struct inode *inode, struct file *file)
{
    pr_info("Device File Opened...!!!\n");
    return 0;
}

static int an_release(struct inode *inode, struct file *file)
{
    pr_info("Device File Closed...!!!\n");
    return 0;
}

static ssize_t an_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    pr_info("Read function\n");
    return 0;
}

static ssize_t an_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
    pr_info("Write Function\n");
    return len;
}

static long an_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch (cmd)
    {
    case WR_VALUE:
        if (copy_from_user(&value, (int32_t *)arg, sizeof(value)))
        {
            pr_err("Data Write : Err!\n");
        }
        pr_info("Value = %d\n", value);
        break;
    case RD_VALUE:
        if (copy_to_user((int32_t *)arg, &value, sizeof(value)))
        {
            pr_err("Data Read : Err!\n");
        }
        break;
    default:
        pr_info("Default\n");
        break;
    }
    return 0;
}

// proc operations
static int open_proc(struct inode *inode, struct file *file)
{
    pr_info("proc file opend.....\t");
    return 0;
}

static int release_proc(struct inode *inode, struct file *file)
{
    pr_info("proc file released.....\n");
    return 0;
}

static ssize_t read_proc(struct file *filp, char __user *buffer, size_t length, loff_t *offset)
{
    pr_info("proc file read.....\n");
    if (len)
    {
        len = 0;
    }
    else
    {
        len = 1;
        return 0;
    }

    if (copy_to_user(buffer, an_array, 20))
    {
        pr_err("Data Send : Err!\n");
    }

    return length;
}

static ssize_t write_proc(struct file *filp, const char *buff, size_t len, loff_t *off)
{
    pr_info("proc file wrote.....\n");

    if (copy_from_user(an_array, buff, len))
    {
        pr_err("Data Write : Err!\n");
    }

    return len;
}

// declaring the file operations for the device
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = an_read,
    .write = an_write,
    .open = an_open,
    .unlocked_ioctl = an_ioctl,
    .release = an_release,
};

// proc operations
static struct proc_ops proc_fops = {
    .proc_open = open_proc,
    .proc_read = read_proc,
    .proc_write = write_proc,
    .proc_release = release_proc,
};

static int __init an_driver_init(void)
{

    if ((alloc_chrdev_region(&dev, 0, 1, "an_Dev")) < 0)
    {
        pr_info("Cannot allocate major number\n");
        return -1;
    }
    pr_info("Major = %d Minor = %d \n", MAJOR(dev), MINOR(dev));

    cdev_init(&an_cdev, &fops);

    // add device to the kernel
    if ((cdev_add(&an_cdev, dev, 1)) < 0)
    {
        pr_info("Cannot add the device to the system\n");
        goto r_class;
    }

    // create class
    if (IS_ERR(dev_class = class_create(THIS_MODULE, "an_class")))
    {
        pr_info("Cannot create the struct class\n");
        goto r_class;
    }

    // create device
    if (IS_ERR(device_create(dev_class, NULL, dev, NULL, "an_device")))
    {
        pr_info("Cannot create the Device 1\n");
        goto r_device;
    }

    // create a directory called an
    parent = proc_mkdir("an", NULL);

    if (parent == NULL)
    {
        pr_info("Error creating proc entry");
        goto r_device;
    }

    // creating the proc entry
    proc_create("an_proc", 0666, parent, &proc_fops);

    pr_info("Device Driver Insert...Done!!!\n");
    return 0;

r_device:
    class_destroy(dev_class);
r_class:
    unregister_chrdev_region(dev, 1);
    return -1;
}

static void __exit an_driver_exit(void)
{
    proc_remove(parent);

    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&an_cdev);
    unregister_chrdev_region(dev, 1);
    pr_info("Device Driver Remove...Done!!!\n");
}

module_init(an_driver_init);
module_exit(an_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Adith Narein");
MODULE_DESCRIPTION("Seeing how PROCFS works!");
MODULE_VERSION("1.6");