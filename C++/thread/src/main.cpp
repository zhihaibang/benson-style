#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include "Thread.h"
using namespace std;


int main() 
{
    Thread* newTh = new Thread();
    newTh->simpleThread();
    return 0;
}
