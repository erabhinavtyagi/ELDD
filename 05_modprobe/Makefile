obj-m := add.o avg.o

KERNELDIR = /lib/modules/5.4.0-121-generic/build

all :   
	make -C $(KERNELDIR) M=$(PWD) modules

clean: 
	make -C $(KERNELDIR) M=$(PWD) clean

install:
	make -C $(KERNELDIR) M=$(PWD) modules_install 
	depmod -a
