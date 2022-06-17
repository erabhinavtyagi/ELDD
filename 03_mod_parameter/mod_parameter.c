#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ABHINAV TYAGI");
MODULE_DESCRIPTION("Changing Arguments through Command line");

static char* charvar = "module";
static int intvar = 56;

module_param(charvar, charp, S_IRUGO);
module_param(intvar, int, S_IRUGO);

static int __init param_init(void)
{
	printk("Hi! we are in Init function\n");
	printk("Value of Charvar is %s\n", charvar);
	printk("Value of Intvar is %d\n", intvar);
	return 0;
}

static void __exit param_exit(void)
{
	printk(KERN_ALERT "GOODBYE\n");
}
module_init(param_init);
module_exit(param_exit);

