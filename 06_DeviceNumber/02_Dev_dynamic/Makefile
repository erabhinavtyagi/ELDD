
obj-m := dev_dynamic.o

KERN_DIR = /lib/modules/$(shell uname -r)/build/

PWD := $(shell pwd)

all:
	make -C $(KERN_DIR) M=$(PWD) modules

clean:
	make -C $(KERN_DIR) M=$(PWD) clean
