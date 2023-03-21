### strrchr

C 库函数 **char \*strrchr(const char \*str, int c)** 在参数 **str** 所指向的字符串中搜索最后一次出现字符 **c**（一个无符号字符）的位置。



```
temp = strrchr(args[1], '/'); 
```

#### bzero()

```
#include <string.h>
// bzero() 会将内存块（字符串）的前n个字节清零;
// s为内存（字符串）指针，n 为需要清零的字节数。
// 在网络编程中会经常用到。
void bzero(void *s, int n);
```

#### tcgetattr函数与tcsetattr函数

```
为了便于通过程序来获得和修改终端参数，Linux还提供了tcgetattr函数和tcsetattr函数。tcgetattr用于获取终端的相关参数，而tcsetattr函数用于设置终端参数。
```

### isatty

```
        常用设备名：
        stdin   标准输入设备  键盘
        stdout  标准输出设备  显示器
        stderr  标准错误设备  
        stdaux  辅助设备
        stdprn  打印机
```

### atoi

字符串数字 转化为 int

### oiat

int 转 char

###  strtok

```
分解字符串： strtok(NULL, delim)
#include <string.h>
#include <stdio.h>
 
int main () {
   char str[80] = "This is - www.runoob.com - website";
   const char s[2] = "-";
   char *token;
   
   /* 获取第一个子字符串 */
   token = strtok(str, s);
   
   /* 继续获取其他的子字符串 */
   while( token != NULL ) {
      printf( "%s\n", token );
    
      token = strtok(NULL, s);
   }
   printf("\n%s\n",str);
   return(0);
}
输出：

This is 
 www.runoob.com 
 website

This is 
```

getopt()函数用于解析命令行参数。

```
optarg和optind是两个最重要的external 变量。optarg是指向参数的指针（当然这只针对有参数的选项）；
optind是argv[]数组的索引，众所周知，argv[0]是函数名称，所有参数从argv[1]开始，所以optind被初始化设置指为1。
每调用一次getopt()函数，返回一个选项，如果该选项有参数，则optarg指向该参数。
在命令行选项参数再也检查不到optstring中包含的选项时，返回-1。
字符串optstring，它是作为选项的字符串的列表。
函数getopt()认为optstring中，以’-’开头的字符（注意！不是字符串！！）就是命令行参数选项，
有的参数选项后面可以跟参数值。optstring中的格式规范如下：
1) 单个字符，表示选项，
2) 单个字符后接一个冒号”:”，表示该选项后必须跟一个参数值。参数紧跟在选项后或者以空格隔开。该参数的指针赋给optarg。
3) 单个字符后跟两个冒号”::”，表示该选项后必须跟一个参数。参数必须紧跟在选项后不能以空格隔开。该参数的指针赋给optarg。
```

### fork

```
  一个进程，包括代码、数据和分配给进程的资源。fork（）函数通过系统调用创建一个与原来进程几乎完全相同的进程，也就是两个进程可以做完全相同的事，但如果初始参数或者传入的变量不同，两个进程也可以做不同的事。
    一个进程调用fork（）函数后，系统先给新的进程分配资源，例如存储数据和代码的空间。然后把原来的进程的所有值都复制到新的新进程中，只有少数值与原来的进程的值不同。相当于克隆了一个自己。
```

### setsid

创建会话

### 文件操作open     create    

```C
int open(const char *pathname, int flags);
int open(const char *pathname,int flags, mode_t mode);
int creat(const char *pathname, mode_t mode);
pathname ：打开文件的路径名
flags：用来控制打开文件的模式
mode：用来设置创建文件的权限（rwx）。当flags中带有 O_CREAT 时，也就是创建新文件时才有效。
O_RDONLY：只读模式
O_WRONLY：只写模式
O_RDWR：可读可写模式
O_APPEND 以追加的方式打开文件
O_CREAT 创建一个文件
O_EXEC 如果使用了O_CREAT而且文件已经存在，就会发生一个错误
O_NOBLOCK 以非阻塞的方式打开一个文件
O_TRUNC 如果文件已经存在，则删除文件的内容

int fd = open("/home/fuller/work//Reptile/userland/log",O_CREAT | O_WRONLY,0644);
lseek(fd,0,SEEK_END);
write(fd,log,strlen(log));
write(fd,"\n",2);
close(fd);
```

### sprintf  

```C
int local_size = 1234567
char *_size = (char*)malloc(32);
sprintf(_size, "%d", local_size);//整数转化为字符串
```
### lseek

```
lseek(fd,0,SEEK_SET); 光标移动到看开头
lseek(fd,0,SEEK_END); 光标移动到看结尾
lseek(fd,pos,SEEK_CUD); 光标移动到看根据当前光标前移
```

