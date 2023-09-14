#### QT编译出现 not found libxkbcommon-x11 

```
sudo apt-get install xcb libxcb-xkb-dev x11-xkb-utils libx11-xcb-dev libxkbcommon-x11-dev libxkbcommon-dev
出现 readline未找到， libreadline-dev
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
linuxdeployqt  -appimage Test
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

### QT中unsigned char * 转 QString

```
unsigned char *buf = "test";
QString(QLatin1String((char*)buf));
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

### QtableWidget 排序的时候 ，改变数据要

```
ui->filesOperHistoryTableWidget->setSortingEnabled(false);
```

QT表格

```c++
//列
typedef enum _SsANOCol {
    SsANOResultRecordColNumNetid,
} SsANOCol;

//结构体
typedef struct _SsANOResultRecord { 
    QString netid;
} SsANOResultRecord, *PSsANOResultRecord;

//内容填充
int row = ui->netstatTableWidget_2->rowCount();// 自动增加行
ui->netstatTableWidget_2->insertRow(row);
QTableWidgetItem *SsnetidItem = new QTableWidgetItem("111111111");//单元显示的内容
SsnetidItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
ui->netstatTableWidget_2->setItem(row, SsANOResultRecordColNumNetid, SsnetidItem);
```

### 时间

```C++
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss ddd");
    QString message = text.append(msg).append("(").append(current_date).append(")");

```

### 表格最后一列扩充

```
 //设置行列均分
tableWidget->horizontalHeader()->setStretchLastSection(true); //就是这个地方
 tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
 //使行列头自适应宽度，最后一列将会填充空白部分
table->horizontalHeader()->setStretchLastSection(true);          

//使行列头自适应宽度，所有列平均分来填充空白部分            
table->horizontalHeader()->setResizeMode(QHeaderView::Strtch);       

//使行自适应高度，假如行很多的话，行的高度不会一直减小，当达到一定值时会自动生成一个QScrollBar
table->verticalHeader()->setResizeMode(QHeaderView::Strtch);

//隐藏左边的序号
QHeaderView* headerView = table的名字->verticalHeader();
      headerView->setHidden(true);

```

### 添加右键菜单

```C++
ui->netstatTableWidget->setContextMenuPolicy(Qt::CustomContextMenu);;
m_networkMenu = new QMenu(ui->netstatTableWidget);
    QAction *actHideaddress = new QAction(MENU_HIDE, this); 口
    m_networkMenu->addAction(actHideaddress);
    connect(actHideaddress, SIGNAL(triggered(bool)), this, SLOT(on_hideButton_clicked()));
connect(ui->netstatTableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slot_show_tabMenu(QPoint)));
```

### qt 当前路径

```
QString absPath = qApp->applicationDirPath();
```

### qt QtConcurrent::run([=] () {}

### 以root启动

    if(!geteuid() == 0)
        {
            QMessageBox::information(this, "错误", "请使用root用户启动。");
            exit(0);
        }
### 释放内存ui->setupUi(this);

```
setAttribute(Qt::WA_DeleteOnClose);
```

### 增加QtConcurrent 线程池数量

```
QThreadPool::globalInstance()->setMaxThreadCount(n);
```

### windows打包程序 ，用Release， 编译

```
打开 qt5.9... cmd
cd 程序目录
windeployqt KBN.exe
执行成功后，会把所需库移动进来
```

### 编译RCC  读取资源文件，并写入本地文件。

```
qre内容:
<RCC>
    <qresource prefix="/icon">
        <file>OIG.ico</file>
    </qresource>
    <qresource prefix="/json">
        <file>config.json</file>
    </qresource>
    <qresource prefix="/exe">
        <file>Aqc15.exe</file>
        <file>Aqc18.exe</file>
        <file>BIOS_info.exe</file>
        <file>Cx2.exe</file>
        <file>Cx3.exe</file>
    </qresource>
</RCC>

编译命令
rcc -binary res.qrc -o resources.rcc
```

```C++
    bool status = QResource::registerResource(qApp->applicationDirPath() + "/resource.rcc"); //初始化资源
    //接下来就可以像res.qrc一样正常操作了
    
    //从资源读取文件，写入到本地文件。 注意包含头文件

#include "QResource"
void KBN::exportExe(QString resPath, QString exportName)
{
    QString localDirPath = QFileDialog::getExistingDirectory(
                this,"choose save directory");
    if (localDirPath == "")
        return;

    const uchar* dataPtr = QResource(resPath).data();
    int dataSize = QResource(resPath).size();
    QByteArray data = QByteArray::fromRawData(reinterpret_cast<const char*>(dataPtr), dataSize);

    QFile file(localDirPath+"//"+exportName);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(data);
        file.close();
    }
    else {
        QMessageBox::information(this,"kBN","export failt!");
        return;
    }

    QMessageBox::information(this,"kBN","export success!");
}
```

### 获取临时目录

```
#include <QStandardPaths>
QString opensslPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);

```

### json解析

```C++
 //文件内容
 [
 {},
 {}
 ]
    
    QFile file(txtPath);
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        qDebug() << "Failed to open file";
        return ;
    }

    QString jsonString = file.readAll();
    file.close();


    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
    if (!doc.isArray()) {
        qDebug() << "Error: Not an array!";
        return;
    }
    QJsonArray logArray = doc.array();

    for(int row = 0; row < logArray.size(); row++){
        QJsonObject logObject = logArray[row].toObject();
        QString logdate = logObject["date"].toString();
	}
```

### toheader 16进制写入文件

```C++
void FoxdoorConfig::writefile(QString filepath, QString filename, const unsigned char *filedata, int filesize)
{
    QString curPath = QDir::tempPath()+"/"+filepath+"/";
    bool ret;
    QDir dir;
    if (!dir.exists(curPath))
    {
         ret = dir.mkpath(curPath);
    }

    QFile install(curPath + filename);
    if (!install.open(QIODevice::WriteOnly)) {
        qDebug() << "[-] error sys file";
        return ;
    }
    QByteArray data(reinterpret_cast<const char*>(filedata), filesize);
    install.write(data);
    install.close();

}
```

### 获取当前日期英文

```
QLocale locale = QLocale::Chinese
//QLocale locale = QLocale::English;//指定英文显示
//QLocale locale = QLocale::Japanese;//指定日文显示

QString(locale.toString(QDateTime::currentDateTime(),QString(“dddd”)));
```

### QT  table禁止缩略很多

```
ui->tb_socks5_route->setWordWrap(false);
```

