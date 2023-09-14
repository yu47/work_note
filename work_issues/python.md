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