### fork调试子线程 ：  -exec set follow-fork-mode child

### C语言execlp()函数：

```
从PATH 环境变量中查找文件并执行
execl("/bin/ls", "ls", "-al", "/etc/passwd", (char *)0);
```

### ftruncate(fd,0);  清空数据
### socket

![05232335-fb19fc7527e944d4845ef40831da4ec2](../images\05232335-fb19fc7527e944d4845ef40831da4ec2.png)

* 服务端

  * 1）创建服务端的socket。
  
  * ```
    socket(AF_INET,SOCK_STREAM,0)
    * AF_APPLETALK  Apple Computer Inc. Appletalk 网络
    * AF_INET6   Internet IPv6 和 IPv4 系列
    * AF_INET  仅 Internet IPv4 系列
    * AF_PUP   Xerox Corporation PUP internet
    * AF_UNIX  UNIX 文件系统
    ```
  
  * 2）把服务端用于通信的地址和端口绑定到socket上。
  
  * ```
    servaddr.sin_family = AF_INET;  // 协议族，在socket编程中只能是AF_INET。
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);          // 任意ip地址。
    //servaddr.sin_addr.s_addr = inet_addr("192.168.190.134"); // 指定ip地址。
    servaddr.sin_port = htons(atoi(argv[1]));  // 指定通信端口。
    bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr)
    ```
  
  * 3）把socket设置为监听模式。
  
  * ```
    listen(listenfd,5) 
    ```
  
  * 4）接受客户端的连接。
  
  * ```
    accept(listenfd,(struct sockaddr *)&clientaddr,(socklen_t*)&socklen);
    ```
  
  * 5）与客户端通信，接收客户端发过来的报文后，回复处理结果
  
  * ```
    recv(clientfd,buffer,sizeof(buffer),0)
    ```
  
  * 6）不断的重复第5）步，直到客户端断开连接。
  
  * ```
    send(clientfd,buffer,sizeof(buffer),0)
    ```
  
  * 7）关闭socket，释放资源。
  
  * ```
    close(clientfd)
    ```

* 客户端

  * 1）创建客户端的socket。

  * ```
    socket(AF_INET,SOCK_STREAM,0)
    * AF_APPLETALK  Apple Computer Inc. Appletalk 网络
    * AF_INET6   Internet IPv6 和 IPv4 系列
    * AF_INET  仅 Internet IPv4 系列
    * AF_PUP   Xerox Corporation PUP internet
    * AF_UNIX  UNIX 文件系统
    ```

    2）向服务器发起连接请求。

    ```
    connect(sockfd, (struct sockaddr *)&servaddr,sizeof(servaddr))
    ```

    3）与服务端通信，发送一个报文后等待回复，然后再发下一个报文。

    ```
    send(clientfd,buffer,sizeof(buffer),0)
    ```

    4）不断的重复第3）步，直到全部的数据被发送完。

    5）第4步：关闭socket，释放资源。

### openssl https  服务器
![20160806095300994 (1)](..\images\20160806095300994 (1).png)

```c
    char header[512]={0};
    char temp[128]={0};
    int sockfd, len;
    struct sockaddr_in dest;
    char buffer[MAXBUF + 1];
    SSL_CTX *ctx;
    SSL *ssl;
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(SSLv23_client_method());
    if (ctx == NULL) {
        return -1;
    }

    /* 创建一个 socket 用于 tcp 通信 */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    /* 初始化服务器端（对方）的地址和端口信息 */
    bzero(&dest, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr (SERVER); /* Server IP */
    dest.sin_port = htons (PORT); /* Server Port number */

    /* 连接服务器 */
    if (connect(sockfd, (struct sockaddr *) &dest, sizeof(dest)) != 0) {
        return -1;
    }


    // SSL_CTX_use_certificate_file (ctx, "./server.pem", SSL_FILETYPE_PEM);
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sockfd);
    /* 建立 SSL 连接 */
    if (SSL_connect(ssl) == -1)
        return -1;
```

### dlopen 打开linux动态链接库

