#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h> // includes functions for kernel memory allocation
#include <linux/uaccess.h> // functions for accessing user space
#include <linux/ioctl.h> // for ioctl system calls
#include <linux/err.h> 

/* _IOW creates unique IOCTL command number
arg 1 -> magic number to differentiate this ioctl command from those of other devices
arg 2 -> command number
arg 3 -> data type */

#define WR_VALUE _IOW('a', 'a', int32_t*)  // write operation (user to kernel space)
#define RD_VALUE _IOR('a', 'b', int32_t*)  // read operation (kernel to user space)

int32_t value = 0;

dev_t dev = 0;
static struct class *dev_class;
static struct cdev an_cdev;

// function prototypes
static int __init an_driver_init(void);
static void __exit an_driver_exit(void);

static int an_open (struct inode *inode, struct file *file){
    pr_info("Device file is opened! \n");
    return 0;
}

static int an_release (struct inode *inode, struct file *file){
    pr_info ("Device file is closed! \n");
    return 0;
}

static ssize_t an_read(struct file *fptr, char __user *buf, size_t len, loff_t *off) {
    pr_info("READ function! \n");
    return 0;
}

static ssize_t an_write(struct file *fptr, const char *buf, size_t len, loff_t *off) {
    pr_info("WRITE function! \n");
    return len;
}

static long an_ioctl(struct file *file, unsigned int cmd, unsigned long arg){

    switch (cmd)
    {
    case WR_VALUE:
        if( copy_from_user(&value, (int32_t *)arg, sizeof(value)))  {
            pr_err("Data write : Err! \n");
        }
        pr_info("Value = %d\n", value);
        break;
    
    case RD_VALUE:
        if(copy_to_user((int32_t *) arg, &value, sizeof(value)))
        {
            pr_err("Data Read: ERR! \n");
        }
        break;

    default:
        pr_info("Default no read/write :( \n");
        break;
    }

    return 0;
}

// file operations structure
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = an_read,
    .write = an_write,
    .open = an_open,
    .release = an_release,
    .unlocked_ioctl = an_ioctl,
};

static int __init an_driver_init(void) {

    if((alloc_chrdev_region(&dev, 0, 1, "an_dev")) < 0) {
        pr_err("Cannot allocate major Number \n");
        return -1;
    }

    pr_info("Major No = %d \t Minor No. = %d", MAJOR(dev), MINOR(dev));

    // creating cdev structure
    cdev_init(&an_cdev, &fops);

    // adding character device to the system
    if(cdev_add(&an_cdev, dev, 1)  < 0 ) {
        pr_err("Cannot add the device to the system \n");
        goto r_class;
    }

    // creating the struct class
    if(IS_ERR(dev_class = class_create( "an_class"))){
        pr_err("Cannot create the struct class \n");
        goto r_class;
    }

    // creating device
    if(IS_ERR(device_create(dev_class, NULL, dev, NULL, "an_device"))){
        pr_err("Cannot create the Device \n");
        goto r_device;
    }

    pr_info("Device driver Inserted  :) \n");
    return 0;

    r_device:
        class_destroy(dev_class);
    r_class:
        unregister_chrdev_region(dev, 1);
        return 1;
} 

static void __exit an_driver_exit(void){
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&an_cdev);
    unregister_chrdev_region(dev, 1);
    pr_info("Device driver Removed.!! \n");
}

module_init(an_driver_init);
module_exit(an_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("adith narein");
MODULE_DESCRIPTION("IOCTL DEVICE DRIVER");
MODULE_VERSION("1.5");