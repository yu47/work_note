## **来到内核世界**

编写一个Linux内核模块并不是一件容易的事情。在修改内核的时候，您将面临数据丢失和系统损坏的风险。对于常规Linux应用程序来说，系统为它们提供了相应的安全网作为保护，但是内核代码却完全不是这样：内核代码一旦出现故障，将会锁定整个系统。

更糟糕的是，内核代码中出现的问题可能不会马上显现出来。如果内核模块加载后，系统立即被锁定的话，这还算是“最佳情况”。随着向模块添加的代码越来越多，我们将面临引入死循环和内存泄漏的风险。如果你不小心犯了这样的错误，随着机器的继续运行，这些代码占用的内存会持续增长。那么，最终会导致重要的内存结构，甚至缓冲区都被覆盖掉。

对于内核模块来说，传统应用程序的大部分开发范式都不适用。除了加载和卸载模块外，我们还需编写代码来响应系统事件，因为这里的代码并非以串行的模式运行。对于内核开发来说，我们要编写的是供应用程序使用的API，而非应用程序本身。

除此之外，我们在内核空间也无法访问各种标准库。虽然内核提供了一些常用的函数，比如printk（用作printf的替代品）和kmalloc（作用与malloc类似），但是大部分情况下，都需要我们亲自跟设备打交道。此外，在卸载模块时，我们必须亲自完成相关的清理工作，因为这里没有提供垃圾收集功能。

## **先决条件**

在开始编写内核模块之前，我们需要确保已经准备好了得心应手的工具。最重要的是，你需要有一台Linux机器。虽然任何Linux发行版都可以满足我们的要求，但是在本文中，我使用的是Ubuntu 16.04 LTS，所以，如果你使用了其他版本的话，在安装的过程中，可能需要稍微调整一下相关的安装命令。

其次，你还需要一台单独的物理机器或虚拟机。虽然我更喜欢在虚拟机上完成这些工作，但是读者完全可以根据自己的喜好来作出决定。我不建议使用您的工作主机，因为一旦出错，就很可能会发生数据丢失的情况。同时，我们在编写内核模块的过程中，一般至少会锁定机器许多次，这个是不用怀疑的。内核出乱子的时候，最近更新的代码很可能还在向缓冲区中写入内容，所以，这就可能导致源文件损坏。如果在虚拟机上进行测试的话，就能够消除这种风险。

最后，您至少需要对C语言有一些基本的了解。由于C++运行时对于内核来说占用的空间太多了，因此，编写C代码对于内核开发来说是非常重要的。此外，为了与硬件进行交互，了解一些汇编语言方面的知识也是非常有帮助的。

## **安装开发环境**

在Ubuntu上，我们需要运行下列命令：

```text
apt-get install build-essential linux-headers-`uname -r`
```

上面的命令将安装必要的开发工具，以及这个示例内核模块所需的内核头文件。

对于下面的示例内核模块，我们假设读者是以普通用户身份运行的，而不是root用户，但是，要求读者拥有sudo权限。对于非root用户来说，sudo在加载内核模块时是必须的，尽管这样有些麻烦，但我们希望尽可能以root之外的身份来完成内核模块开发工作。

## **踏上征程**

从现在开始，我们就要开始编写代码了。好了，让我们先准备好工作环境：

```text
mkdir ~/src/lkm_example
cd ~/src/lkm_example
```

你可以启动自己最喜欢的编辑器（对于我来说，就是VIM），创建文件lkm_example.c，并输入以下代码：

```text
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
MODULE_LICENSE(“GPL”);
MODULE_AUTHOR(“Robert W. Oliver II”);
MODULE_DESCRIPTION(“A simple example Linux module.”);
MODULE_VERSION(“0.01”);
static int __init lkm_example_init(void) {
 printk(KERN_INFO “Hello, World!\n”);
 return 0;
}
static void __exit lkm_example_exit(void) {
 printk(KERN_INFO “Goodbye, World!\n”);
}
module_init(lkm_example_init);
module_exit(lkm_example_exit);
```

现在，我们已经做好了一个最简单的内核模块，接下来，我会对一些重点内容加以详细说明：

·“includes”用于包含Linux内核开发所需的头文件。

· 根据模块的许可证的不同，MODULE_LICENSE可以设置为不同的值。要查看许可证的完整列表，请运行：