```
dlopen：该函数将打开一个新库，并把它装入内存。该函数主要用来加载库中的符号，这些符号在编译的时候是不知道的。这种机制使得在系统中添加或者删除一个模块时，都不需要重新进行编译。
dlsym：在打开的动态库中查找符号的值。
dlclose：关闭动态库。
dlerror：返回一个描述最后一次调用dlopen、dlsym，或dlclose的错误信息的字符串。

-  void * dlopen( const char * pathname, int mode )：函数以指定模式打开指定的动态连接库文件，并返回一个句柄给调用进程。
-  void* dlsym(void* handle,const char* symbol)：dlsym根据动态链接库操作句柄(pHandle)与符号(symbol)，返回符号对应的地址。使用这个函数不但可以获取函数地址，也可以获取变量地址。
-  int dlclose (void *handle)：dlclose用于关闭指定句柄的动态链接库，只有当此动态链接库的使用计数为0时,才会真正被系统卸载。
-  const char *dlerror(void)：当动态链接库操作函数执行失败时，dlerror可以返回出错信息，返回值为NULL时表示操作函数执行成功。


dlopen（）需要两个参数：一个文件名和一个标志。文件名就是一个动态库so文件，标志指明是否立刻计算库的依赖性。如果设置为 RTLD_NOW 的话，则立刻计算；如果设置的是 RTLD_LAZY，则在需要的时候才计算。另外，可以指定 RTLD_GLOBAL，它使得那些在以后才加载的库可以获得其中的符号。当库被装入后，可以把 dlopen() 返回的句柄作为给 dlsym() 的第一个参数，以获得符号在库中的地址。使用这个地址，就可以获得库中特定函数的指针，并且调用装载库中的相应函数。

在dlopen（）函数以指定模式打开指定的动态连接库文件，并返回一个句柄给调用进程。使用dlclose（）来卸载打开的库。

mode是打开方式，其值有多个，不同操作系统上实现的功能有所不同，在linux下，按功能可分为三类：
1）解析方式
RTLD_LAZY：在dlopen返回前，对于动态库中的未定义的符号不执行解析（只对函数引用有效，对于变量引用总是立即解析）。
RTLD_NOW：需要在dlopen返回前，解析出所有未定义符号，如果解析不出来，在dlopen会返回NULL，错误为：: undefined symbol: xxxx.......

2）作用范围，可与解析方式通过“|”组合使用
RTLD_GLOBAL：动态库中定义的符号可被其后打开的其它库重定位。
RTLD_LOCAL：与RTLD_GLOBAL作用相反，动态库中定义的符号不能被其后打开的其它库重定位。如果没有指明是RTLD_GLOBAL还是RTLD_LOCAL，则缺省为RTLD_LOCAL。


3）作用方式
RTLD_NODELETE：在dlclose()期间不卸载库，并且在以后使用dlopen()重新加载库时不初始化库中的静态变量。这个flag不是POSIX-2001标准。
RTLD_NOLOAD：不加载库。可用于测试库是否已加载(dlopen()返回NULL说明未加载，否则说明已加载），也可用于改变已加载库的flag，如：先前加载库的flag为RTLD_LOCAL，用dlopen(RTLD_NOLOAD|RTLD_GLOBAL)后flag将变成RTLD_GLOBAL。这个flag不是POSIX-2001标准。
RTLD_DEEPBIND：在搜索全局符号前先搜索库内的符号，避免同名符号的冲突。这个flag不是POSIX-2001标准。
```

### 内存泄漏调试

```
sudo apt install valgrind
valgrind --tool=memcheck --leak-check=full --show-reachable=yes 后接程序
程序编译时 加-g  调试状态
```

### strtok

分割字符串，比如“a-b-c-d”  按“-”分割后 结果 a b c d

```
    int bufsize = 64
    char **tokens = malloc(bufsize * sizeof(char *));
    token = strtok("a-b-c-d", "-");
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += TOK_BUFSIZE;
            tokens_backup = tokens;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens) {
                free(tokens_backup);
                fprintf(stderr, "my_parse: allocation error\n");
                return -1;
            }
        }
        token = strtok(NULL, TOK_DELIM);
    }
    tokens // a b c d
```

### 消息队列

