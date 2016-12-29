# SSM
 SSM框架——详细整合教程（Spring+SpringMVC+MyBatis）
 博客地址：
 http://www.cnblogs.com/zyw-205520/p/4771253.html
 
# 使用MyBatis Generator自动创建代码
打开cmd，进入lib目录下，执行脚本：
Java -jar mybatis-generator-core-1.3.2.jar -configfile generatorConfig.xml -overwrite
 
# 打包成war部署到tomcat下
Run As -> 6Maven build，在Goals里填package命令，执行就可以生成war包。
生成的war包在target目录下，然后放到tomcat目录的webapp下，就可以访问项目。

访问地址：
http://localhost:8080/maven02/user/jsontype/1
