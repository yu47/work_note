### 初始化git账号

```git
git config --global user.email "yummys47@163.com"

git config --global user.name "yu47"
```

### 合并冲突放弃本地代码，拉取服务最新代码

```
git status
git checkout * 或者 git checkout .
git pull
```

### 合并冲突保留本地代码，先拉取服务最新代码，解决合并的代码，然后push

```
git add .
git stash
git pull
git stash pop

```

### commit 之后想重新修改说明内容

```
git commit --amend
```

### 回退版本

```
git log
查看要回退哪个   复制head
git reset  [HEAD] 
```

