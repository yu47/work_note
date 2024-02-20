1. 被控端轮询上线。√
2. 端口扫描。√
3. 进程名自定义。√
4. 远程更新，卸载。 √
5. 日志显示  控，被上下线  时间。 （加密文件 ）  C/S uuid  ip 上线  时间    C/S  uuid  ip 下线 时间。 √ 
6. 保活机制。  10分钟
7. 文件增加全量获取。√  属性
8. 激活包增加udp协议。   √
9. 控制端连接多服务器端。
10. 控制程序大小。 
11. 正反模式可以同时开启。  √
12. mips 自动选择sh。 √
13. 源码说明。

```

```



控制端  中转  token + 协议 +  p_active_header

中转 转发 被控端  加密p_active_header

被控端  验证token 解密p_active_header



1. disconnect 控制端没下线   
2. 控制端加服务端状态 √
3. 分组错误 √
4. sock5 不能加多个  √
5. 激活选择一台服务端激活   √
6. 配置端添加默认进程名。 √







源码说明：

insert_to_route_set（）  插入到glvar.route_set 



```
glvar.route_set
```

![image-20230925163701825](C:\Users\admin\AppData\Roaming\Typora\typora-user-images\image-20230925163701825.png)

设备表更新

