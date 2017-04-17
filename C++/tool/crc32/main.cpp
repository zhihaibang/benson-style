#include <stdio.h>
#include <string.h>
#include "crc32.h"

int main(int argc,char **argv)
{
    const char *str = "shanghai";
    printf("%lld\n",crc32(reinterpret_cast<const unsigned char *>(str),strlen(str)));//需要借助reinterpret_cast强制转换
    return 0;
}