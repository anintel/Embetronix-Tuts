#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/fs.h>

dev_t dev = 0;
static struct class *dev_class;

static int __init dynamic_init(void){

	if(alloc_chrdev_region(&dev, 0, 1, "adith_device") < 0 ) {
		pr_err("Cannot allocate major number for the device");
		return -1;
	}
	pr_info("Major = %d \t Minor = %d \n", MAJOR(dev), MINOR(dev));

	// we create the class
	dev_class = class_create("adith_class");
	
	// check if it is created without any error
	if(IS_ERR(dev_class)){
		pr_err("Cannot create your class \n");
		goto r_class;
	}


	// creating the device
	if(IS_ERR(device_create(dev_class, NULL, dev, NULL, "adith_device"))){
		pr_err("Cannot create your device \n");
		goto r_device;
	}

	pr_info("Kernel Module successfully inserted! \n");
	return 0;


	r_device:
		class_destroy(dev_class);
	r_class:
		unregister_chrdev_region(dev, 1);
		return -1;

}

static void __exit dynamic_exit(void){
	
	device_destroy(dev_class, dev);
	class_destroy(dev_class);
	unregister_chrdev_region(dev, 1);
	pr_info("Kernel Module Removed Successfully \n");
}

module_init(dynamic_init);
module_exit(dynamic_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("adithnarein");
MODULE_DESCRIPTION("This is a simple linux kernl module to automatically create and destroy a device file. ");
MODULE_VERSION("1.2 ");

