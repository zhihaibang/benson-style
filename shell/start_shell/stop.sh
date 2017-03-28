#! /bin/sh
#进程名字可修改
PRO_NAME=fcgisample

pidnum=`ps -ef|grep ${PRO_NAME} |grep -v grep|wc -l`
if [ $pidnum -lt 1 ]
then
	echo "no program killed."
else
	for pid in `ps -ef|grep ${PRO_NAME} |grep -v grep|awk '{print $2}'`
	do		
		kill -9 $pid
	done
	echo "program stoped."
fi
exit 0