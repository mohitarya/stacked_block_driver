obj-m += sdfdrv.o

sdfdrv-objs := sdfdrv_main.o helper_fun.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
