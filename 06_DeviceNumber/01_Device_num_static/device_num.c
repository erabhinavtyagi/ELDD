// DEVICE NUMBER -MAJOR/MINOR NUMBER

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

//creating the dev with statically allocation of major and minor number.
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ABHINAV TYAGI");

dev_t device = MKDEV(250, 0);   // MAX value of Major number allowed = 511

//Module init function
static int __init dev_init(void)
{
    register_chrdev_region(device, 1, "simplechardriver");
    printk(KERN_INFO "major = %d\n Minor = %d\n", MAJOR(device),MINOR(device));
    printk(KERN_INFO "kernel module inserted successfully");
    return 0;
}
//Module exit function
static void __exit dev_exit(void)
{
    unregister_chrdev_region(device, 1);
    printk(KERN_INFO "kernel module removed successfully");
}

module_init(dev_init);
module_exit(dev_exit);

