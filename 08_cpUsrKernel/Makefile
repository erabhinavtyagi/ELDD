obj-m := charDriver.o
 

ifdef ARCH

#RPI Compilation

KERNDIR = /lib/modules/5.15.48destroyer-v7l/build/

PWD := $(shell pwd)

all:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(KERNDIR)  M=$(shell pwd) modules
 
clean:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(KERNDIR)  M=$(shell pwd) clean

else

# x86 Compilation
KERNDIR = /lib/modules/$(shell uname -r)/build

PWD := $(shell pwd)

all:
	make -C $(KERNDIR)  M=$(shell pwd) modules
 
clean:
	make -C $(KERNDIR)  M=$(shell pwd) clean

endif
