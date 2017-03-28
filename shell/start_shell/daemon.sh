#! /bin/sh
#进程名字可修改
PRO_NAME=zhb-t
#程序启动脚本命令
START_SH="./start.sh"

#用ps获取$PRO_NAME进程数量
NUM=`ps aux | grep ${PRO_NAME} | grep -v grep |wc -l`
#echo $NUM
#少于1，重启进程
if [ "${NUM}" -lt "1" ];then
    echo "${PRO_NAME} has been killed"
    ${START_SH}
    echo "${PRO_NAME} started"
fi
exit 0