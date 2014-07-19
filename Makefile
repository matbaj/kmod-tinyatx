obj-m += tinyatxmod.o 
ksrc   = "/usr/lib/modules/3.12.24-1-ARCH/build"
sysr   = "/"
mdir   = "kernel/drivers/watchdog"

all:
	make -C $(ksrc) M=$(PWD) modules

module_install:
	make -C $(ksrc) M=$(PWD) INSTALL_MOD_PATH=$(sysr) INSTALL_MOD_DIR=$(mdir) modules_install

clean:
	make -C $(ksrc) M=$(PWD) clean
