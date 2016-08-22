libevent的一个简单demo

1. 安装libevent
tar –xzvf libevent-1.4.13-stable.tar.gz
cd libevent-1.4.13-stable
./configure --prefix=/usr/libevent
make && make install

2. 程序编译的时候要加-levent连接选项,以及头文件目录
INC = -I /usr/libevent/include
LIB = -L /usr/libevent/lib -levent

3. 在执行时，出现error while loading shared libraries: libevent-2.0.so.5: cannot open shared object file: No such file or directory，因为找不到共享库。
执行export LD_LIBRARY_PATH=/usr/libevent/lib/:$LD_LIBRARY_PATH将该路径加入系统环境变量里，再执行程序就可以了。
/usr/libevent/lib/是configure时的prefix参数内容
