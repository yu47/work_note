首先，更新软件包索引，并且安装必要的依赖软件，来添加一个新的 HTTPS 软件源：

```text
sudo apt update
sudo apt install apt-transport-https ca-certificates curl gnupg-agent software-properties-common
```

使用下面的 `curl` 导入源仓库的 GPG key：

```text
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
```

将 Docker APT 软件源添加到你的系统：

```text
sudo add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable"
```

现在，Docker 软件源被启用了，你可以安装软件源中任何可用的 Docker 版本。

01.想要安装 Docker 最新版本，运行下面的命令。如果你想安装指定版本，跳过这个步骤，并且跳到下一步。

```text
sudo apt update
sudo apt install docker-ce docker-ce-cli containerd.io
```

02.想要安装指定版本，首先列出 Docker 软件源中所有可用的版本：

```text
sudo apt update
apt list -a docker-ce
```

一旦安装完成，Docker 服务将会自动启动。你可以输入下面的命令，验证它：

```text
sudo systemctl status docker
```

输出将会类似下面这样：

```text
● docker.service - Docker Application Container Engine
     Loaded: loaded (/lib/systemd/system/docker.service; enabled; vendor preset: enabled)
```

当一个新的 Docker 发布时，你可以使用标准的`sudo apt update && sudo apt upgrade`流程来升级 Docker 软件包。

如果你想阻止 Docker 自动更新，锁住它的版本：

```text
sudo apt-mark hold docker-ce
```

### 二、以非 Root 用户身份执行 Docker

默认情况下，只有 root 或者 有 sudo 权限的用户可以执行 Docker 命令。

想要以非 root 用户执行 Docker 命令，你需要将你的用户添加到 Docker 用户组，该用户组在 Docker CE 软件包安装过程中被创建。想要这么做，输入：

```text
sudo usermod -aG docker $USER
```

`$USER`是一个环境变量，代表当前用户名。

登出，并且重新登录，以便用户组会员信息刷新。



### 三，拉取最新版的 Ubuntu 镜像

```
$ docker pull ubuntu
```

或者：

```
$ docker pull ubuntu:latest
```

### 3、查看本地镜像

```
$ docker images
```

在上图中可以看到我们已经安装了最新版本的 ubuntu。

### 4、运行容器，并且可以通过 exec 命令进入 ubuntu 容器

```
$ docker run -itd --name ubuntu-test ubuntu
```

### 四、卸载 Docker

在卸载 Docker 之前，你最好 移除所有的容器，镜像，卷和网络。

运行下面的命令停止所有正在运行的容器，并且移除所有的 docker 对象：

```text
docker container stop $(docker container ls -aq)
docker system prune -a --volumes
```

现在你可以使用`apt`像卸载其他软件包一样来卸载 Docker：

```text
sudo apt purge docker-ce
sudo apt autoremove
```

- ###### 自定义容器名称

  ```
  # --name 后面写一个名字，会在后台看到一个容器NAMES的名字是自己创建的
  docker run -it -d --name my_start_docker 镜像ID /bin/bash
  ```

### 五、安装库

```
docker exec -it ubuntu-test bash -c "apt-get update && apt-get install vim"
```

### 六、传输文件

```
docker cp  本机路径 820c89300dd2:容器路径
路径对换，docker 向本机传输文件
docker cp  820c89300dd2:容器路径  本机路径 
```

##### 七、映射端口

```
# 随机映射端口
docker run -it -P 容器ID/容器NAMES /bin/bash
# 指定端口映射
docker run -it -p 127.0.0.1:80:8080 容器ID/容器NAMES /bin/bash
```

- #### 停止容器

  ```
  docker stop 容器ID/容器NAMES
  ```

