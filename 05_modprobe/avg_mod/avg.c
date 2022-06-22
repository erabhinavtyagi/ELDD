// AVERAGE Module
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include "header.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ABHINAV");
MODULE_DESCRIPTION("Calling dependent module");

static int a = 10;
static int b = 20;

static int __init avg_init(void)
{
	int average;
	average = my_add(a,b)/2;
	printk("Average of %d and %d is %d.\n", a,b,average);
	return 0;
}

static void __exit avg_exit(void)
{
	printk("\nBYE!\n");
}

module_init(avg_init);
module_exit(avg_exit);
