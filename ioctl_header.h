struct data
{
	int delay;
	char data[200];
};
#define STRUCT_WR	_IOW('a','a',struct data*)
#define DATA_RD	_IOR('a','b',struct data*)
#define STRUCT_RDWR	_IOWR('a','c',struct data*)
#define APP1_WR	_IOW('b','a',char*)
#define APP1_RD	_IOR('b','b',char*)
#define APP2_WR	_IOW('c','a',char*)
#define APP2_RD	_IOR('c','b',char*)