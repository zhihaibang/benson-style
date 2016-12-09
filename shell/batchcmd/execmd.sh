#!/bin/bash

user=root
passwd=root

hosts=("192.168.150.165" \
	  "192.168.150.166" \
	  "192.168.150.167");

LOG_PATH=/export/servers/log
TMP_FILE=tmp

cd $(dirname "$0")

for ip in ${hosts[@]};do
	./ssh_cmd.exp $ip $user $passwd "cd $LOG_PATH;grep ERROR ./*" > $TMP_FILE
done
