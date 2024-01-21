struct data
{
	int delay;
	char data[200];
};
#define STRUCT_WR	_IOW('a','a',struct data*)
#define DATA_RD	_IOR('a','b',struct data*)
#define STRUCT_RDWR	_IOWR('a','c',struct data*)
