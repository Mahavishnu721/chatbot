#include<stdio.h>
#include<stdlib.h>
#include<sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include"ioctl_header.h"

int main()
{
	char buf[200];
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
	printf("user app 1 is running\n");
	printf("Enter data to send to driver \n");
	scanf("%[^\n]s",buf);
	ioctl(dev,APP2_WR,&buf);
	printf("data send to user app1  \n");
	
	ioctl(dev,APP1_RD,&buf);
	printf("data from user app1 : %s\n",buf);
	
	/*printf("data send by RDWR \n");
	ioctl(dev,STRUCT_RDWR,&s);
	printf("rx data from kernal :\n");
	printf("%s\n",s.data);
	*/
	
	close(dev);
	printf("driver is close\n");
	return 0;
}
