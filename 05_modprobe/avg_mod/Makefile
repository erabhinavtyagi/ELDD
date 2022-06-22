obj-m := avg.o

KERNDIR = /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNDIR) M=$(PWD) modules

clean:  
	$(MAKE) -C $(KERNDIR) M=$(PWD) clean