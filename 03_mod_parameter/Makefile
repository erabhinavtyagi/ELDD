obj-m := mod_parameter.o

KERN_DIR =/lib/modules/5.4.0-120-generic/build/

all :
	make -C $(KERN_DIR) M=$(PWD) modules

clean:  
	make -C $(KERN_DIR) M=$(PWD) clean
                           
