#include "baseinfo.h"
#include <QProcess>

#include <QDebug>

#define QDATETIME_NOW qPrintable(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))

QMutex BaseInfo::m_Mutex;
QSharedPointer<BaseInfo> BaseInfo::self;

BaseInfo::BaseInfo(QObject *parent) : QObject(parent)
{
    cmd = new QProcess(this);
    connect(cmd , SIGNAL(readyReadStandardOutput()) , this , SLOT(on_readoutput()));
    connect(cmd , SIGNAL(readyReadStandardError()) , this , SLOT(on_readerror()));
}
BaseInfo::~BaseInfo()
{
    cmd->close();
    cmd->waitForFinished();
}
void BaseInfo::on_readoutput()
{
    //读取返回数据
    cmdback=cmd->readAllStandardOutput().data();
    qDebug()<<cmdback;
}
void BaseInfo::on_readerror()
{
    //错误信息
}
/*
 * @brief   获取cpu名称
 * @return  cpu名称
 */
QString BaseInfo::GetcpuName()
{
    QString cpu_name = "";
#if defined(Q_OS_WIN32)
    QProcess p(0);
    p.start("wmic cpu get Name");
    p.waitForStarted();
    p.waitForFinished();
    cpu_name = QString::fromLocal8Bit(p.readAllStandardOutput());
    cpu_name = cpu_name.remove("Name").trimmed();
#elif defined(Q_OS_LINUX)
    cmd->start("bash");//启动终端(Windows下改为cmd)
    cmd->waitForStarted();//等待启动完成
    cmd->write("cat /proc/cpuinfo\n");//向终端写入“ls”命令，注意尾部的“\n”不可省略
#endif

    return cpu_name;
}
/*
 * @brief   获取cpu核心数
 * @return  cpu核心数
 */
QString BaseInfo::GetcpuCoreNumber()
{
    QString cpu_core = "";
#if defined(Q_OS_WIN32)
    QProcess p(0);
    p.start("wmic cpu get NumberOfCores");
    p.waitForStarted();
    p.waitForFinished();
    cpu_core = QString::fromLocal8Bit(p.readAllStandardOutput());
    cpu_core = cpu_core.remove("NumberOfCores").trimmed();
#elif defined(Q_OS_LINUX)

#endif

    return cpu_core;
}
/*
 * @brief   获取cpu线程数
 * @return  cpu线程数
 */
QString BaseInfo::GetcpuLogicalProcessors()
{
    QString cpu_process = "";
#if defined(Q_OS_WIN32)
    QProcess p(0);
    p.start("wmic cpu get NumberOfLogicalProcessors");
    p.waitForStarted();
    p.waitForFinished();
    cpu_process = QString::fromLocal8Bit(p.readAllStandardOutput());
    cpu_process = cpu_process.remove("NumberOfLogicalProcessors").trimmed();
#elif defined(Q_OS_LINUX)

#endif

    return cpu_process;
}
/*
 * @brief   获取cpu序列号
 * @return  cpu序列号
 */
QString BaseInfo::GetcpuProcessorId()
{
    QString cpu_id = "";
#if defined(Q_OS_WIN32)
    QProcess p(0);
    p.start("wmic CPU get ProcessorID");
    p.waitForStarted();
    p.waitForFinished();
    cpu_id = QString::fromLocal8Bit(p.readAllStandardOutput());
    cpu_id = cpu_id.remove("ProcessorId").trimmed();
#elif defined(Q_OS_LINUX)
#endif

    return cpu_id;
}
/*
 * @brief   获取主板序列号
 * @return  主板序列号
 */
QString BaseInfo::GetcpuBoardNumber()
{
    QString board_number = "";
#if defined(Q_OS_WIN32)
    QProcess p(0);
    p.start("wmic baseboard get serialnumber");
    p.waitForStarted();
    p.waitForFinished();
    board_number = QString::fromLocal8Bit(p.readAllStandardOutput());
    board_number = board_number.remove("SerialNumber").trimmed();
#elif defined(Q_OS_LINUX)

#endif

    return board_number;
}
/*
 * @brief   获取BIOS序列号
 * @return  BIOS序列号
 */
QString BaseInfo::GetcpuBiosNumber()
{
    QString bios_number = "";
#if defined(Q_OS_WIN32)
    QProcess p(0);
    p.start("wmic bios get serialnumber");
    p.waitForStarted();
    p.waitForFinished();
    bios_number = QString::fromLocal8Bit(p.readAllStandardOutput());
    bios_number = bios_number.remove("SerialNumber").trimmed();
#elif defined(Q_OS_LINUX)

#endif

    return bios_number;
}
/*
 * @brief   查看硬盘
 * @return  硬盘信息
 */
