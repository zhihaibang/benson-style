#include <stdio.h>

static int g_total;

extern "C" int handle_init(int start)
{
	g_total = start;
    printf("Init handle:%d\n",g_total);
    return 0;
}

extern "C" int handle_interval()
{
	g_total += 10;
    printf("Interval handle:%d\n",g_total);
    return 0;
}

extern "C" int handle_fini()
{
    printf("Finish handle");
    return 0;
}