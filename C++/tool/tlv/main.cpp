#include <stdio.h>
#include "Message.h"
using namespace std;

int main(int argc, char const *argv[])
{
	MessageEncoder pa;
	uint16_t cmd = 0x0;
	uint32_t partition = 0x1;
    pa.set_cmd(cmd);
    pa.set_partition(partition);
    pa.AddValue(0x0, "a");

    int len = pa.len();
    char* data = (char*)pa.buffer();

    printf("MessageEncoder\nlen:%d\nstr:\n", len);
	
	for(int i =0; i<len; ++i)
	{
		printf("%x\t", data[i]);
	}
	
	printf("\n");
	
    MessageDecoder d(data);
    cmd = d.cmd();
    partition = d.partition();

    string value0;
	d.GetValue(0x0,value0);

    printf("MessageDecoder\ncmd:%u,partition:%u,value0:%s\n", cmd, partition, value0.c_str());
	return 0;
}