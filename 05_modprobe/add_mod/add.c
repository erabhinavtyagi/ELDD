#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ABHINAV");
//MODULE_DESCRIPTION("ADD FUNCTIONALITY MODULE FOR MODPROBE DEMO.","ADD FUNCTIONALITY MODULE FOR MODPROBE DEMO.");

int my_add(int a, int b)
{	
	return (a+b);
}

EXPORT_SYMBOL(my_add);			// "my_add" Symbol is Exported to Kernel Symbol Table

// To initialize this module and load it to Kernel Symbol Table
static int __init hello_init(void)
{
	printk("WELCOME BACK.\n");
	return 0;
}

// To remove this module from Kernel Symbol Table
static void __exit hello_exit(void)
{
	printk("Goodbye, Have a Nice Day!!");
}

module_init(hello_init);
module_exit(hello_exit);
