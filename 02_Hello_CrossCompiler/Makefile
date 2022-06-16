
obj-m := hello.o

KERNELDIR = /lib/modules/5.15.45-v7l+/build

all:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(KERNELDIR) M=$(PWD) modules

clean:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(KERNELDIR) M=$(PWD) clean

