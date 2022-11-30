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
		启动：service squid start   #最安全做法
		启动：/usr/sbin/squid -s  #后台启动,有可能不生效
		停止：squid -k shutdown
		重启配置：systemctl reload squid
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

### openSSL编译和使用

		* 下载openssl源代码
		* 网站：https://www.openssl.org/source/old/
		* 解压，进入openssl目录
		* ./config -fPIC no-shared
		* 其中，-fPIC：指示生成位置无关的代码，这个选项是在把openssl生成的静态库链接到动态库的时候提示错误添加的；no-shared：指示生成静态库。
		* 动态库 ./config -fPIC shared
		* make
		* 完成静态编译
		*  最终在当前目录下会编译出libssl.a和libcrypto.a两个库文件
		*  将include/openssl目录拷贝到自己模块的头文件目录（./inc）下，将libssl.a和libcrypto.a静态库拷贝到自己模块的库文件目录（./lib）下，在makefile中添加头文件目录和lib库：
	  INCLUDE += ******** -I./inc
	  LIB += ******* -L./lib -lssl -lcrypto
linux 查询程序依赖

	* objdump -p shell
	* ldd shell
	* readelf -s shell 
冰点 5f6Dq2YB

### Vscode root权限调试

```
root.sh 内容 pkexec /usr/bin/gdb "$@"

-----------------------------------------------------------------------------------------------------
launch.json 内容
{
    "version": "0.2.0",
    "configurations": [
     {
        "name": "shell",
        "type": "cppdbg",
        "request": "launch",
        "program": "/home/yu/work/foxdoor-base/userland/output/shell",
        "args": [
            "-t", "192.168.182.128", "-s", "s3cr3t", "-p", "443", "-r", "1",

        ],
        "stopAtEntry": false,
        "cwd": "/home/yu/work/foxdoor-base/userland/output",
        "environment": [],
        "externalConsole": false,
        "MIMode": "gdb",
        "miDebuggerPath": "/home/yu/work/foxdoor-base/root.sh", //root状态调试
        "setupCommands": [
            {
                "description": "为 gdb 启用整齐打印",
                "text": "-enable-pretty-printing",
                "ignoreFailures": true
            },
            {
                "description":  "将反汇编风格设置为 Intel",
                "text": "-gdb-set disassembly-flavor intel",
                "ignoreFailures": true
            }
        ],
        "preLaunchTask": "build"
     }
    ]
}

-----------------------------------------------------------------------------------------------------
tasks.json   内容
{
	"version": "2.0.0",
	"tasks": [
		{
			"type": "cppbuild",
			"label": "build",
			"command": "make",
			"args": [
				"${fileBasenameNoExtension}"
			],
			"options": {
				"cwd": "/home/yu/work/foxdoor-base/userland"
			},
			"problemMatcher": [
				"$gcc"
			],
            "group": {
                "kind": "build",
                "isDefault": true
            },
			"detail": "调试器生成的任务。"
		}
	]
}
```

### makeself

```
# 下载工具
wget https://github.com/megastep/makeself/releases/download/release-2.4.2/makeself-2.4.2.run

# 解压安装
bash makeself-2.4.2.run; cd makeself-2.4.2
sudo cp makeself.sh /usr/bin/makeself

#打包
makeself.sh [args] archive_dir file_name label startup_script [script_args]
运行脚本  [可选参数] 目标sh所在目录  打包完可执行文件名  sh文件（相对于archive_dir的路径） sh可选参数
```



- **[1] makeself 打包命令可选参数**

