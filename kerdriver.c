#include<linux/fs.h>
#include<linux/module.h>
#include<linux/uaccess.h>
#include<linux/delay.h>
#include<linux/cdev.h>
#include<linux/slab.h>
#include<linux/mutex.h>

#include"ioctl_header.h"
struct mutex mutex;

#define pr_fmt(fmt) "%s:%s: " fmt, KBUILD_MODNAME, __func__

struct cdev *my_cdev;
dev_t dev_num;
struct class *my_class;
static char *app1_data;
static char *app2_data;

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
			copy_to_user((char *)arg,rx,sizeof(rx));
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
		case APP1_WR:
			copy_from_user(app1_data,(char*)arg,sizeof(app1_data));
			pr_info("recived data is : %s \n",app1_data);

 			break;
 		case APP1_RD:
			copy_to_user((char *)arg,app1_data,sizeof(app1_data));
			pr_info("data send to user app\n");
			break;
			
 		case APP2_WR:
			copy_from_user(app2_data,(char*)arg,sizeof(app2_data));
			pr_info("recived data is : %s \n",app2_data);

 			break;
 		case APP2_RD:
			copy_to_user((char *)arg,app2_data,sizeof(app2_data));
			pr_info("data send to user app\n");
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
	
	app1_data=(char *)kzalloc(4000,GFP_KERNEL);
	app2_data=(char *)kzalloc(4000,GFP_KERNEL);
	pr_info("init fun called\n");
	
	
	

	return 0;
}
static void exit_fun(void)
{
	
	device_destroy(my_class, dev_num);
	class_destroy(my_class);
	cdev_del(my_cdev);
	unregister_chrdev_region(dev_num, 1);
	kfree(app1_data);
	kfree(app2_data);
	pr_info("exit fun called\n");
}
module_init(init_fun);
module_exit(exit_fun);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("vishnu");
MODULE_DESCRIPTION("user app interact to kernel with diff delay");
