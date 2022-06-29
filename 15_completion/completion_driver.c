/*********steps for compiling and loading******************
 * 1.make
 * 2.sudo -s
 * 3.dmesg -C
 * 4.dmesg
 * 5.insmod completion_driver.ko
 * 6.dmesg
 * 7.mknod -m 777 /dev/WR_1 c 234 0
 * 8.mknod -m 777 /dev/WR_2 c 234 1
 * 9.mknod -m 777 /dev/APP_READ c 234 2
 * 10../wr_1
 * 11../wr_2 (in first terminal)****./rd(in second terminal)
 * 12.dmesg
 * 13.rmmod completion_driver
 * 14.dmesg
 * 15.exit
 ************************************************************/


#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/completion.h>

// ************ Declarations *******************

dev_t dev;
struct cdev *my_dev;
struct completion my_completion;
unsigned int op_result;
int op_done_flag = 0;

static int my_open(struct inode *inode, struct file *file);
static int my_release(struct inode *inode, struct file *file);
static ssize_t my_write(struct file *file, const char __user *ubuff, size_t count, loff_t *off);
static ssize_t my_read(struct file *file, char __user *ubuff, size_t count, loff_t *off);

static int my_operation(char *ubuff);

struct file_operations fops = 
{
	.owner = THIS_MODULE,
	.open = my_open,
	.write = my_write,
	.read = my_read,
	.release = my_release,
};

// ************* Definitions ***********************

static int my_open(struct inode *inode, struct file *file)
{
printk("OPEN FUNCTIONALITY....\n");
return 0;
}

static int my_release(struct inode *inode, struct file *file)
{
printk("EXIT FUNCTIONALITY....\n");
return 0;
}

static ssize_t my_write(struct file *file, const char __user *ubuff, size_t count, loff_t *off)
{
	printk("In WRITE Functionality\n");
    int ret_val;
    if(op_done_flag != 0)
{
	wait_for_completion(&my_completion);
	ret_val = my_operation(ubuff);
	if(ret_val == 0)
	{
		op_done_flag = 1;
		return count;
	}
	else
	return -1;
}
else
{
	ret_val = my_operation(ubuff);
	if(ret_val == 0)
	{
		op_done_flag = 1;
		return count;
	}
	else
	return -1;

}
}

static ssize_t my_read(struct file *file, char __user *ubuff, size_t count, loff_t *off)
{
	printk("In Read Functionality...\n");
	int result = copy_to_user((char *)ubuff, &op_result, sizeof(op_result));

	if(result == 0)
	{
	printk("Success in Reading Data from Kernel to User.\n");
	printk("In Read Section The Result is : %d \n",op_result);
	complete(&my_completion);
	op_done_flag = 0;

	
	return count;
	}
	else
	{
	pr_err("Error in Reading Data from Kernel to User\n");
	return -1;
	}
}

static int my_operation(char *ubuff)
{
	int kbuff[2],result,num1,num2;
	result = copy_from_user((char *)kbuff, (char *)ubuff, sizeof(kbuff));
	if(result == 0)
	{
		printk("\n success in writing data from user to kernel\n");
		num1 = kbuff[0];
		num2 = kbuff[1];
		op_result = num1 + num2;
		printk("\n Result in write section of the driver: %d\n", op_result);
		return 0;
	}
	else
	{
		printk("\n Error in writing from user to kernel\n");
		return -1;

	}

	}

// *********** MODULE INITIALIZATION ***************************************

static int my_completion_init(void)
{
	printk("Initialization Function \n");
	if (alloc_chrdev_region(&dev , 0, 3, "completionDRIVER")<0)
	{
		pr_err("Major Num not Allocated\n");
		return -1;
	}
	pr_info("MAJOR NUMBER: %d\nMINOR NUMBER: %d\n",MAJOR(dev),MINOR(dev));
	
	my_dev = cdev_alloc();
	my_dev->ops = &fops;
	
	if(cdev_add(my_dev, dev, 3)<0)
	{
		pr_err("Error registering Device with File Operation.\n");
		return -1;
	}

	
	init_completion(&my_completion);
	return 0;

}

//*************** MODULE EXIT *********************************
static void my_completion_exit(void)
{
	printk("\n In Exit Section\n");
	cdev_del(my_dev);
	printk("Success in Deactivate\n");
	unregister_chrdev_region(dev, 3);
	printk("Success in Deregister\n");
}

// *********************************************

module_init(my_completion_init);
module_exit(my_completion_exit);

//**********LICENSE*************************

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ABHINAV");
MODULE_DESCRIPTION("COMPLETION PROGRAM- Kernel Synchronization");