| 参数                        | 参数使用说明                                                 |
| :-------------------------- | :----------------------------------------------------------- |
| **`--version`**             | 打印版本号                                                   |
| **`--needroot`**            | 需要`root`用户才可以执行                                     |
| **`--gzip`**                | 默认使用`gzip`工具进行压缩                                   |
| **`--pigz`**                | `gzip`的多线程解决方案                                       |
| **`--bzip2`**               | 使用`bzip2`代替`gzip`进行更好的压缩                          |
| **`--pbzip2`**              | `bzip2`的多线程解决方案                                      |
| **`--xz`**                  | 使用`xz`代替`gzip`进行更好的压缩(远程支持多线程压缩)         |
| **`--zstd`**                | 使用`zstd`代替`gzip`进行更好的压缩                           |
| **`--threads thds`**        | 限制支持多线程解压缩工具所使用的线程数量                     |
| **`--base64`**              | 用`Base64`格式将存档编码为`ASCII`格式而不进行压缩            |
| **`--gpg-encrypt`**         | 使用`gpg -ac -z $COMPRESS_LEVEL`加密归档文件，需要输入密码   |
| **`--ssl-encrypt`**         | 使用`openssl aes-256-cbc -a -salt`加密归档文件，需要输入密码 |
| **`--ssl-passwd pass`**     | 设置`openssl`加密归档文件需要输入的密码                      |
| **`--compress`**            | 使用`UNIX系统的`compress`命令压缩数据(在没有 gzip 时默认)    |
| **`--nocomp`**              | 不对存档文件进行压缩，默认是一个`tar`包                      |
| **`--complevel lvl`**       | 指定压缩级别，默认使用`9`级别                                |
| **`--notemp`**              | 生成的归档文件不会将文件解压缩到临时目录，而解压到当前目录的创建的新目录中 |
| **`--current`**             | 文件将被提取到当前目录，而不是子目录中(同`--notemp`一样，但会提取没有文件夹) |
| **`--follow`**              | 存储指向的文件而不是链接本身                                 |
| **`--append file`**         | 将数据追加到现有存档中，而不是创建新文件                     |
| **`--header file`**         | 使用指定的脚本来代替默认的`makeself-header.sh`文件(默认与`makeself`文件同级) |
| **`--cleanup file`**        | 指定在执行中断或成功完成时运行的脚本                         |
| **`--nomd5/--nocrc`**       | 禁止检查存档文件的`MD5`或`CRC`校验和                         |
| **`--tar-extra opt`**       | 在`tar`命令行中附加更多选项                                  |
| **`--keep-umask`**          | 保持`umask`设置为`shell`默认值，而不是在执行自解压缩存档时覆盖 |
| **`--packaging-date date`** | 使用提供的字符串作为打包日期而不是当前日期                   |
| **`--license`**             | 附加许可证文件                                               |
| **`--nooverwrite`**         | 如果指定的目标目录已经存在则不要解压缩存档                   |
| **`--header file`**         | 指定头部信息脚本文件的位置，默认为`makeself-header.sh`文件   |
| **`--help-header file`**    | 在存档的`--help`输出中添加标题                               |
| **`--export-conf`**         | 将配置变量导出到`startup_script`脚本中                       |

- **[2] ARGS - 需要注意的选项**

| 选项                 | 说明                                         |
| :------------------- | :------------------------------------------- |
| **`archive_dir`**    | 包含要归档的文件的目录的名称                 |
| **`file_name`**      | 要创建的存档名称                             |
| **`label`**          | 描述存档包的任意描述并在解压的时候显示       |
| **`startup_script`** | 解压后要执行的脚本，需指定`./`在当前目录执行 |
| **`[script_args]`**  | `startup_script`脚本的附加参数               |

- **[3] ARGS - 解包的可选参数**

| 参数                   | 参数使用说明                                                 |
| :--------------------- | :----------------------------------------------------------- |
| **`--keep`**           | 文件将被提取到当前工作目录中并将一直保留到您删除它们为止     |
| **`--verbose`**        | 在执行嵌入式脚本之前将提示用户                               |
| **`--target dir`**     | 允许在任意指定目录解压`run`包                                |
| **`--confirm`**        | 在执行嵌入式脚本之前将提示用户是否需要执行                   |
| **`--info`**           | 打印出有关`run`包的常规信息                                  |
| **`--list`**           | 列出档案中的文件                                             |
| **`--check`**          | 校验和检查归档文件的完整性                                   |
| **`--nochown`**        | 默认情况下提取后在目标目录上运行`chown -R`命令以便所有文件都属于当前用户 |
| **`--tar`**            | 使用以下参数作为`tar`命令的参数                              |
| **`--noexec`**         | 提取后不要运行嵌入式脚本                                     |
| **`--nodiskspace`**    | 在提取之前不检查可用的磁盘空间                               |
| **`--noexec-cleanup`** | 在提取之前不支持指定的删除脚本                               |