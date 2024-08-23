#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>


// __^ after adding the device file when we try to open it we find it not to open.
// This is because we haven't assigned any file operations to it.
// Like to read or write in the file so that it does the required actions for the 
// user.


dev_t dev = 0;
static struct class *dev_class;
static struct cdev an_cdev;

// cdev -> character device. 
// used to register a character device
// has file operations that can be used to define custom behaviour
// also keeps track of device numbers


/* So here we need five functions init, exit, open, close(release), read and write */
static int __init andriver_init(void);
static void __exit andriver_exit(void);

// inode - a fundanmental data structure used to represent a filesystem object
// it contains its metadata but not its name or actual data.

static int an_open(struct inode *inode, struct file *file);
static int an_release(struct inode *inode, struct file *file);
static ssize_t an_read(struct file *fptr, char __user *buf, size_t len, loff_t * off);
static ssize_t an_write(struct file *fptr, const char *buf, size_t len, loff_t * off);

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = an_read,
	.write = an_write,
	.open = an_open,
	.release = an_release,
};

// now we define the functions 
static int an_open(struct inode *inode, struct file *file){
	pr_info("The open function is being called.. \n");
	return 0;
}

static int an_release(struct inode *inode, struct file *file){
	pr_info("The relase aka close function is being called. \n");
	return 0;
}

static ssize_t an_read(struct file *fptr, char __user *buf, size_t len, loff_t * off){
	pr_info("The read function is being called \n");
	return 0;
}

static ssize_t an_write(struct file *fptr, const char *buf, size_t len, loff_t * off){
	pr_info("The write function is being called \n");
	return len;
}


// init function
static int __init andriver_init(void){

	if(alloc_chrdev_region(&dev, 0, 1, "an_driver") < 0 ){
		pr_err("The major number allocation failed!");
		return -1;
	}
	pr_info("Major = %d \t Minor =%d", MAJOR(dev), MINOR(dev));

	// creaing cdev
	cdev_init(&an_cdev, &fops);
	
	// addding cdev to the system
	if(cdev_add(&an_cdev, dev, 1) < 0 ){
		pr_err("Cannot add the device to the system");
		goto r_class;
	}

	// creating struct class
	if(IS_ERR(dev_class = class_create("an_class"))){
		pr_err("Cannot create the struct class 1 \n");
		goto r_class;
	}

	// creating devices 
	if(IS_ERR(device_create(dev_class, NULL, dev, NULL, "an_device"))){
		pr_err("The device cannot be created");
		goto r_device;
	}

	pr_info("Device driver insert .. Done!! \n");
	return 0;

	r_device:
		class_destroy(dev_class);
	r_class:
		unregister_chrdev_region(dev, 1);
		return -1;
}


static void __exit andriver_exit(void){
	device_destroy(dev_class, dev);
	class_destroy(dev_class);
	cdev_del(&an_cdev);
	unregister_chrdev_region(dev, 1);
	pr_info("The module is succcessfully exited! \n");
}

module_init(andriver_init);
module_exit(andriver_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("This is to see the working of cdev module and get familiar with it");
MODULE_VERSION("1.1");
MODULE_AUTHOR("Adith Narein");
