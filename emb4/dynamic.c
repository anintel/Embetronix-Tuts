#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>

dev_t dev = 0;


static int __init dynamic_init(void){

	// here we use alloc_chrdev_region
	// to dynamically assign major and minor number
	// it takes 4 inputs
	// 1. dev - the address of the variable to store the major & minor number
	// 2. firstMinor - unsigned it is the requested first minor to use, generally 0
	// 3. count - same as register_chrdev_region, no. of continous device numbers being requested.
	// 4. name - for the device file 
	// it also returns 0 on success and a -ve integer for any error
	if(alloc_chrdev_region(&dev, 1, 1, "Adith's Dynamic device") < 0){

		printk(KERN_INFO "Was unable to allocated the requested major & minor numbers");
		return -1;
	} else {

		printk(KERN_INFO "MAJOR : %d \t MINOR : %d " , MAJOR(dev), MINOR(dev));
		printk(KERN_INFO "The module has been successfully loaded!");
	}

	return 0;
}


static void __exit dynamic_exit(void){

	// the unregister funciton is going to stay the same for static 
	// dynamic assignment of the major & minor numbers
	unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "The module has been succesfully unloaded!");

}

module_init(dynamic_init);
module_exit(dynamic_exit);

MODULE_LICENSE("gpl");
MODULE_DESCRIPTION("This is another simple module to dynamically allocate major and minor numbers to a device file");
MODULE_AUTHOR("ITs me Adith Narein");
MODULE_VERSION("1.1");
