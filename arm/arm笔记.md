# ARM笔记

## cortex-A cortex-R cortex-M的区别
- cortex-A:Application Processors（应用处理器）–面向移动计算，智能手机，服务器等市场的的高端处理器。这类处理器运行在很高的时钟频率（超过1GHz），支持像Linux，Android，MS Windows和移动操作系统等完整操作系统需要的内存管理单元（MMU）。 如果规划开发的产品需要运行上述其中的一个操作系统，你需要选择ARM 应用处理器。
- cortex-R:Real-time Processors（实时处理器）–面向实时应用的高性能处理器系列，例如硬盘控制器，汽车传动系统和无线通讯的基带控制。多数实时处理器不支持MMU，不过通常具有MPU、Cache和其他针对工业应用设计的存储器功能。实时处理器运行在比较高的时钟频率（例如200MHz 到 >1GHz ），响应延迟非常低。虽然实时处理器不能运行完整版本的Linux和Windows操作系统，但是支持大量的实时操作系统（RTOS）。
- cortex-M:Microcontroller Processors（微控制器处理器）–微控制器处理器通常设计成面积很小和能效比很高。通常这些处理器的流水线很短，最高时钟频率很低（虽然市场上有此类的处理器可以运行在200Mhz之上）。 并且，新的Cortex-M处理器家族设计的非常容易使用。因此，ARM 微控制器处理器在单片机和深度嵌入式系统市场非常成功和受欢迎。cortex-M不可以运行Linux操作系统，因为M系列没有Linux操作系统必备的内存管理单元（MMU）。

## CPU工作状态
- ARM状态(32位的指令)
- THumb状态(16位的指令，节省空间)
- 汇编代码:使用ARM指令集的时候加上“.arm”,使用THumb指令集的时候加上".thumb"
- C代码:在编译的时候加上“-marm”表示使用的是arm指令集,"-mthumb"表示使用的是thumb指令集

### 模式(ARM9)
- 用户模式(USR):正常程序执行模式,不能直接切换到其他模式
- 系统模式(SYS):运行操作系统特权任务，与用户模式类似，但具有可以直接切换到其他模式等特权
- 快中断模式(FIQ):支持高速数据传输及通道处理，FIQ异常响应时进入此模式
- 中断模式(IRQ):用于通用中断处理，IRQ异常响应时进入此模式
- 管理模式(SVC):操作系统保护模式，系统复位和软件中断响应时进入此模式（由系统调用执行软中断SWI命令触发）
- 中止模式(ABT):用于支持虚拟内存或存储器保护，在ARM7TDMI没大用处
- 未定义模式(UND):支持硬件协处理器软件仿真，未定义指令异常响应时进入此模式

### 寄存器
- Cortex-A7的寄存器图片
![](./register.png)
- R13:SP(Stack Pointerl)栈指针
> SP寄存器有值就是满栈（先调整SP，再存放数据），SP寄存器没有值就是空栈（先存放数据，再调整SP）
- R14:LR(Link Register)用来保存返回地址
- 寄存器R15被用作程序计数器，也称为PC，其值等于当前正在执行指令的地址+8（因为在取址和执行之间多了一个译码阶段）
- PSR:(Program strtus Register)程序状态寄存器

- 对于cortex-M3/M4来说xPSR对应3个寄存器
> APSR:Application PSR,应用PSR
> IPSR:Interrupt PSR,中断PSR
> EPSR:Exectution PSR,执行PSR
> 这3个寄存器可以单独访问:
| 指令        | 解析   |
| ---         | ---    |
| MRS R0,APSR | 读APSR |
| MRS R0,IPSR | 读IPSR |
| MSR APSR,R0 | 写APSR |
> 这3个寄存器也可以一次性访问:
| 指令       | 解析   |
| ---        | ---    |
| MRS R0,PSR | 读组合状态寄存器 |
| MSR PSR,R0 | 写组合状态寄存器 |

### 寻址方式
- 立即数寻址
- 寄存器寻址
- 寄存器间接寻址
- 基址变址寄存器
- 多寄存器寻址
- 相对寻址
- 堆栈寻址