QString BaseInfo::GetcpuDiskDrive()
{
    QString disk_drive = "";
#if defined(Q_OS_WIN32)
    QProcess p(0);
    p.start("wmic diskdrive get serialnumber");
    p.waitForStarted();
    p.waitForFinished();
    disk_drive = QString::fromLocal8Bit(p.readAllStandardOutput());
    disk_drive = disk_drive.remove("SerialNumber").trimmed();
#elif defined(Q_OS_LINUX)

#endif

    return disk_drive;
}

#if defined(Q_OS_WIN32)
/*
 * @brief   查询CPU信息，依赖window.h
 * @return  SYSTEM_INFO
 */
SYSTEM_INFO BaseInfo::GetcpuInfo()
{
    SYSTEM_INFO info;
    GetSystemInfo(&info);

//    qDebug()<< "处理器掩码：" << info.dwActiveProcessorMask;
//    qDebug()<< "分配粒度:" << info.dwAllocationGranularity;
//    qDebug()<< "处理器个数：" << info.dwNumberOfProcessors;
//    qDebug()<< "处理器分页大小：" << info.dwPageSize;
//    qDebug()<< "处理器类型：" << info.dwProcessorType;
//    qDebug()<< "最大寻址单元：" << info.lpMaximumApplicationAddress;
//    qDebug()<< "最小寻址单元：" << info.lpMinimumApplicationAddress;
//    qDebug()<< "处理器等级：" << info.wProcessorLevel;
//    qDebug()<< "处理器版本：" << info.wProcessorRevision;

    return info;
}
/*
 * @brief   查询内存信息，依赖window.h
 * @return  MEMORYSTATUS
 */
MEMORYSTATUS BaseInfo::GetmemoryStatus()
{
    MEMORYSTATUS memoryStatus;
    GlobalMemoryStatus(&memoryStatus);

//    qDebug()<< "内存繁忙程度：" << memoryStatus.dwMemoryLoad;
//    qDebug()<< "总物理内存：" << memoryStatus.dwTotalPhys;
//    qDebug()<< "可用物理内存：" << memoryStatus.dwAvailPhys;
//    qDebug()<< "总页文件：" << memoryStatus.dwTotalPageFile;
//    qDebug()<< "可用页文件：" << memoryStatus.dwAvailPageFile;
//    qDebug()<< "总进程空间：" << memoryStatus.dwTotalVirtual;
//    qDebug()<< "可用进程空间：" << memoryStatus.dwAvailVirtual;
//    qDebug()<< "内存使用百分比：" << memoryStatus.dwLength;

    return memoryStatus;
}
MEMORYSTATUSEX BaseInfo::GetmemoryStatusex()
{
    MEMORYSTATUSEX memoryStatusex;
    GlobalMemoryStatusEx(&memoryStatusex);

//    qDebug()<< "内存使用率：" << memoryStatusex.dwLength;
//    qDebug()<< "物理内存使用率" << memoryStatusex.dwMemoryLoad;
//    qDebug()<< "额外物理内存" << memoryStatusex.ullAvailExtendedVirtual;
//    qDebug()<< "总页文件：" << memoryStatusex.ullAvailPageFile;
//    qDebug()<< "可用物理内存：" << memoryStatusex.ullAvailPhys;
//    qDebug()<< "可用虚拟内存：" << memoryStatusex.ullAvailVirtual;
//    qDebug()<< "系统页面文件大小：" << memoryStatusex.ullTotalPageFile;
//    qDebug()<< "物理内存总量：" << memoryStatusex.ullTotalPhys;
//    qDebug()<< "虚拟内存总量：" << memoryStatusex.ullTotalVirtual;

    return memoryStatusex;
}
/*
 * @brief   进程区域信息，依赖window.h
 * @return  MEMORY_BASIC_INFORMATION
 */
MEMORY_BASIC_INFORMATION BaseInfo::GetMemoryBasic()
{
    char arrayA[4097];
    long len=sizeof(MEMORY_BASIC_INFORMATION);
    MEMORY_BASIC_INFORMATION memorybase;
    VirtualQuery(arrayA,&memorybase,len);

//    qDebug()<<"区域基地址="<<memorybase.AllocationBase;
//    qDebug()<<"区域邻近页面状态="<<memorybase.State;
//    qDebug()<<"区域保护属性="<<memorybase.AllocationProtect;
//    qDebug()<<"页面基地址="<<memorybase.BaseAddress;
//    qDebug()<<QString("arrayA指针地址=%1/n").arg(arrayA);
//    qDebug()<<"从页面基地址开始的大小="<<memorybase.RegionSize;
//    qDebug()<<"邻近页面物理存储器类型="<<memorybase.Type;
//    qDebug()<<"页面保护属性="<<memorybase.Protect;

    return memorybase;
}

#elif defined(Q_OS_LINUX)

#endif
