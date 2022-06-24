#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

// Dynamic allocation of device number (Major/Minor Number).

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ABHINAV TYAGI");
MODULE_DESCRIPTION("Dynamic allocation of Major and Minor number");

dev_t device =0;

//MODULE INITIALIZATION

static int __init dyn_init(void)
{
	//Allocating Major Number Dynamically
	if ((alloc_chrdev_region(&device, 0, 1, "FileMK"))<0)
	{
		printk(KERN_INFO "Cannot allocate Major Number for Device 1.\n");
		return (-1);
	}
	printk(KERN_INFO "MAJOR : %d  MINOR :  %d \n", MAJOR(device), MINOR(device));
	printk(KERN_INFO "Kernel module inserted Successfully.\n");
	return 0;
}

// MODULE EXIT

static void __exit dyn_exit(void)
{
	unregister_chrdev_region(device,1);
	printk(KERN_INFO "Kernel Module Removed Successfully.\n");
}

module_init(dyn_init);
module_exit(dyn_exit);


