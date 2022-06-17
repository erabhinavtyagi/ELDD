obj-m := kern_sym.o add_sym.o 

KERNELDIR = /lib/modules/5.4.0-120-generic/build

all :   
	make -C $(KERNELDIR) M=$(PWD) modules

clean: 
	make -C $(KERNELDIR) M=$(PWD) clean
