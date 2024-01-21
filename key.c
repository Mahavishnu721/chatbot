#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include<asm/io.h>



#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/mutex.h>
#include"ioctl_header.h"
struct mutex mutex;

#define pr_fmt(fmt) "%s:%s: " fmt, KBUILD_MODNAME, __func__

struct cdev *my_cdev;
dev_t dev_num;
struct class *my_class;


DECLARE_COMPLETION(data_read_done);


const unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};


static int irq = 1,  dev = 0xaa;
#define KBD_DATA_REG        0x60    /* I/O port for keyboard data */
#define KBD_SCANCODE_MASK   0x7f
#define KBD_STATUS_MASK     0x80
char c;
static irqreturn_t keyboard_handler(int irq, void *dev)
{
	char scancode;
	scancode = inb(KBD_DATA_REG);
	pr_info("scancode : %c %d %x\n",scancode,scancode,scancode);
	pr_info("Character %c %s\n",
			kbdus[scancode & KBD_SCANCODE_MASK],
			scancode & KBD_STATUS_MASK ? "Released" : "Pressed");
	c=kbdus[scancode & KBD_SCANCODE_MASK];
	pr_info("char is : %c ",c);
	if(!(scancode & KBD_STATUS_MASK)){
	complete (&data_read_done);
	pr_info("complete Event...\n");}
        return IRQ_NONE;
}
/* registering irq *//*
static int test_interrupt_init(void)
{
        
}

static void test_interrupt_exit(void)
{
        
}
*/


static int my_open (struct inode *inode, struct file *file)
{
	pr_info("open fun called\n");
	return 0;
}
static int my_close (struct inode *inode, struct file *file)
{
	pr_info("close fun called\n");
	return 0;
}
static long (my_ioctl) (struct file *file, unsigned int cmd, unsigned long arg)
{
	struct data test;
	char rx[200]="success";
	
	switch(cmd)
	{
		case STRUCT_WR:
			mutex_lock(&mutex);
			copy_from_user(&test,(struct data *)arg,sizeof(test));
			pr_info("recived data is : delay %d, string %s \n",test.delay,test.data);
			msleep(test.delay);
			mutex_unlock(&mutex);
			break;
		case DATA_RD:
			pr_info("data_rd in c value is %c\n",c);
			pr_info("Waiting For Event...\n");
                	wait_for_completion (&data_read_done);
			copy_to_user((char *)arg,&c,sizeof(char));
			pr_info("data send to user app\n");
			break;
		case STRUCT_RDWR:
			mutex_lock(&mutex);
			copy_from_user(&test,(struct data *)arg,sizeof(test));
			pr_info("recived data is : delay %d, string %s \n",test.delay,test.data);
			msleep(test.delay);
			struct data new={3300,"success"};
			copy_to_user((struct data *)arg,&new,sizeof(new));
			pr_info("data send to user app\n");
			mutex_unlock(&mutex);
			break;
		default:
			pr_info("default run\n");
			break;
			}
		
		return 0;
}

struct file_operations my_fops={
	.owner=THIS_MODULE,
		.open=my_open,
		.release=my_close,
		.unlocked_ioctl=my_ioctl,
};

static int init_fun(void)
{
	alloc_chrdev_region(&dev_num,0,1,"kerdriver");
	my_cdev=cdev_alloc();
	cdev_init(my_cdev,&my_fops);
	cdev_add(my_cdev,dev_num,1);
	my_class=class_create("class_ioctl");
	device_create(my_class, NULL, dev_num,NULL,"%s", "kerdriver");
	mutex_init(&mutex);
	
	pr_info("interrupt init\n");
        request_irq(irq, keyboard_handler, IRQF_SHARED,"my_keyboard_handler", &dev);
	
	
	pr_info("init fun called\n");
	
	
	

	return 0;
}
static void exit_fun(void)
{
	
        synchronize_irq(irq); /* synchronize interrupt */
        free_irq(irq, &dev);
	
	
	device_destroy(my_class, dev_num);
	class_destroy(my_class);
	cdev_del(my_cdev);
	unregister_chrdev_region(dev_num, 1);
	pr_info("exit fun called\n");
}
module_init(init_fun);
module_exit(exit_fun);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("vishnu");
MODULE_DESCRIPTION("user app interact to kernel with diff delay");


