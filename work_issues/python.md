### 写入一个文件

```
import datetime

def write_to_file(string):
    timestamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    content = f"[{timestamp}] {string}\n"
    
    with open("output.txt", "a") as file:
        file.write(content)

# 示例用法
write_to_file("这是一条示例文本")
```

### 转化编码（ Unicode 转义序列转换为对应的中文字符）

```
string = r"\u60a8\u4f3c\u4e4e\u5df2\u7ecf\u7b7e\u5230\u8fc7\u4e86"
decoded_string = bytes(string, "utf-8").decode("unicode_escape")
print(decoded_string)

```



##### 本地源

```
pip2pi ./pypi_packages -r request.txt

dir2pi ./pypi_packages   //生产simple

#搭建web
python -m http.server 8080 --bind your_machine_ip --directory /path/to/pypi_packages/simple

# 添加对搜索IP的信任，否则会install失败
pip config set install.trusted-host 168.163.1.1
```

