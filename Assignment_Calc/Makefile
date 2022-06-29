
obj-m := calcDriver.o

KERNELDIR = /lib/modules/$(shell uname -r)/build

PWD := $(shell pwd)

default :
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	gcc -o div div.c
	gcc -o sub sub.c
	gcc -o mul mul.c
	gcc -o add add.c

clean :
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm div sub mul add

