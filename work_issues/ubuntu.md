### ubuntu扩容

1. 删除所有快照

2. 关机，在vm设置内存

3. ubuntu 安装 gparted

4. 打开gparted   选择根目录  Resize/Move

   

### [dpkg: 处理软件包 xxx (--configure)时出错 解决办法 ](https://www.cnblogs.com/dylancao/p/10226090.html)

##### 第一步：备份

```
sudo mv /var/lib/dpkg/info /var/lib/dpkg/info.bk
```

##### 第二步：新建

```
sudo mkdir /var/lib/dpkg/info
```

##### 第三步：更新

```
sudo apt-get update
sudo apt-get -f install
```

##### 第四步：替换

```
sudo mv /var/lib/dpkg/info/* /var/lib/dpkg/info.
//把更新的文件替换到备份文件夹
```

##### 第五步：删除

`$ sudo rm -rf /var/lib/dpkg/info` 


##### 第六步：还原

`$ sudo mv /var/lib/dpkg/info.bk /var/lib/dpkg/info` 

### 创建指定大小文件

```
dd if=/dev/zero of=文件名 bs=1M count=50
dd if=/dev/zero of=文件名 bs=1G count=20
bs=1M表示每一次读写1M数据，count=50表示读写 50次，这样就指定了生成文件的大小为50M。bs参数还可以进一步细分为ibs和obs两种，为读操作与写操作分别指定不同的Buffer大小。
```

### 网络没有wired 无法配置

```
sudo service network-manager stop
sudo rm /var/lib/NetworkManager/NetworkManager.state
sudo service network-manager start
sudo gedit /etc/NetworkManager/NetworkManager.conf
把false改成true
sudo service network-manager restart

```

### 跟踪服务程序

```
strace  -T -tt -e trace=all -p pid
```

### DNS

```
vi /etc/resolv.conf
```

### 替换动态库

```
sudo ldconfig 刷新系统库缓存
```

### 关闭防火墙

```
service iptables stop
systemctl stop firewalld.service 
systemctl stop firewalld
```

### 根据进程名删除

```
(ps -ef | grep "gedit 123"  | grep -v 'grep --color=auto' | awk '{print $2}') | xargs kill -9
```

### (ERROR 1698 (28000): Access denied for user 'root'@'localhost')

1、通过sudo进入mysql，ubuntu默认sudo不需要密码就可以进

```bash
ubuntu@ubuntu:~$ sudo mysql
```

2、进入mysql库

```bash
mysql> use mysql;
```

3、改密码

```bash
mysql> ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY '123456';
```

### 动态库路径添加

```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:XXX
静态库
LIBRARY_PATH
可执行程序
PATH
C头文件
C_INCLUDE_PATH
C++ 头文件
CPLUS_INCLUDE_PATH
pkg-config路径
PKG_CONFIG_PATH	
```

### 静态编译

```
./configure --disable-shared --enable-static
```

### 编译protobuf-c

```
1、首先安装依赖环境
	sudo apt-get install autoconf automake libtool curl make g++ unzip
2、下载源码
	git clone https://github.com/protocolbuffers/protobuf.git
3、进入源码文件夹
	cd protobuf/
4、版本回滚--v3.20.3
	git checkout fe271ab	
5、解压后进入到解压目录
	cd protobuf-3.20.3/
6、运行如下命令
	./autogen.sh
	./configure
	sudo make
	sudo make check
	sudo make install
	sudo ldconfig				// 更新配置
	protoc --vertion            // 在任何目录下输入这个命令，查看protoc版本，出现版本号即成功


7、下载protobuf-c
    git clone https://github.com/protocolbuffers/protobuf.git
    cd protobuf
    ./autogen.sh
    ./configure
    make
    sudo make install

protoc --c_out=. student.proto

-------------------------------------
syntax = "proto2";
message Student
{
    required string name    = 1;
    required uint32 num     = 2;
    required uint32 c_score = 3;
}
-------------------------------------
```





### 系统备份





### 自动补全依赖

```
sudo apt install aptitude
aptitude install xxx
```

