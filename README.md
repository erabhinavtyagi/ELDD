# Embedded Linux Device Driver

## Day 1 & 2
- Toolchain
- Cross-compiler and kernel environment for rpi4

I. Steps to flash Raspbian OS onto sd card:

On Host(ubuntu) :
1. Open terminal
```
$ sudo apt install rpi-imager
$ rpi-imager
```
Choose OS : Raspberry Pi OS (other) => raspberry pi OS LITE 32-bit
Choose storage : choose your sd card
Click on write and then click on yes - This will take some time.

2. After completing flashing image plug out sd card and insert sd card again.
```
$ cd /media/<user-name>/boot
$ touch ssh
$ touch wpa_supplicant.conf
$ vim wpa_supplicant.conf
```
Write the following code in wpa_supplicant.conf file and save it.
```
country=IN
ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
network={
ssid=”<your SSID>”
psk=”your password”
key_mgmt=WPA-PSK
}
```
3. Plug out the SD card and insert into your raspberry pi board.
4. Board will start booting and access it.

II. Steps for cross-compiling kernel :
On Host (ubuntu) :
1. Install Required dependencies :
```
$ sudo apt install git bc bison flex libssl-dev make libc6-dev libncurses5-dev
```
2. Install 32-bit toolchain
```
$ sudo apt install crossbuild-essential-armhf
```
3. Download/clone kernel source
```
~ $ mkdir rpi
$ cd rpi
$ git clone --depth=1 --branch rpi-5.15.y https://github.com/raspberrypi/linux
$ cd linux
```

4. Apply the config file of rpi4 :
Check config file for your board(rpi4) using below command
```
$ ls arch/arm/configs
```
Default config file for rpi4 is bcm2711_defconfig
Now apply config file using below command
```
$ make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- bcm2711_defconfig
```

5. Build kernel image and kernel modules for rpi4 :
```
$ make -j8 ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- zImage modules
```

Result of above command :
```
$ ls arch/arm/boot
zImage
```

6. Plug in your sd card to your HOST PC(ubuntu)
```
$ cp arch/arm/boot/zImage /media/<user_name>/boot
```

7. Install modules onto rootfs partition of SDcard
```
$ make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-
INSTALL_MOD_PATH=<path-to-sdcard rootfs partition> modules_install
```
Example In my pc :
$ make ARCH=arm
CROSS_COMPILE=arm-linux-gnueabihf-INSTALL_MOD_PATH=/media/embedded/rootfs
modules_install

Modules gets installed in rootfs/lib/modules path

8. Configuring config.txt to boot our new kernel
```
$ cd /media/<user-name>/boot
```
Open config.txt:
```
$ vim config.txt
```
Add below line at the end of the file and save file :
```
kernel=zImage
```
9. If “ssh” and “wpa_supplicant.conf” files are not in your boot partition then follow
steps of 2 of flashing raspbian OS.
10. Plug out sd card and insert into your raspberry pi board.
11. Board will start booting and access it .


III. Cross compile module for the Raspberry Pi.

1. Write a source code on HOST Machine(Ubuntu)

hello.c
```
#include<linux/module.h>
#include<linux/init.h>
static int __init hello_init(void)
{
pr_info("Hello World\n");
return 0;
}
static void __exit hello_exit(void)
{
pr_info("Good Bye\n");
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("CDAC");
MODULE_DESCRIPTION("A simple hello_world kernel module");
MODULE_INFO(board,"RASPBERRY PI 4");
```

Makefile for Cross Compilation.

```
obj-m := hello.o
KERN_DIR=/lib/modules/5.10.52-v7l+/build/
all:
    Make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(KERN_DIR)
    M=$(PWD) modules
clean:
    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(KERN_DIR)
    M=$(PWD) clean
```    

- After compiling the module, Copy the Kernel Object(.ko) file to the RaspberryPi.
- You can copy the contents from host to destination using command,
scp -r <path-to-source> <path-to-destination>

Example-
scp hello.ko /home/pi/<your-folder>

IV. Native compile for the Raspberry Pi

hello.c
- source code will be same - refer earlier hello.c file.

Makefile for the Native Compilation
```
obj-m := hello.o
KERN_DIR=/lib/modules/$(shell uname -r)/build/
all:
    make -C $(KERN_DIR) M=$(PWD) modules
clean:
    make -C $(KERN_DIR) M=$(PWD) clean
```

## Day 3
- Cross Compilation & Kernel configuration for RaspberryPi-4B

## Day 4
- Program "Hello World" using Native Compiler
- Program "Hello World" using Cross Compiler

## Day 5
- Module Parameters (command line arguments)
- Export Symbol (Stacking and unstacking of modules)
- Used Modprobe command to add the dependent modules automatically
Use following command to install the dependent modules

```bash
make
make install
```
and check the folder: /lib/modules/<uname -r>/extra
Then Run:
'''
modprobe -i avg
modprobe -r avg
'''
'i' - To Insert Kernel Modules
'r' - To Remove Kernel Modules

## Day 6

**Major Number and Minor Number**

1. Static Allocation
    MKDEV(250, 0)
    register_chrdev_region(device, 1, "simplechardriver")

2. Dynamic Allocation
    alloc_chrdev_region(&device, 0, 1, "charDeviceDemo")

**CREATING DEVICE FILE**

1. Using "mknod" Method

After module insertion commands used are:
```bash
mknod -m 777 /dev/char c 234 0
```
Here,
'777' is permissions
/dev/char is name of Device File
'c' - Char Device
'234' - Major Number which is allocated by kernel to us dynamically
'0' - Minor Number 

Create a device file dynamically using mknod and then check using "ls -l /dev/"

cross check it using following command:
```
cat /proc/devices
```
Unload the driver using "sudo rmmod".

2. Using "Struct class" Method

```
class_create(THIS_MODULE,"CharDevClass");
```

## Day 7

- cdev Structure
- File Operation Structure
- File Structure
- inode Structure

### **Assignments**:

1. Write a Character Driver with open and close functionality.
    Test the driver by writing an application that opens and closes the device driver. When open or closed, the open and close calls in the driver should be executed.

2. Write a Character Driver with open, read, write, close.
    Test the driver through a User Application by reading data from driver and writing data to the Driver.

3. Write a Character Driver to Dynamically allocate a major,minor number pair from kernel.
    Test the same and conclude.

4. Write a Calculator Driver in the Kernel which performs the following:
- **Create 4 Device Numbers**: Each device number depicts a specific calculation operation like Add, Subtract, Multiply, Divide.
- **Implement 8 Methods**: Read_add, Read_sub, Read_mul, Read_div, Write_add, Write_sub, Write_mul, Write_div.
- In User Space, create 4 devices nodes for the 4device numbers created -/dev/AddDev, /dev/SubDev, /dev/MulDiv, /dev/DivDev.
- Write 4 user applications in the user space to test the above. If /dev/AddDev application is run, it should write 2 numbers to the kernel and the kernel should add it and return the sum in the subsequent read.

## Day 8

- IOCTL (Input-Output Control)
- Program to implement "IOCTL".

## Day 9

- Wait Queue
    - Initialization
    - Process going to sleep
    - Waking up queued task    
- Program to implement its concept.