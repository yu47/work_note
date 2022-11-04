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

oiat

int 转 char

###  strtok

```
分解字符串： strtok(NULL, delim)
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
lseek(fd,0,SEEK_CUD); 光标移动到看根据当前光标前移
```

### fork调试子线程 ：  -exec set follow-fork-mode child

### C语言execlp()函数：

```
从PATH 环境变量中查找文件并执行
execl("/bin/ls", "ls", "-al", "/etc/passwd", (char *)0);

```

### ftruncate(fd,0);  清空数据