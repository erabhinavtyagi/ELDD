
obj-m := sem_tryl.o

KERNELDIR = /lib/modules/$(shell uname -r)/build

PWD := $(shell pwd)

default :
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	gcc -o wr_1 wr_1.c
	gcc -o wr_2 wr_2.c
	gcc -o rd rd.c

clean :
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm wr_1 wr_2 rd 

