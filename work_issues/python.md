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

### 打包

```
    pip install nuitka
    apt install patchelf
    # 打包命令
    python -m nuitka --standalone --output-dir=out --show-progress --onefile --plugin-enable=upx --upx-binary=/home/ubuntu/pack_seckill/upx/upx pack_seckill.py
    sudo chmod +x ./pack_seckill.bin
    ./pack_seckill.bin


#windows
pip install ordered-set # 加速编译
    pip install nuitka
    pip install zstandard # onefile时压缩文件
    # 打包命令
    python -m nuitka --mingw64 --standalone --output-dir=out --show-progress --onefile --plugin-enable=upx --enable-plugin=tk-inter --windows-disable-console pack_seckill.py
```



### 粘贴板输入

```python
import time
import tkinter as tk
import signal
import keyboard
from pynput.keyboard import Controller

def signal_handler(sig, frame):
    pass

signal.signal(signal.SIGINT, signal_handler)

def pynpu_unicode(word):
    for char in word:
        Controller().type(char)

      
def sendContent():
    root = tk.Tk()
    content = root.clipboard_get()
    root.destroy()
    pynpu_unicode(content)

def on_ctrl_alt_v():
    time.sleep(0.5)
    sendContent()

 
keyboard.add_hotkey('ctrl+alt+v', on_ctrl_alt_v)
keyboard.wait('ctrl+alt+q') 


```

linux service自启动 设置xhost 失败。解决办法： unable to open display ":0"

```
sed -i '$a xhost +SI:localuser:root' /etc/profile
source /etc/profile
```



