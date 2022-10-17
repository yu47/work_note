### QT编译出现 not found libxkbcommon-x11 

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

    QString hideFilePaths = "test";
    unsigned char* buf = (unsigned char*)hideFilePaths.toLatin1().data();
### QT中QString 转 char*

```
QString hideFilePaths = "test";
char* buf = (char*)hideFilePaths.toStdString().c_str();
```