```text
grep“MODULE_LICENSE”-B 27 / usr / src / linux-headers -`uname -r` / include / linux / module.h
```

· 我们将init（加载）和exit（卸载）函数定义为static类型，并让它返回一个int型数据。

· 注意，这里要使用printk函数，而不是printf函数。此外，printk与printf使用的参数也各不相同。例如，KERN_INFO（这是一个标志，用以声明相应的消息记录等级）在定义的时候并没有使用逗号。

· 在文件的最后部分，我们调用了module_init和module_exit函数，来告诉内核哪些是加载函数和卸载函数。这样的话，我们就能够给这些函数自由命名了。

当目前为止，我们仍然无法编译这个文件：我们还需要一个Makefile文件。有了它，这个简单的示例模块就算就绪了。请注意，make会严格区分空格和制表符，因此，在应该使用tab的地方千万不要使用空格。

```text
obj-m += lkm_example.o
all:
 make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
 make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```

如果我们运行“make”，正常情况下应该成功编译我们的模块。最后得到的文件是“lkm_example.ko”。如果在此过程中出现了错误消息的话，请检查示例源文件中的引号是否正确，并确保没有意外粘贴为UTF-8字符。

现在，可以将我们的模块插入内核空间进行测试了。为此，我们可以运行如下所示的命令：

```text
sudo insmod lkm_example.ko
```

如果一切顺利的话，屏幕上面是不会显示任何内容的。这是因为，printk函数不会将运行结果输出到控制台，相反，它会把运行结果输出到内核日志。为了查看内核模块的运行结果，我们需要运行下列命令：

```text
sudo dmesg
```

正常情况下，这里应该看到带有时间戳前缀的“Hello，World！”行。这意味着我们的内核模块已经加载，并成功向内核日志输出了相关的字符串。我们还可以通过下面的命令，来检查该模块是否仍然处于加载状态：

```text
lsmod | grep “lkm_example”
```

要删除该模块，请运行下列命令：

```text
sudo rmmod lkm_example
```

如果您再次运行dmesg，则会在日志中看到字符串“Goodbye, World!”。同时，您也可以再次使用lsmod来确认它是否已被卸载。

正如你所看到的那样，这个测试工作流程有点繁琐而乏味，为了实现自动化，我们可以在Makefile文件末尾添加下列内容：

```text
test:
 sudo dmesg -C
 sudo insmod lkm_example.ko
 sudo rmmod lkm_example.ko
 dmesg
```

然后，运行下列命令：

```text
make test
```

这样的话，要想测试模块并查看内核日志的输出的话，就不必专门来运行相应的命令了。

现在，我们已经打造好了一个五脏俱全，但是没有什么用处的内核模块！

## **打造更有趣的内核模块**

接下来，让我们通过具体的例子来进一步了解内核模块的开发。虽然内核模块可以完成各种任务，但最常见的用途，恐怕就是与应用程序进行交互了。

由于应用程序无法直接查看内核空间内存的内容，因此，它们必须借助API与其进行通信。虽然从技术上来说有多种方法可以实现这一点，但最常见的方法却是创建一个设备文件。

实际上，您很可能早就跟设备文件打过交道了。比如，涉及/dev/zero、/dev/null或类似文件的命令，实际上就是在跟名为“zero”和“null”的设备进行交互，以返回相应的内容。

在我们的例子中，我们将返回“Hello，World”。虽然对于应用程序来说，这一功能没有多大的用途，但它却为我们详细展示了通过设备文件响应应用程序的具体过程。

下面是完整的代码：

