#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>

static int __init hello_world_init(void){
	pr_info("This is my first kernel module");
	pr_info("This is a super simple module");
       	pr_info("kernel module inserted successfully");

	return 0;
}

static void __exit hello_world_exit(void){
	pr_info("The kernel module is successfully exited");
}

/* Macros to register the init & clean up functions  */
module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ADITH NAREIN <adith.narein.t@intel.com> ");
MODULE_DESCRIPTION("A simple driver by me");
MODULE_VERSION("2:1.0");
