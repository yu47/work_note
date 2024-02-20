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

### char to wchar

```
	char runname[32] = "rundll32.exe";

	wchar_t* pwszUnicode;

	size_t iSize = MultiByteToWideChar(CP_ACP, 0, runname, -1, NULL, 0);
	pwszUnicode = (wchar_t*)malloc(iSize * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, runname, -1, pwszUnicode, iSize);
	
	
	
void s2ws(char* charstr, wchar_t* wcharstr, int len)
{
	MultiByteToWideChar(CP_ACP, 0, charstr, -1, wcharstr, len);
}
char test[32] = "12234567";
wchar_t wtest[32] = { 0 };
s2ws(test, wtest, 8 * 2);

bool UnicodeToUtf8(const wchar_t* lpszUnicode, char* lpszUtf8, int nLen)
{
	int nRet = ::WideCharToMultiByte(CP_UTF8, 0, lpszUnicode, -1, lpszUtf8, nLen, NULL, NULL);
	return (0 == nRet) ? FALSE : TRUE;
}

bool Utf8ToUnicode(const char* lpszUtf8, wchar_t* lpszUnicode, int nLen)
{
	int nRet = ::MultiByteToWideChar(CP_UTF8, 0, lpszUtf8, -1, lpszUnicode, nLen);
	return (0 == nRet) ? FALSE : TRUE;
}
```

### 取消const 转化提示

![](../\images\Snipaste_2023-11-07_18-06-34.png)

wchar_t

```
wcscat
wsclen
```

### 日志

```

freopen("文件名", "a", stdout);

#define PR2(str, ...) {fprintf(stdout, " %s:%d  %s()\n  %s:  " str "\n\n", \
 __FILE__ ,__LINE__,__func__, __TIMESTAMP__, ##__VA_ARGS__);fflush(stdout);}


输出系统时间

printf("\n mycode  Build on "__TIME__" "__DATE__" win7\n");

打印的结果是：

 mycode  Build on 15:39:58 Apr 19 2019 win7

系统常用的宏定义：

__TIME__：当前系统时间  例如 15:39:58

__DATE__：当前系统日期  Apr 19 2019

__TIMESTAMP__：当前时间日期 例如：  printf(__TIMESTAMP__）;   打印结果Fri Apr 19 16:24:31 2019

__LINE__： 当前代码行 例如： printf("%d",__LINE__);

__FILE__：执行的当前文件名  例如：  printf(__FILE__）;

__FUNCTION__：打印函数名 printf("%s",__FUNCTION__); 或者printf(__FUNCTION__）;只要是字符串的都可以这样

```