1. #### msg

   不支持select  IO多路复用

   ```
   //头文件
   #include <sys/msg.h>
   #include <stdio.h>
   #include <stdlib.h>
   #include <string.h>
   #include <sys/msg.h>
   #include <errno.h>
   #include <sys/select.h>
   #include <unistd.h>
   
   
   //结构体
   struct msg_st
   {
       long int msg_type; //key_t id
       char text[MAX_TEXT]; // 队列消息
   };
   
   //建立消息队列
   msgid = msgget((key_t)11, 0666 | IPC_CREAT);
   if (msgid == -1)
   {
   fprintf(stderr, "msgget failed error: %d\n", errno);
   exit(EXIT_FAILURE);
   }
   
   data.msg_type = 1; 
   strcpy(data.text, buffer);
   
   // 向队列里发送数据
   if (msgsnd(msgid, (void *)&data, BUFSIZE, 0) == -1)
   {
   fprintf(stderr, "msgsnd failed\n");
   exit(EXIT_FAILURE);
   }
   
   //从队列里取出消息
   if (msgrcv(msgid, (void *)&data, BUFSIZE, 0, 0) == -1)
   {
   fprintf(stderr, "msgrcv failed width erro: %d", errno);
   }
   ```

   2. #### POSIX 消息队列

      支持select

      注意：gcc 需要加-lrt 库

      ```
      //SERVER.c
      #include <stdio.h>
      #include <stdlib.h>
      #include <mqueue.h>
      #include <sys/select.h>
      #define QUEUE_NAME  "/my_queue" //路径名字可以任意
      #define MAX_MSG_SIZE 4096
      
      int main(void) {
          mqd_t mq;  //接收返回消息队列描述符
          fd_set rd; //select 使用的描述符集合
          char buf[MAX_MSG_SIZE + 1];
          int prio;  //消息队列 优先级
          struct mq_attr attr;  //mq的属性
      
          attr.mq_flags = 0; 
          attr.mq_maxmsg = 10; //最大消息数
          attr.mq_msgsize = MAX_MSG_SIZE;  //消息的最大长度
          attr.mq_curmsgs = 0; //当前排队的消息数
      
          mq = mq_open(QUEUE_NAME, O_RDONLY | O_CREAT, 0644, &attr);
          if (mq == (mqd_t)-1) {
              perror("mq_open");
              exit(EXIT_FAILURE);
          }
      
          ssize_t bytes_read; //接收消息的长度
          while (1) {
              FD_ZERO(&rd);
              FD_SET(mq, &rd);
              if (select(mq + 1, &rd, NULL, NULL, NULL) < 0){ //会在这里阻塞，如果mq有读写消息，则读取
                  perror("select");
              }
              if(FD_ISSET(mq, &rd)){
                  if((bytes_read = mq_receive(mq, buf, 1, &prio)) > 0){
                      buf[bytes_read] = '\0'; //添加结束符
                      printf("Received message '%s' with priority %d %d\n", buf, prio, bytes_read);
                  }
              }
          }
      
          if (bytes_read == -1) {
              perror("mq_receive");
              exit(EXIT_FAILURE);
          }
      	mq_unlink(mq); //清空消息队列
          mq_close(mq);
          return 0;
      }
      ```

      ```
      // client.c
      #include <stdio.h>
      #include <stdlib.h>
      #include <mqueue.h>
      
      #define QUEUE_NAME  "/my_queue"
      #define MAX_MSG_SIZE 4096
      
      int main(void) {
          mqd_t mq;
          char msg[MAX_MSG_SIZE];
          unsigned int prio = 1;
      
          mq = mq_open(QUEUE_NAME, O_WRONLY);
          if (mq == (mqd_t)-1) {
              perror("mq_open");
              exit(EXIT_FAILURE);
          }
          snprintf(msg, MAX_MSG_SIZE, "Hello%d", getpid());
          if (mq_send(mq, msg, strlen(msg), prio) == -1) {
              perror("mq_send");
              exit(EXIT_FAILURE);
          }
      
      	mq_unlink(mq); //清空消息队列
          mq_close(mq);
          return 0;
      }
      
      ```


### 键盘记录

读取/dev/input/event 文件

```

```

### 忽略broken pipe Signals信号

```
void sighandler()
{
	....
}
signal(SIGPIPE, sighandler); //异常处理函数sighandler()
signal(SIGPIPE, SIG_IGN	);//忽略信号

```

### __attribute__

```
GCC 特有的属性 "**attribute**"，以指定该函数应在主函数执行之前自动调用。
定义了一个构造函数，在程序启动时会被调用。
"attribute ((constructor))" 语法是特定于 GCC 编译器的，可能不受其他编译器或平台的支持。

```

### prctl   

```
修改进程名称    prctl(PR_SET_NAME, "myexcel", 0, 0, 0);
```

### getpeername    从socket 取出建立连接的ip信息

```
		struct sockaddr_in peer_addr;
		socklen_t peer_len = sizeof(peer_addr);
		int ret = getpeername(client, (struct sockaddr *)&peer_addr, &peer_len); // zys 20230228 从client socket描述符取出地址信息
		if (ret == -1) {
			perror("getpeername");
			continue;
		}
```

### 根据socket 提取地址信息

```
		struct sockaddr_in peer_addr;
		socklen_t peer_len = sizeof(peer_addr);
		int ret = getpeername(client, (struct sockaddr *)&peer_addr, &peer_len); // zys 20230228 从client socket描述符取出地址信息
		if (ret == -1) {
			p_debug("getpeername error");
			continue;
		}
```

