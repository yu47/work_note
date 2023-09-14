### windows 设备管控

* Server 项目编译出 ssl.dll or tcp.dll 文件。 作用：被控端的功能。
* SetBootMode 项目编译出 ssl_setup.exe or tcp_setup.exe 文件。作用：加载.dll 文件，安装服务。
* 用maker中的bin文件夹的run,bat  把上述的dll和exe文件转化为头文件。*.h（每个字节的16进制，数组包含的形式）。
* CreateServer把上述的*.h 头文件加载到项目，再写根据用的填写的配置，修改.h 里的写死的配置。生成被控端exe和tmp。（ExeTo.h.exe 转化为.h 文件的工具。）<img src="..\images\Snipaste_2023-07-07_15-17-49.png" style="zoom:80%;" />
* 

#### node 服务端节点

* rabbitmq + mebedtls
* 控制端发出指令→node→bot的流程：BotInfo.cpp 192和226行 用ReadPacket阻塞接收控制端的指令 ↓ DataProcess.cpp 158 行  FwdTobot函数 ↓ BotInfo 808行 SendToBot函数 ↓  TCPSocket.cpp 43行send函数 发送指令给bot
* bot执行结果发出→node→控制端的流程：Main.cpp 51 RecvFormBot ↓ BotInfo.cpp ReadPacket函数阻塞 → 接收数据之后 155行 RecvBotPacket 函数解析 发送到控制端 BotInfo.cpp 850 WritePacket.
* web端 通过消息队列进行通信（json的格式）。

