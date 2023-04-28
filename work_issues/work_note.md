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

大明养小明 谁养大明 久(狗)仰大名