```text
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
MODULE_LICENSE(“GPL”);
MODULE_AUTHOR(“Robert W. Oliver II”);
MODULE_DESCRIPTION(“A simple example Linux module.”);
MODULE_VERSION(“0.01”);
#define DEVICE_NAME “lkm_example”
#define EXAMPLE_MSG “Hello, World!n”
#define MSG_BUFFER_LEN 15
/* Prototypes for device functions */
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
static int major_num;
static int device_open_count = 0;
static char msg_buffer[MSG_BUFFER_LEN];
static char *msg_ptr;
/* This structure points to all of the device functions */
static struct file_operations file_ops = {
 .read = device_read,
 .write = device_write,
 .open = device_open,
 .release = device_release
};
/* When a process reads from our device, this gets called. */
static ssize_t device_read(struct file *flip, char *buffer, size_t len, loff_t *offset) {
 int bytes_read = 0;
 /* If we’re at the end, loop back to the beginning */
 if (*msg_ptr == 0) {
 msg_ptr = msg_buffer;
 }
 /* Put data in the buffer */
 while (len && *msg_ptr) {
 /* Buffer is in user data, not kernel, so you can’t just reference
 * with a pointer. The function put_user handles this for us */
 put_user(*(msg_ptr++), buffer++);
 len--;
 bytes_read++;
 }
 return bytes_read;
}
/* Called when a process tries to write to our device */
static ssize_t device_write(struct file *flip, const char *buffer, size_t len, loff_t *offset) {
 /* This is a read-only device */
 printk(KERN_ALERT “This operation is not supported.\n”);
 return -EINVAL;
}
/* Called when a process opens our device */
static int device_open(struct inode *inode, struct file *file) {
 /* If device is open, return busy */
 if (device_open_count) {
 return -EBUSY;
 }
 device_open_count++;
 try_module_get(THIS_MODULE);
 return 0;
}
/* Called when a process closes our device */
static int device_release(struct inode *inode, struct file *file) {
 /* Decrement the open counter and usage count. Without this, the module would not unload. */
 device_open_count--;
 module_put(THIS_MODULE);
 return 0;
}
static int __init lkm_example_init(void) {
 /* Fill buffer with our message */
 strncpy(msg_buffer, EXAMPLE_MSG, MSG_BUFFER_LEN);
 /* Set the msg_ptr to the buffer */
 msg_ptr = msg_buffer;
 /* Try to register character device */
 major_num = register_chrdev(0, “lkm_example”, &file_ops);
 if (major_num < 0) {
 printk(KERN_ALERT “Could not register device: %d\n”, major_num);
 return major_num;
 } else {
 printk(KERN_INFO “lkm_example module loaded with device major number %d\n”, major_num);
 return 0;
 }
}
static void __exit lkm_example_exit(void) {
 /* Remember — we have to clean up after ourselves. Unregister the character device. */
 unregister_chrdev(major_num, DEVICE_NAME);
 printk(KERN_INFO “Goodbye, World!\n”);
}
/* Register module functions */
module_init(lkm_example_init);
module_exit(lkm_example_exit);
```

## **测试加强版的示例代码**

现在，我们的示例代码已经不仅限于在加载和卸载过程中输出相应的消息了，所以，我们需要一个限制性较小的测试例程。接下来，让我们修改Makefile，让它只加载模块，而不进行卸载。

```text
obj-m += lkm_example.o
all:
  make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
  make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
test:
  # We put a — in front of the rmmod command to tell make to ignore
  # an error in case the module isn’t loaded.
  -sudo rmmod lkm_example
  # Clear the kernel log without echo
  sudo dmesg -C
  # Insert the module
  sudo insmod lkm_example.ko
  # Display the kernel log
  dmesg
```

现在，运行“make test”的时候，该示例代码将会输出设备的主编号。在本例中，这个编号是由内核自动分配的。但是，您后面会用到这个值来创建设备。

接下来，我们需要利用运行“make test”后得到的值，创建一个设备文件，以便从用户空间与内核模块进行通信。

```text
sudo mknod /dev/lkm_example c MAJOR 0
```

（在上面的例子中，请用运行“make test”或“dmesg”时获得的值来替换MAJOR）

mknod命令中的“c”的作用是，告诉mknod我们要创建一个字符设备文件。

现在，我们就可以通过该设备来获取相应的内容了：

```text
cat /dev/lkm_example
```

此外，您也可以通过应用程序来访问该设备。当然，这些应用不要求一定是编译型的程序——即使Python、Ruby和PHP这样的脚本程序，也照样可以访问这些数据。

当我们使用完这些设备时，可以将其删除，并卸载相应的内核模块：

```text
sudo rm /dev/lkm_example
sudo rmmod lkm_example
```



编写write（）

```C++
static ssize_t device_write(struct file *flip, const char *buffer, size_t len, loff_t *offset) {
	/* This is a read-only device */
	printk("write");
	char* test = (char*)kmalloc(len,GFP_KERNEL);

	copy_from_user(test,buffer,len);
	printk("111111,%s",test);

 	// printk(KERN_ALERT "This operation is not supported.\n");
 	return -EINVAL;
}
```

ioctl（）实现

