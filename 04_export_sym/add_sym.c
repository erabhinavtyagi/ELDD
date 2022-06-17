// program for AVERAGE Module


#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include"header.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ABHINAV TYAGI");
MODULE_DESCRIPTION("Calling dependent module- my_add()");

static int a = 10;
static int b = 20;

static int __init avg_init(void)
{
	printk("Addition of %d and %d is %d \n.", a,b,my_add(a,b));
	return 0;
}

static void __init avg_exit(void)
{
	printk("\nBYE!\n");
}

module_init(avg_init);
module_exit(avg_exit);
