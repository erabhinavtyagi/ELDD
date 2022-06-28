obj-m := dyn_waitQ.o

KERNDIR = /lib/modules/$(shell uname -r)/build

PWD := $(shell pwd)

all:
	make -C $(KERNDIR) M=$(PWD) modules

clean:
	make -C $(KERNDIR) M=$(PWD) clean