### ARM指令
- B：(Branch) 跳转
- BL：(Branch with Link)跳转前先把返回地址保存在LR寄存器中
- BX：(Branch and eXchange)根据跳转地址的BIT0切换为ARM状态或者Thumb状态（0:ARM状态，1:Thumb状态）
- BLX：(Branch with link and eXchange)
- !:表示是否改变寄存器的值,ldmfd sp!,{r0-r12}:最后会改变sp值
- ^:在目标寄存器中有PC时，会同时将spsr写入到cpsr，一般用于异常模式返回。ldmfd sp!,{R0-R12,PC}^

### 中断FIQ & IRQ
IRQ模式下，中断处理程序需要自己保存R8到R12这几个额寄存器，退出中断处理时需要自己恢复这几个寄存器，退出中断处理时需要自己恢复这几个寄存器，而FIQ模式由于这几个寄存器都有back寄存器（fiq_R8）,模式切换时CPU自动保存这些值到back寄存器，退出FIQ模式时自动恢复，所以这个过程FIQ比IRQ快。FIQ比IRQ有更高的优先级，如果FIQ和IRQ同时产生，那么FIQ先处理。

### linux操作系统和arm工作模式
首先，ARM开发板在刚上电或复位后都会首先进入SVC即管理模式，此时、程序计数器R15-PC值会被赋为0x0000 0000；bootloader就是在此模式下，位于0x0000 0000的NOR FLASH或SRAM中装载的，因此、开机或重启后bootloader会被首先执行。
    接着，bootloader引导Linux内核，此时、Linux内核一样运行在ARM的SVC即管理模式下；当内核启动完毕、准备进入用户态init进程时，内核将ARM的当前程序状态CPSR寄存器M[4:0]设置为10000、进而用户态程序只能运行在ARM的用户模式。
    由于ARM用户模式下对资源的访问受限，因此、可以达到保护Linux操作系统内核的目的。
    需要强调的是：Linux内核态是从ARM的SVC即管理模式下启动的，但在某些情况下、如：硬件中断、程序异常（被动）等情况下进入ARM的其他特权模式，这时仍然可以进入内核态（因为就是可以操作内核了）；同样，Linux用户态是从ARM用户模式启动的，但当进入ARM系统模式时、仍然可以操作Linux用户态程序（进入用户态，如init进程的启动过程）。
    即：Linux内核从ARM的SVC模式下启动，但内核态不仅仅指ARM的SVC模式（还包括可以访问内核空间的所有ARM模式）；Linux用户程序从ARM的用户模式启动，但用户态不仅仅指ARM的用户模式首先，ARM开发板在刚上电或复位后都会首先进入SVC即管理模式，此时、程序计数器R15-PC值会被赋为0x0000 0000；bootloader就是在此模式下，位于0x0000 0000的NOR FLASH或SRAM中装载的，因此、开机或重启后bootloader会被首先执行。
    接着，bootloader引导Linux内核，此时、Linux内核一样运行在ARM的SVC即管理模式下；当内核启动完毕、准备进入用户态init进程时，内核将ARM的当前程序状态CPSR寄存器M[4:0]设置为10000、进而用户态程序只能运行在ARM的用户模式。
    由于ARM用户模式下对资源的访问受限，因此、可以达到保护Linux操作系统内核的目的。
    需要强调的是：Linux内核态是从ARM的SVC即管理模式下启动的，但在某些情况下、如：硬件中断、程序异常（被动）等情况下进入ARM的其他特权模式，这时仍然可以进入内核态（因为就是可以操作内核了）；同样，Linux用户态是从ARM用户模式启动的，但当进入ARM系统模式时、仍然可以操作Linux用户态程序（进入用户态，如init进程的启动过程）。
    即：Linux内核从ARM的SVC模式下启动，但内核态不仅仅指ARM的SVC模式（还包括可以访问内核空间的所有ARM模式）；Linux用户程序从ARM的用户模式启动，但用户态不仅仅指ARM的用户模式首先，ARM开发板在刚上电或复位后都会首先进入SVC即管理模式，此时、程序计数器R15-PC值会被赋为0x0000 0000；bootloader就是在此模式下，位于0x0000 0000的NOR FLASH或SRAM中装载的，因此、开机或重启后bootloader会被首先执行。
    接着，bootloader引导Linux内核，此时、Linux内核一样运行在ARM的SVC即管理模式下；当内核启动完毕、准备进入用户态init进程时，内核将ARM的当前程序状态CPSR寄存器M[4:0]设置为10000、进而用户态程序只能运行在ARM的用户模式。
    由于ARM用户模式下对资源的访问受限，因此、可以达到保护Linux操作系统内核的目的。
    需要强调的是：Linux内核态是从ARM的SVC即管理模式下启动的，但在某些情况下、如：硬件中断、程序异常（被动）等情况下进入ARM的其他特权模式，这时仍然可以进入内核态（因为就是可以操作内核了）；同样，Linux用户态是从ARM用户模式启动的，但当进入ARM系统模式时、仍然可以操作Linux用户态程序（进入用户态，如init进程的启动过程）。
    即：Linux内核从ARM的SVC模式下启动，但内核态不仅仅指ARM的SVC模式（还包括可以访问内核空间的所有ARM模式）；Linux用户程序从ARM的用户模式启动，但用户态不仅仅指ARM的用户模式首先，ARM开发板在刚上电或复位后都会首先进入SVC即管理模式，此时、程序计数器R15-PC值会被赋为0x0000 0000；bootloader就是在此模式下，位于0x0000 0000的NOR FLASH或SRAM中装载的，因此、开机或重启后bootloader会被首先执行。
    接着，bootloader引导Linux内核，此时、Linux内核一样运行在ARM的SVC即管理模式下；当内核启动完毕、准备进入用户态init进程时，内核将ARM的当前程序状态CPSR寄存器M[4:0]设置为10000、进而用户态程序只能运行在ARM的用户模式。
    由于ARM用户模式下对资源的访问受限，因此、可以达到保护Linux操作系统内核的目的。
    需要强调的是：Linux内核态是从ARM的SVC即管理模式下启动的，但在某些情况下、如：硬件中断、程序异常（被动）等情况下进入ARM的其他特权模式，这时仍然可以进入内核态（因为就是可以操作内核了）；同样，Linux用户态是从ARM用户模式启动的，但当进入ARM系统模式时、仍然可以操作Linux用户态程序（进入用户态，如init进程的启动过程）。
    即：Linux内核从ARM的SVC模式下启动，但内核态不仅仅指ARM的SVC模式（还包括可以访问内核空间的所有ARM模式）；Linux用户程序从ARM的用户模式启动，但用户态不仅仅指ARM的用户模式

