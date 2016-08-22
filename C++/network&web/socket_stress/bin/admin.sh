#!/bin/sh  
PROGRAM=socket_stress
  
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
    echo -e "$RED USAGE: $0 $YELLOW option [start | stop]$RESET"  
    exit 0;  
fi  
  
  
dir=`echo $0|sed 's/admin.sh//g'`  
cd $dir  
  
if [ $1 = "start" ]  
then  
    nohup ./$PROGRAM&
	echo "program started."
  
elif [ $1 = "stop" ]  
then  
    for pid in `ps -ef|grep $PROGRAM |grep -v grep|awk '{print $2}'`  
    do        
        kill -10 $pid  
    done  
    echo "program stoped."
  
else  
    echo -e "$RED USAGE: $0 $YELLOW option [start | stop]$RESET"  
    exit 0;  
fi  
