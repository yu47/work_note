## 1、 安装kernel image、dbsym、source code

```
1.搜索要下载的linux内核版本 apt-cache search linux-image | grep linux-image-4.10.0 |grep generic
2.安装内核 sudo apt-get install linux-image-4.10.0-19-generic
3.查看已安装的内核版本 sudo dpkg --list | grep linux-image
```

##### 安装dbgsym符号文件

增加符号文件对应的source.list ,更新源文件。

```
# 增加source.list
codename=$(lsb_release -c | awk '{print $2}')
sudo tee /etc/apt/sources.list.d/ddebs.list << EOF
deb http://ddebs.ubuntu.com/ ${codename} main restricted universe multiverse
deb http://ddebs.ubuntu.com/ ${codename}-security main restricted universe multiverse
deb http://ddebs.ubuntu.com/ ${codename}-updates main restricted universe multiverse
deb http://ddebs.ubuntu.com/ ${codename}-proposed main restricted universe multiverse
EOF
# 添加访问符号服务器的秘钥文件
wget -O - http://ddebs.ubuntu.com/dbgsym-release-key.asc | sudo apt-key add -
# 更新源文件
sudo apt-get update
```

通过apt-get下载dbgsym，默认保存在/usr/lib/debug/boot中，文件名是vmlinux-4.10.0-19-generic。

```
sudo apt-get install linux-image-`uname -r`-dbgsym
```

（可选）另一种搜索当前内核的 dbgsym的方法：

```
sudo apt-get install aptitude
sudo aptitude search 'linux-image-'$(uname -r)'-dbgsym'
```

##### 获取kernel对应的源码

一、apt安装
打开/etc/apt/sources.list，启用deb-src，执行sudo apt-get update更新源文件：

- 搜索所有的source code：`apt-cache search linux-source`
- 安装指定版本的source code： `sudo apt-get install linux-source-4.10.0`

### 搭建双机调试环境

两者通过串口借助物理机（linux）的/tmp/serial 通信，如果是windows需要设置为//./pipe/com_1。

。配置debugging 的串口为server，debuggee的串口为client

![image-20221017205110803](C:\Users\admin\AppData\Roaming\Typora\typora-user-images\image-20221017205110803.png)

#### 配置debuggee

需要让debuggee在开机时进入KGDB的被调试状态，首先需要修改grub文件，增加grub引导时的菜单项（menuentry）。

**配置debuggee**
需要让debuggee在开机时进入KGDB的被调试状态，首先需要修改grub文件，增加grub引导时的菜单项（menuentry）。

```
sudo vim /etv/grub.d/40_custom
```

修改的内容可以从/boot/grub/grub.cfg中复制。在菜单名中增加调试信息，并在内核命令行中增加KGDB选项。

```
#!/bin/sh
exec tail -n +3 $0
# This file provides an easy way to add custom menu entries.  Simply type the
# menu entries you want to add after this comment.  Be careful not to change
# the 'exec tail' line above.
menuentry 'Ubuntu, KGDB with nokaslr' --class ubuntu --class gnu-linux --class gnu --class os $menuentry_id_option 'gnulinux-simple-bf306d0a-28c8-49c6-bffc-446be272ddcf' {
    recordfail
    load_video
    gfxmode $linux_gfx_mode
    insmod gzio
    if [ x$grub_platform = xxen ]; then insmod xzio; insmod lzopio; fi
    insmod part_msdos
    insmod ext2
    set root='hd0,msdos1'
    if [ x$feature_platform_search_hint = xy ]; then
      search --no-floppy --fs-uuid --set=root --hint-bios=hd0,msdos1 --hint-efi=hd0,msdos1 --hint-baremetal=ahci0,msdos1  bf306d0a-28c8-49c6-bffc-446be272ddcf
    else
      search --no-floppy --fs-uuid --set=root bf306d0a-28c8-49c6-bffc-446be272ddcf
    fi
    echo 'Loading Linux 4.10.0-19 with KGDB built by GEDU lab...'
    linux /boot/vmlinuz-4.10.0-19-generic root=UUID=bf306d0a-28c8-49c6-bffc-446be272ddcf ro find_preseed=/preseed.cfg auto noprompt priority=critical locale=en_US quiet kgdbwait kgdb8250=io,03f8,ttyS0,115200,4 kgdboc=ttyS0,115200 kgdbcon nokaslr
    echo 'Loading initial ramdisk ...'
    initrd    /boot/initrd.img-4.10.0-19-generic
}
```

```
quiet kgdbwait kgdb8250=io,03f8,ttyS0,115200,4 kgdboc=ttyS0,115200 kgdbcon nokaslr
加上这一段
```

更新grub后，重启按住SHIFT 进入grub选择刚才的menu即可等到进入被调试模式。

```
quiet kgdbwait kgdb8250=io,03f8,ttyS0,115200,4 kgdboc=ttyS0,115200 kgdbcon nokaslr
```

#### 配置debugging

1. 设置串口通信波特率：

   ```
   	stty -F /dev/ttyS0 115200
   ```

   

2. 编写gdb config，在vmlinux时，可以通过source加载。

   ```
   set architecture i386:x86-64:intel
   target remote /dev/ttyS0
   ```

3. 使用gdb来调试带符号的vmlinux，这时gdb就会尝试使用串口/dev/ttyS0来调试。

   ```
   gdb -s /usr/lib/debug/boot/vmlinux-4.10.0-19-generic
   gdb> source config
   ```

   