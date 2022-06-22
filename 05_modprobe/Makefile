obj-y :=  avg_mod/ add_mod/

KERNELDIR = /lib/modules/$(shell uname -r)/build

PWD := $(shell pwd)

default :   
	make -C $(KERNELDIR) M=$(PWD) modules

clean: 
	make -C $(KERNELDIR) M=$(PWD) clean

install:
	make -C $(KERNELDIR) M=$(PWD) modules_install 
	depmod -a
