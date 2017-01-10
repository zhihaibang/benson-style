<?php
$ip="192.168.130.119";
$user="root";
$passwd="0okmnji9";
$log_path="/export/servers/log";
$sub_cmd="cd {$log_path};grep ERROR ./*";

$cmd = "./ssh_cmd.exp {$ip} {$user} {$passwd} \"{$sub_cmd}\"";
exec($cmd, $res, $rc);

foreach($res as $key=>$value)
{
	echo "{$value}\n";
}
