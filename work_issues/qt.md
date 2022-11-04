#### QT编译出现 not found libxkbcommon-x11 

```
sudo apt-get install xcb libxcb-xkb-dev x11-xkb-utils libx11-xcb-dev libxkbcommon-x11-dev libxkbcommon-dev
```

QT在linux下，用Linuxdeployqt打包程序

##### 1. 下载linuxdeployqt

linuxdeployqt-x86_64.AppImage，下载地址：

```
https://github.com/probonopd/linuxdeployqt/releases
```

2. ##### 安装linuxdeployqt

```
mv linuxdeployqt-6-x86_64.AppImage linuxdeployqt
```



##### 然后修改为可执行权限

```
chmod 777 linuxdeployqt
再把这个可执行文件扔进/usr/local/bin目录，这样，我们就可以在其他目录访问了
mv linuxdeployqt /usr/local/bin
```



##### 测试是否安装成功

```
linuxdeployqt --version
```

##### 添加环境变量

```
export PATH=/opt/Qt5.13.0/5.13.0/gcc_64/bin:$PATH
export LIB_PATH=/opt/Qt5.13.0/5.13.0/gcc_64/lib:$LIB_PATH
export PLUGIN_PATH=/opt/Qt5.13.0/5.13.0/gcc_64/plugins:$PLUGIN_PATH
export QML2_PATH=/opt/Qt5.13.0/5.13.0/gcc_64/qml:$QML2_PATH

```

然后，对环境变量测试下，是否设置正确。如下：

```cpp
qmake -v
```

使用命令执行依赖文件拷贝

```powershell
linuxdeployqt Test -appimage
```

忽略新版本

```powershell
linuxdeployqt Test -appimage -unsupported-allow-new-glibc

linuxdeployqt Test -appimage -unsupported-bundle-everything
```



### QT中QString 转 unsigned char*

```C++
QString hideFilePaths = "test";
unsigned char* buf = (unsigned char*)hideFilePaths.toLatin1().data();
```
### QT中QString 转 char*

```C++
QString hideFilePaths = "test";
char* buf = (char*)hideFilePaths.toStdString().c_str();
```

QT弹出终端

```c++
system("dbus-launch gnome-terminal --window -- bash -c './Sophie cmd 'bin/sh';exec bash'");
//ubuntu18以上
system("gnome-terminal --window -- bash -c './Sophie cmd 'bin/sh';exec bash'");
```

Qprocess

```c++
QProcess *p = new Qprocess(0);  
p.start("bash", "ls");  
p.waitForStarted();  
p.waitForFinished();  
qDebug()<<QString::fromLocal8Bit(p.readAllStandardOutput()); 
```

### 文件操作

```C++
   	QFile configFile(configFilePath);//文件路径
    if(configFile.open(QIODevice::ReadOnly|QIODevice::Text)){//只读打开
        QByteArray data = configFile.readAll();
        data.replace("[General]","");
        configFile.close();
        configFile.open(QIODevice::WriteOnly|QIODevice::Text);//只写打开
        configFile.write(data);
    }
```

| QIODevice::ReadOnly  | 只能对文件进行读操作                                         |
| -------------------- | ------------------------------------------------------------ |
| QIODevice::WriteOnly | 只能对文件进行写操作，如果目标文件不存在，会自行创建一个新文件。 |
| QIODevice::ReadWrite | 等价于 ReadOnly \| WriteOnly，能对文件进行读和写操作。       |
| QIODevice::Append    | 以追加模式打开文件，写入的数据会追加到文件的末尾（文件原有的内容保留）。 |
| QIODevice::Truncate  | 以重写模式打开，写入的数据会将原有数据全部清除。注意，此打开方式不能单独使用，通常会和 ReadOnly 或 WriteOnly 搭配。 |
| QIODevice::Text      | 读取文件时，会将行尾结束符（Unix 系统中是 "\n"，Windows 系统中是 "\r\n"）转换成‘\n’；将数据写入文件时，会将行尾结束符转换成本地格式，例如 Win32 平台上是‘\r\n’。 |

| qint64 QFile::size() const                                | 获取当前文件的大小。对于打开的文件，该方法返回文件中可以读取的字节数。 |
| --------------------------------------------------------- | ------------------------------------------------------------ |
| bool QIODevice::getChar(char *c)                          | 从文件中读取一个字符，并存储到 c 中。读取成功时，方法返回 true，否则返回 false。 |
| bool QIODevice::putChar(char c)                           | 向文件中写入字符 c，成功时返回 true，否则返回 false。        |
| QByteArray QIODevice::read(qint64 maxSize)                | 从文件中一次性最多读取 maxSize 个字节，然后返回读取到的字节。 |
| qint64 QIODevice::read(char *data, qint64 maxSize)        | 从文件中一次性对多读取 maxSize 个字节，读取到的字节存储到 data 指针指定的内存控件中。该方法返回成功读取到的字节数。 |
| QByteArray QIODevice::readAll()                           | 读取文件中所有的数据。                                       |
| qint64 QIODevice::readLine(char *data, qint64 maxSize)    | 每次从文件中读取一行数据或者读取最多 maxSize-1 个字节，存储到 data 中。该方法返回实际读取到的字节数。 |
| qint64 QIODevice::write(const char *data, qint64 maxSize) | 向 data 数据一次性最多写入 maxSize 个字节，该方法返回实际写入的字节数。 |
| qint64 QIODevice::write(const char *data)                 | 将 data 数据写入文件，该方法返回实际写入的字节数。           |
| qint64 QIODevice::write(const QByteArray &byteArray)      | 将 byteArray 数组中存储的字节写入文件，返回实际写入的字节数。 |
| bool QFile::copy(const QString &newName)                  | 将当前文件的内容拷贝到名为 newName 的文件中，如果成功，方法返回 true，否则返回 false。  copy 方法在执行复制操作之前，会关闭源文件。 |
| bool QFile::rename(const QString &newName)                | 对当前文件进行重命名，新名称为 newName，成功返回 true，失败返回 false。 |
| bool QFile::remove()                                      | 删除当前文件，成功返回 true，失败返回 false。                |

### QT 开发文件浏览器

```C++
QString localDirPath = QFileDialog::getExistingDirectory();
QFileDialog.getExistingDirectory()//打开文件夹
QFileDialog.getOpenFileName()    //获取一个打开文件的文件名
QFileDialog.getOpenFileNames()   //获取多个打开文件的文件名
QFileDialog.getOpenFileUrl()     //获取一个打开文件的统一资源定位符
QFileDialog.getOpenFileUrls()    //获取多个打开文件的统一资源定位符
QFileDialog.getSaveFileName()    //获取保存的文件名
QFileDialog.getSaveFileUrl()     //获取保存的url
```



### 随机数

```C++
头文件#include <QRandomGenerator>
int AUTH = QRandomGenerator::global()->bounded(268435456,929496729) //随机数
//转化16进制
QString AUTH= QString::number(QRandomGenerator::global()->bounded(268435456,929496729),16);
```

### 树组件tablewidget拖拽

```
void dropEvent(QDropEvent *event) override;
重写dropEvent函数
写出对应操作
```

### 分割数据split()

```
QString test = "11111111/22222/3333";
QStringList list = test.split("/")
int num = list.count()
```

