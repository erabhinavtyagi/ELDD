#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ABHINAV");
MODULE_DESCRIPTION("ELDD First Program.");
MODULE_ALIAS("HELLO WORLD");
MODULE_INFO(board,"RASPBERRY PI 4");

// Initialize this Module and load it into Kernel
static int hello_init(void)
{
	printk("Welcome to E-World\n");
	return 0;
}

// Remove this Module from Kernel
static void end_exit(void)
{
	printk("Goodbye\n");
}

module_init(hello_init);
module_exit(end_exit);
