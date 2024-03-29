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

 取消commit

```
git reset --soft HEAD~1 
回到上一次commit前
git reset --soft HEAD~2
回到上上次commit前
```

### 只拉取项目中的，某个目录

```
$ git init
$ git remote add -f origin https://github.com/XXXXX/test.git    #拉取remote的all objects信息

$ git config core.sparsecheckout true   #开启sparse clone
$ echo "build" >> .git/info/sparse-checkout   

$ less .git/info/sparse-checkout
  build
$  git pull origin master or main  #拉取
$ ls
 build
```

### 令牌

```
git remote set-url origin https://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@github.com/yu47/work_note.git
```

### .gitignore 不生效的解决	

```
git rm -r --cached .
git add .
git commit -m 'update .gitignore'
git push -u origin main
```

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