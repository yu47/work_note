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
