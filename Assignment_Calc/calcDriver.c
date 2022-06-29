/* CALCULATOR DRIVER */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/types.h>
//#include <linux/semaphore.h>

dev_t dev_aj;
struct cdev *my_dev;
//struct semaphore my_sem;
unsigned int op_result;

static int add_open(struct inode *inode, struct file *file);
static int sub_open(struct inode *inode, struct file *file);
static int mul_open(struct inode *inode, struct file *file);
static int div_open(struct inode *inode, struct file *file);

static int add_release(struct inode *inode, struct file *file);
static int sub_release(struct inode *inode, struct file *file);
static int mul_release(struct inode *inode, struct file *file);
static int div_release(struct inode *inode, struct file *file);

static ssize_t add_write(struct file *file, const char __user *ubuff, size_t count, loff_t *off);
static ssize_t sub_write(struct file *file, const char __user *ubuff, size_t count, loff_t *off);
static ssize_t mul_write(struct file *file, const char __user *ubuff, size_t count, loff_t *off);
static ssize_t div_write(struct file *file, const char __user *ubuff, size_t count, loff_t *off);

static ssize_t add_read(struct file *file, char __user *ubuff, size_t count, loff_t *off);
static ssize_t sub_read(struct file *file, char __user *ubuff, size_t count, loff_t *off);
static ssize_t mul_read(struct file *file, char __user *ubuff, size_t count, loff_t *off);
static ssize_t div_read(struct file *file, char __user *ubuff, size_t count, loff_t *off);

struct file_operations fops =
{
	.owner = THIS_MODULE,
	.open = add_open,
	.open = sub_open,
	.open = mul_open,
	.open = div_open,

	.write = add_write, 
	.write = sub_write, 
	.write = mul_write, 
	.write = div_write,
	
	.read = add_read,
	.read = sub_read,
	.read = mul_read,
	.read = div_read,

	//.read = add_read, sub_read, mul_read, div_read,
	.release = add_release,
	.release = sub_release,
	.release = mul_release,
	.release = div_release,
};

// ************* File Structure Definitions ***********************

static int add_open(struct inode *inode, struct file *file)
{
printk("OPEN ADD FUNCTIONALITY....\n");
return 0;
}

static int add_release(struct inode *inode, struct file *file)
{
printk("EXIT ADD FUNCTIONALITY....\n");
return 0;
}

static int sub_open(struct inode *inode, struct file *file)
{
printk("OPEN SUB FUNCTIONALITY....\n");
return 0;
}

static int sub_release(struct inode *inode, struct file *file)
{
printk("EXIT SUB FUNCTIONALITY....\n");
return 0;
}
static int mul_open(struct inode *inode, struct file *file)
{
printk("OPEN MUL FUNCTIONALITY....\n");
return 0;
}

static int mul_release(struct inode *inode, struct file *file)
{
printk("EXIT MUL FUNCTIONALITY....\n");
return 0;
}

static int div_open(struct inode *inode, struct file *file)
{
printk("OPEN DIV FUNCTIONALITY....\n");
return 0;
}

static int div_release(struct inode *inode, struct file *file)
{
printk("EXIT DIV FUNCTIONALITY....\n");
return 0;
}
//*************** Write Section (COPY FROM USER)*****************

static ssize_t add_write(struct file *file, const char __user *ubuff, size_t count, loff_t *off)
{
	//down(&my_sem);
	printk("In ADD -WRITE Functionality\n");

	int kbuff[2];
	int result, num1, num2;

	result = copy_from_user((char *)kbuff, (char *)ubuff, sizeof(kbuff));

	if (result == 0)
	{
		printk("Success in Writing Data from User to Kernel.\n");
		num1 = kbuff[0];
		num2 = kbuff[1];

		op_result = num1 + num2;
		printk("Result in ADD- Write Section of Driver: %d\n", op_result);
		return count;
	}
	else
	{
		pr_err("Error in ADD- Writing Data from User to Kernel.\n");
		return -1;
	}
}

static ssize_t sub_write(struct file *file, const char __user *ubuff, size_t count, loff_t *off)
{
	//down(&my_sem);
	printk("In SUB- WRITE Functionality\n");

	int kbuff[2];
	int result, num1, num2;

	result = copy_from_user((char *)kbuff, (char *)ubuff, sizeof(kbuff));

	if (result == 0)
	{
		printk("Success in SUB- Writing Data from User to Kernel.\n");
		num1 = kbuff[0];
		num2 = kbuff[1];

		op_result = num1 - num2;
		printk("Result in SUB- Write Section of Driver: %d\n", op_result);
		return count;
	}
	else
	{
		pr_err("Error in SUB- Writing Data from User to Kernel.\n");
		return -1;
	}
}

