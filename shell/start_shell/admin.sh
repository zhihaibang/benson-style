#!/bin/sh

RED=\\e[1m\\e[31m
DARKRED=\\e[31m
GREEN=\\e[1m\\e[32m
DARKGREEN=\\e[32m
BLUE=\\e[1m\\e[34m
DARKBLUE=\\e[34m
YELLOW=\\e[1m\\e[33m
DARKYELLOW=\\e[33m
MAGENTA=\\e[1m\\e[35m
DARKMAGENTA=\\e[35m
CYAN=\\e[1m\\e[36m
DARKCYAN=\\e[36m
RESET=\\e[m

if [ $# != 1 ]
then
	echo -e "$RED USAGE: $0 $YELLOW option [start | stop | restart]$RESET"
	exit 0;
fi


dir=`echo $0|sed 's/admin.sh//g'`
cd $dir

if [ $1 = "start" ]
then
    ./start.sh

elif [ $1 = "stop" ]
then
    ./stop.sh

elif [ $1 = "restart" ]
then
    ./stop.sh
    ./start.sh

else
	echo -e "$RED USAGE: $0 $YELLOW option [start | stop | restart]$RESET"
	exit 0;
fi