```C++
static long device_ioctl( struct file *, unsigned int, unsigned long);
static struct file_operations file_ops = {
 .read = device_read,
 .write = device_write,
 .open = device_open,
 .release = device_release,
 .unlocked_ioctl = device_ioctl,
};
static long device_ioctl( struct file *file, unsigned int cmd, unsigned long arg){

	int err = 0;
    int ret = 0;
    int ioarg = 0;
    
    // /* 检测命令的有效性 */
    // if (_IOC_TYPE(cmd) != MEMDEV_IOC_MAGIC) 
    //     return -EINVAL;
    // if (_IOC_NR(cmd) > MEMDEV_IOC_MAXNR) 
    //     return -EINVAL;

    /* 根据命令类型，检测参数空间是否可以访问 */
    // if (_IOC_DIR(cmd) & _IOC_READ)
    //     err = !access_ok(VERIFY_WRITE, (void *)arg, _IOC_SIZE(cmd));
    // else if (_IOC_DIR(cmd) & _IOC_WRITE)
    //     err = !access_ok(VERIFY_READ, (void *)arg, _IOC_SIZE(cmd));
    // if (err) 
    //     return -EFAULT;

    /* 根据命令，执行相应的操作 */
    switch(cmd) {

      /* 打印当前设备信息 */
      case MEMDEV_IOCPRINT:
          printk("<--- CMD MEMDEV_IOCPRINT Done--->\n\n");
        break;
      
      /* 获取参数 */
      case MEMDEV_IOCGETDATA: 
        ioarg = 222032;
        ret = __put_user(ioarg, (int *)arg);
		printk("arg is %llx", arg);
		// ret = copy_from_user(ioarg,(int *)arg,sizeof(ioarg));
        break;
      
      /* 设置参数 */
      case MEMDEV_IOCSETDATA: 
        ret = __get_user(ioarg, (int *)arg);
        printk("<--- In Kernel MEMDEV_IOCSETDATA ioarg = %d --->\n\n",ioarg);
        break;

      default:  
        return -EINVAL;
    }
    return ret;
}
```

#### 应用程序（app_lkm.c）

```C++
#include <stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
/* 定义幻数 */
#define MEMDEV_IOC_MAGIC  'k'

/* 定义命令 */
#define MEMDEV_IOCPRINT   _IO(MEMDEV_IOC_MAGIC, 1)
#define MEMDEV_IOCGETDATA _IOR(MEMDEV_IOC_MAGIC, 2, int)
#define MEMDEV_IOCSETDATA _IOW(MEMDEV_IOC_MAGIC, 3, int)

int main()
{
    int fd = 0;
    int cmd;
    int arg = 0;
    char Buf[4096];
    
    
    /*打开设备文件*/
    fd = open("/dev/lkm_example",O_RDWR);
    if (fd < 0)
    {
        printf("Open Dev Mem0 Error!\n");
        return -1;
    }
    
    /* 调用命令MEMDEV_IOCPRINT */
    printf("<--- Call MEMDEV_IOCPRINT --->\n");
    cmd = MEMDEV_IOCPRINT;
    if (ioctl(fd, cmd, &arg) < 0)
        {
            printf("Call cmd MEMDEV_IOCPRINT fail\n");
            return -1;
    }
    
    
    /* 调用命令MEMDEV_IOCSETDATA */
    printf("<--- Call MEMDEV_IOCSETDATA --->\n");
    cmd = MEMDEV_IOCSETDATA;
    arg = 2007;
    if (ioctl(fd, cmd, &arg) < 0)
        {
            printf("Call cmd MEMDEV_IOCSETDATA fail\n");
            return -1;
    }

    
    /* 调用命令MEMDEV_IOCGETDATA */
    printf("<--- Call MEMDEV_IOCGETDATA --->\n");
    cmd = MEMDEV_IOCGETDATA;
    printf("arg is %llx", &arg);
    if (ioctl(fd, cmd, &arg) < 0)
        {
            printf("Call cmd MEMDEV_IOCGETDATA fail\n");
            return -1;
    }
    printf("<--- In User Space MEMDEV_IOCGETDATA Get Data is %d --->\n\n",arg);    
    
    close(fd);
    return 0;    
}
```

链接：https://www.cnblogs.com/sctb/p/13816110.html

链接：https://www.cnblogs.com/geneil/archive/2011/12/04/2275372.html