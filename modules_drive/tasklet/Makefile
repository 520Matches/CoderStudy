# ifeq($(KERNELRELEASE),)
# ifeq($(ARCH),arm)
	#
# else
KERNELDIR ?= /lib/modules/$(shell uname -r)/build
# endif
PWD := $(shell pwd)

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) INSTALL_MOD_PATH=$(ROOTFS) modules_install

clean:
	rm -rf modules.order Module.symvers *.mod.* *.mod *.o .*.*.* 

# else
obj-m := tasklet.o
# endif
