obj-m := kerdriver.o
 
KDIR = /lib/modules/$(shell uname -r)/build
 
 
all:
	make -C $(KDIR)  M=$(shell pwd) modules
 
clean:
	make -C $(KDIR)  M=$(shell pwd) clean

u1:
	gcc userapp1.c -o u1
u2:
	gcc userapp2.c -o u2

i:
	$(shell sudo insmod kerdriver.ko)

r:
	$(shell sudo rmmod kerdriver)
