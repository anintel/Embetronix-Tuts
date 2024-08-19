#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/moduleparam.h>

int valueETX, arr_valueETX[4];
char *nameETX;
int cb_valueETX = 0;

module_param(valueETX, int , S_IRUSR|S_IWUSR);
module_param(nameETX, charp, S_IRUSR|S_IWUSR);
module_param_array(arr_valueETX, int, NULL, S_IRUSR|S_IWUSR);

int notify_param(const char *val, const struct kernel_param *kp){

	int res = param_set_int(val, kp);
	if(res==0) {
		pr_info("Call back function called...\n");
		pr_info("New value of cb_valueETX = %d\n", cb_valueETX);
		return 0;
	}
	return -1;
}

const struct kernel_param_ops my_param_ops = {
	.set = &notify_param,
	.get = &param_get_int,
};

module_param_cb(cb_valueETX, &my_param_ops, &cb_valueETX, S_IRUGO|S_IWUSR); 

static int __init passArg_init(void){
	int i;
	pr_info("ValueETX = %d \n", valueETX);
	pr_info("cb_valueETX = %d \n", cb_valueETX);
	pr_info("NameETX = %s \n", nameETX);
	for(i = 0 ; i<(sizeof arr_valueETX / sizeof (int)) ; i++) {
		pr_info("Arr_value[%d] = %d \n", i, arr_valueETX[i]);
	}
	pr_info("kernle module inserted successfully ... \n");
	return 0;
}

static void __exit passArg_exit(void){
	pr_info("Kernel module removed Successfully \n");
}

module_init(passArg_init);
module_exit(passArg_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ADITH NAREIN");
MODULE_DESCRIPTION("THIS IS THE 3RD TUTORIALS OF EMBETRONIX");
