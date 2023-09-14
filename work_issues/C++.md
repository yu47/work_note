### 添加资源文件

![](../\images\C++addres.png)

![](../\images\C++addres2.png)

![](../\images\C++addres3.png)

```
//写入本地文件
void writeFile(const std::string& name, int IDR) {
    HMODULE hInstance = GetModuleHandle(NULL);
    HRSRC hResource = FindResource(hInstance, MAKEINTRESOURCE(IDR), RT_RCDATA);
    HGLOBAL hResourceData = LoadResource(hInstance, hResource);
    LPVOID pData = LockResource(hResourceData);
    DWORD dwDataSize = SizeofResource(hInstance, hResource);
    std::wstring widestr = std::wstring(name.begin(), name.end());
    const wchar_t* widecstr = widestr.c_str();

    HANDLE hFile = CreateFile(widecstr, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        DWORD dwBytesWritten;
        if (WriteFile(hFile, pData, dwDataSize, &dwBytesWritten, NULL))
        {
            // RCDATA 数据写入文件成功
        }
        CloseHandle(hFile);
    }
    //GlobalFree(hResourceData);
}
//执行写入
//参数1 写入路径
//参数2 头文件里的对应资源文件的宏定义
writeFile("D://111", IDR_RCDATA1);
```

### 执行CMD命令

```
bool execute_command(const std::string cmd, std::string& output, size_t size) {
    char tmp[4096] = { 0 };
    FILE* pipe = _popen(cmd.c_str(), "r");
    if (!pipe2) {
        printf("Failed to execute command\n");
        return 1;
    }
    while (fgets(tmp, size, pipe2) != nullptr) {
        output += tmp;
    }
    _pclose(pipe);
}
// 如果需要交互式，则通过fput提交
    fputs("y\n", pipe); 
    fflush(pipe);
    fputs("a\0\n\n", pipe); 
    fflush(pipe);
    fflush(pipe);
```

### C++ 后台静默启动（C++程序最上面添加）

```
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
```

### 读取dll 文件的函数,并使用。

```
typedef U32(*PBmapiInitDiag)(IN U32 handle);
int main(){
	U32 status;
	U32 handle;
	HMODULE Hdll = LoadLibrary(L"BMAPIa.dll");
    PBmapiInitDiag BmapiInitDiag = (PBmapiInitDiag)GetProcAddress(Hdll, "BmapiInitDiag");
	status = BmapiInitDiag(handle);
}
```

### 写入文件日志

```
void writeLog(const char* message) {
	FILE* file;
	time_t rawtime;
	struct tm* timeinfo;
	char buffer[80];


	// 打开日志文件，追加写入
	file = fopen("C:\\Users\\admin\\logfile.txt", "a");
	if (file == NULL) {
		printf("无法打开日志文件\n");
		return;
	}

	// 写入日志信息
	fprintf(file, "[%s] %s\n", buffer, message);

	// 关闭文件
	fclose(file);
}
```

### 获取自身程序名

```
    char buffer[MAX_PATH] = {0};
    GetModuleFileNameA(NULL, buffer, MAX_PATH);

    std::string programPath(buffer);
    size_t pos = programPath.find_last_of("\\");

    char* selfName = NULL;
    selfName = (char*)buffer + pos + 1;
```

### 执行cmd 不弹出的方法

```
 WinExec("cmd.exe /c cmd", SW_HIDE);
```

