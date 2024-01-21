obj-m := key.o
 
KDIR = /lib/modules/$(shell uname -r)/build
 
 
all:
	make -C $(KDIR)  M=$(shell pwd) modules
 
clean:
	make -C $(KDIR)  M=$(shell pwd) clean

cp:
	gcc userapp1.c -o ko

i:
	$(shell sudo insmod key.ko)

r:
	$(shell sudo rmmod key)
