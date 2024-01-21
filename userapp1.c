#include<stdio.h>
#include<stdlib.h>
#include<sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include"ioctl_header.h"

int main()
{
	FILE* fptr;
	char c='h';
	int dev=open("/dev/kerdriver",O_RDWR);
	printf("current process id %d\n",getpid());
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
	printf("user app 1 is running\n");
	/*s.delay=5000;
	printf("Enter data to send to driver \n");
	scanf("%[^\n]s",s.data);
	//ioctl(dev,STRUCT_WR,&s);
	printf("data send to kernel \n");
	char buf[200];*/
	int i=1;
	do{
	fptr=fopen("spy.txt","a");
	if(fptr==NULL)
	{
		printf("spy not open \n");
		return 0;
	}
	
	ioctl(dev,DATA_RD,&c);
	printf("data from kernel is : %c\n",c);
	
	/*printf("data send by RDWR \n");
	ioctl(dev,STRUCT_RDWR,&s);
	printf("rx data from kernal :\n");
	printf("%s\n",s.data);
	*/
	
	putc(c,fptr);
	if(c=='x')
	{
	i=0;
	}
	
	fclose(fptr);
	}while(i==1);
	close(dev);
	printf("driver is close\n");
	return 0;
}