static ssize_t mul_write(struct file *file, const char __user *ubuff, size_t count, loff_t *off)
{
	//down(&my_sem);
	printk("In MUL- WRITE Functionality\n");

	int kbuff[2];
	int result, num1, num2;

	result = copy_from_user((char *)kbuff, (char *)ubuff, sizeof(kbuff));

	if (result == 0)
	{
		printk("Success in MUL- Writing Data from User to Kernel.\n");
		num1 = kbuff[0];
		num2 = kbuff[1];

		op_result = num1 * num2;
		printk("Result in MUL- Write Section of Driver: %d\n", op_result);
		return count;
	}
	else
	{
		pr_err("Error in MUL- Writing Data from User to Kernel.\n");
		return -1;
	}
}

static ssize_t div_write(struct file *file, const char __user *ubuff, size_t count, loff_t *off)
{
	//down(&my_sem);
	printk("In DIV- WRITE Functionality\n");

	int kbuff[2];
	int result, num1, num2;

	result = copy_from_user((char *)kbuff, (char *)ubuff, sizeof(kbuff));

	if (result == 0)
	{
		printk("Success in DIV-  Writing Data from User to Kernel.\n");
		num1 = kbuff[0];
		num2 = kbuff[1];

		op_result = num1 / num2;
		printk("Result in DIV- Write Section of Driver: %d\n", op_result);
		return count;
	}
	else
	{
		pr_err("Error in DIV- Writing Data from User to Kernel.\n");
		return -1;
	}
}

//************** READ SECTION ************

static ssize_t add_read(struct file *file, char __user *ubuff, size_t count, loff_t *off)
{
	printk("In ADD- Read Functionality...\n");
	int result = copy_to_user((char *)ubuff, &op_result, sizeof(op_result));

	if(result == 0)
	{
	printk("Success in Reading Data from Kernel to User.\n");
	printk("In Read Section The Result is : %d \n",op_result);

	//up(&my_sem);
	return count;
	}
	else
	{
	pr_err("Error in Reading Data from Kernel to User\n");
	return -1;
	}
}

static ssize_t sub_read(struct file *file, char __user *ubuff, size_t count, loff_t *off)
{
	printk("In SUB- Read Functionality...\n");
	int result = copy_to_user((char *)ubuff, &op_result, sizeof(op_result));

	if(result == 0)
	{
	printk("Success in Reading Data from Kernel to User.\n");
	printk("In Read Section The Result is : %d \n",op_result);

	//up(&my_sem);
	return count;
	}
	else
	{
	pr_err("Error in Reading Data from Kernel to User\n");
	return -1;
	}
}

static ssize_t mul_read(struct file *file, char __user *ubuff, size_t count, loff_t *off)
{
	printk("In MUL- Read Functionality...\n");
	int result = copy_to_user((char *)ubuff, &op_result, sizeof(op_result));

	if(result == 0)
	{
	printk("Success in Reading Data from Kernel to User.\n");
	printk("In Read Section The Result is : %d \n",op_result);

	//up(&my_sem);
	return count;
	}
	else
	{
	pr_err("Error in Reading Data from Kernel to User\n");
	return -1;
	}
}

static ssize_t div_read(struct file *file, char __user *ubuff, size_t count, loff_t *off)
{
	printk("In DIV- Read Functionality...\n");
	int result = copy_to_user((char *)ubuff, &op_result, sizeof(op_result));

	if(result == 0)
	{
	printk("Success in Reading Data from Kernel to User.\n");
	printk("In Read Section The Result is : %d \n",op_result);

	//up(&my_sem);
	return count;
	}
	else
	{
	pr_err("Error in Reading Data from Kernel to User\n");
	return -1;
	}
}
 
// *********** MODULE INITIALIZATION ***************************************

static int calc_init(void)
{
	printk("CALCULATOR Initialization Function \n");
	if (alloc_chrdev_region(&dev_aj , 0, 4, "CalcDriver")<0)
	{
		pr_err("Major Num not Allocated\n");
		return -1;
	}
	pr_info("MAJOR NUMBER: %d\nMINOR NUMBER: %d\n",MAJOR(dev_aj),MINOR(dev_aj));

	my_dev = cdev_alloc();
	my_dev->ops = &fops;

	if(cdev_add(my_dev, dev_aj, 4)<0)
	{
		pr_err("Error registering Device with File Operation.\n");
		return -1;
	}

	//sema_init(&my_sem,1);
	return 0;

}

//*************** MODULE EXIT *********************************
static void calc_exit(void)
{
	printk("\n In CALCULATOR Exit Section\n");
	cdev_del(my_dev);
	printk("Success in Deactivate\n");
	unregister_chrdev_region(dev_aj, 4);
	printk("Success in Deregister\n");
}

// *********************************************

module_init(calc_init);
module_exit(calc_exit);

//**********LICENSE*************************

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Destroyer");
MODULE_DESCRIPTION("CALCULATOR ASSIGNMENT");