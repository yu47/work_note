## squid

	* 配置路径
		服务名：squid
		主程序：/usr/sbin/squid
		配置目录：/etc/squid
		主配置文件：/etc/squid/squid.conf
		监听tcp端口号：3128
		默认访问日志文件：/var/log/squid/access.log
	* 常见命令：
		修改配置：/etc/squid/suqid.conf
		重启squid: systemctl restart squid
		查看日志: tail -f /var/log/squid/access.log
	* 工作流程：
		* 1 客户端与代理建立TCP连接（三次握手）
		* 2 客户端给服务端发送请求；
		* 3 代理劫持客户端的请求，重新解析数据包，转发给服务器
		* 4 服务端响应代理的请求，发送响应给代理
		* 5 代理转发给客户端

### 修改IP

```
vi /etc/sysconfig/network-scripts/ifcfg-eth0  修改ip
vi /etc/resolv.conf  修改DNS
/etc/init.d/network restart  重启网卡
```

## sock5

```
* 安装编译环境和依赖
	* yum install gcc openldap-devel pam-devel openssl-devel -y
	* wget http://jaist.dl.sourceforge.net/project/ss5/ss5/3.8.9-8/ss5-3.8.9-8.tar.gz
	* tar xf ss5-3.8.9-8.tar.gz
	* cd ss5-3.8.9
	* ./configure 
make && make install
cd /etc/opt/ss5/

* 修改/etc/opt/ss5/ss5.passwd，一行写一个用户名和密码，中间空格分开，比如:
	* username1 pwd1
	* username2 pwd2

* 修改/etc/sysconfig/ss5，取消#SS5_OPTS=” -u root”的注释，并修改为
	SS5_OPTS=" -u root -b 0.0.0.0:1080"
* 其中1080是指定的端口号，可以使用其它端口（这步不设置的话默认使用1080端口）

* 增加执行权限
	chmod +x /etc/init.d/ss5

* 在chkconfig工具服务列表中增加此服务，设置开机自启（可选）
	chkconfig --add ss5
	chkconfig ss5 on

* 手动启动
	service ss5 start

* 默认端口1080,开防火墙
	/usr/sbin/iptables -I INPUT -p tcp --dport 1080 -j ACCEPT

* 每次重启计算机后或者出现以下错误时：
	Can’t create pid file /var/run/ss5/ss5.pid
	Can’t unlink pid file /var/run/ss5/ss5.pid
* 需要先创建 /var/run/ss5 目录，然后再启动 ss5
	echo 'mkdir /var/run/ss5/' >> /etc/rc.d/rc.local ;\
	chmod +x /etc/rc.d/rc.local ;\
	/sbin/chkconfig ss5 on

```

