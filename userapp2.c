#include<stdio.h>
#include<stdlib.h>
#include<sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include"ioctl_header.h"

int main()
{
	int dev=open("/dev/kerdriver",O_RDWR);
	if(dev<0)
	{
		printf("driver cannot open\n");
		return -1;
	}
	else
	{
		printf("driver open success\n");
	}
	struct data s;
	printf("user app 2 is running\n");
	s.delay=10000;
	printf("Enter data to send to driver \n");
	scanf("%[^\n]s",s.data);
	ioctl(dev,STRUCT_WR,&s);
	printf("data send to kernel \n");
	char buf[200];
	ioctl(dev,DATA_RD,buf);
	printf("data from kernel is %s\n",buf);
	
	printf("data send by RDWR \n");
	ioctl(dev,STRUCT_RDWR,&s);
	printf("rx data from kernal :\n");
	printf("%s\n",s.data);
	
	close(dev);
	printf("driver is close\n");
	return 0;
}
