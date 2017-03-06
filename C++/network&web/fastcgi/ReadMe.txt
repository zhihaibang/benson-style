一个c++构建的web应用demo，应用了fastcgi和nginx

集成了webapplib，该库封装了cgi的常用操作，获取解析参数，cookies等。
http://www.open-open.com/lib/view/open1345192887803.html

我将其改造成支持fastcgi的类库。

nginx配置文件：
server {
        listen 8100;

        location / {
            root /export/servers/nginx;
            index index.html;
        }

        location ~ wsp.cgi$ {           
            fastcgi_pass 127.0.0.1:9988;
			#fastcgi_pass unix:/dev/shm/fcgisample.sock;
            fastcgi_index index.cgi;
            #fastcgi_param SCRIPT_FILENAME /root/zhb/cpp/my_practice/fastcgi/bin$fastcgi_script_name;
            include fastcgi_params;
        }
}

访问地址：http://192.168.144.120:8100/wsp.cgi?username=133&email=12132，post的body参数为password=sdfdsf
返回内容：
username: 133
email: 12132
password: sdfdsf
usercookie: 133

具体教程：http://blog.csdn.net/okiwilldoit/article/details/51659170