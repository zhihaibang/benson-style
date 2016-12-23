#include <stdio.h>
#include "Singleton.h"

class Emperor{
	int id;
	char name[20];
};

int main()
{
	Emperor* emperor1 = Singleton<Emperor>::instance();
	Emperor* emperor2 = Singleton<Emperor>::instance();
	if(emperor1 == emperor2)
	{
		printf("is the same instance\n");
	}
	else
	{
		printf("is different instance\n");
	}
	return 0;
}