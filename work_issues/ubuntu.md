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