### 专业名词
- CCM    : Clock Controller Module(时钟控制模块)
- IOMUXC : IOMUXC Controller(IO复用控制器)
- GPIO   : General-purpose input/output(通用输入输出口)

### GPIO
- 通过CCM去使能引脚
- 通过IOMUXC去设置引脚为GPIO模式

### RISC & CISC
- CISC:(Complex Instruction SetComputer)复杂指令集
- RISC:(Reduced Instructiion Set Computer)精简指令集
> CISC的指令能力强，但多数指令使用率低，增加了CPU的复杂度，指令是可变长格式。
> RISC的指令大部分为单周期指令，指令长度固定，操作寄存器，对于内存只有Load/Store操作
> CISC支持多种寻址方式
> CISC通过微控制技术实现
> RISC增加了通用寄存器，硬布线逻辑控制为主，采用流水线
> CISC的研制周期长
> RISC优化编译，有效支持高级语言

### 异常处理
|               | cortexM3/M4        | cortexA7 |
|---------------|--------------------|----------|
| 保存现场      | 硬件完成           | 软件完成 |
| 分辨异常/中断 | 硬件完成           | 软件完成 |
| 调用处理函数  | 硬件完成           | 软件完成 |
| 恢复现场      | 软件出发，硬件完成 | 软件完成 |
|---------------|--------------------|----------|

- cortexA7产生中断/异常，CPU会强制进入那个异常模式，然后程序状态寄存器CPSR会被保存到那个异常模式下的SPSR寄存器，最后跳转到异常处理程序入口
- svc异常：使用指令``` svc #val ```会触发一个svc异常，在linux中可以用来用户态进入内核态的方法，比如open,read,write等函数
