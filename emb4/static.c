#include <linux/kernel.h> // for basic kernel functions like printk()
#include <linux/init.h> // for the  __init() and __exit() 
#include <linux/module.h> // for making a modeule eg. MODULE_LICENSE
#include <linux/fs.h> // for file system operations
		

// dev_t is used to declare a variable that stores the major and minor number
dev_t dev = MKDEV(235, 0);

static int __init static_init(void){
	
	// to register a character device file wiht major and minor number 
	// use register_chrdev_region
	// takes for inputs 
	// 1 - dev_t variable that has the major and minor number
	// 2 - int count total no. of continous device numbers you are requesting
	// 3 - char* name, name of the device to which this is to be associated with
	
	register_chrdev_region(dev, 1, "Adith's Device");
	printk("Major - %d \t, Minor - %d\n ", MAJOR(dev), MINOR(dev));
	printk("The module has been successfully loaded");
	
	return 0;
}


static void __exit static_exit(void){
	unregister_chrdev_region(dev, 1);
	printk("The module is successfully unloaded");
	return ;
} 

module_init(static_init);
module_exit(static_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Adith Narein");
MODULE_DESCRIPTION("This is a kernel module that helps to load a custom device file with major & minor number");
MODULE_VERSION("1.0");
