# Cross Compilation Tool Chain

### 工具链组成

| arch    | vendor                       | os                      | libc/abi             |
|:-------:|:----------------------------:|:-----------------------:|:--------------------:|
| CPU架构 | 自定义字符串（被autoconf忽略） | 操作系统（none或者linux） | C库和使用的ABI的组合 |

- arm-foo-none-eabi:针对ARM架构的裸机工具链，来自供应商foo。
- arm-unknown-linux-gnueabihf:针对ARM架构的linux工具链，使用来自未知供应商的EABIhf ABI和glib C库。
- armeb-linux-uclibcgnueabi:针对ARM big-endian的linux工具链架构，使用EABI ABI和uClibc C库。
- mips-img-linux-gnu:针对MIPS架构的linux工具链，使用glib C库，由Imagination Technologies提供。

### ABI定义
- ABI = Application Binary Interface(应用程序二进制接口)
- 从工具链的角度来看，ABI定义了：
> 如何进行函数调用（所谓的调用约定）
>> 如何传递参数：在寄存器中（有哪些？）。在堆栈中，如何在32位架构上处理64位参数。
>> 返回值如何传递。
> 基本数据类型的大小。
> 机构体中成员的对齐
> 当有操作系统时，如何进行系统调用。
- 来自不同ABI的目标文件不能链接在一起（如果您有预构建的库或可执行文件，这一点尤其重要！）。
- 对于给定的CPU架构，可能存在无限数量的ABI：ABI只是关于如何使用CPU架构的规范。
- 需要了解没给架构的ABI。

### ABI例子 ARM32的示例
- OABI：过时的ABI。强制使用硬浮点指令，这需要在内核中模拟浮点运算。已不在任何地方支持。
- EABI：由ARM标准化。允许将硬浮点代码与软浮点代码混合。在整数寄存器中传递浮点参数。
> 硬浮点代码：直接使用浮点指令。
> 软浮点代码：使用gcc提供的用户控件库模拟浮点指令。
- EABIhf：也是由ARM标准化。需要一个专门的浮点单元：只有硬浮点代码。在浮点寄存器中传递的浮点参数。
- gcc选项：
> EABI soft-float:-mabi=aapcs-linux -mfloat-abi=soft
> EABI hard-float:-mabi-aapcs-linux -mfloat-abi=softfp
> EABIhf:-mabi=aapcs-linux -mfloat-abi=hard 

### OS两个重要的值
- none:用于bare-metal toolchains(裸机工具链)
> 用于没有操作系统的开发。
> 使用C库一般时newlib。
> 提供不需要操作系统的C库服务。
> 允许为特定的硬件目标提供基本的系统调用。
> 可用于构建bootloader程序或linux kernel,不能用于构建linux用户空间代码。
- linux for linux toolchains(linux工具链)
> 用于linux操作系统开发。
> 选择特定于linux的C库：glibc、uclibc、musl。
> 支持linux系统调用。
> 可用于构建linux用户空间代码，也可用于构建引导加载程序或者内核本身等裸机代码。

### Multilib工具链
- 大多数工具链提供一个带有C库和gcc运行时库的sysroot
- 这些为目标构建的库针对特定的架构变体和ABI进行了优化
- 每个架构变体或ABI需要有一个工具链
- multilib工具链包含多个sysroot，每个都有一个针对不同架构/ABI变体的目标库版本。
- sourcery codebench arm工具链示例：
> $arm-none-linux-gnueabi-gcc -print-multi-lib.;
> armv4t;@march=armv4t
> thumb2;@mtumb@march=armv7-a
- 三个系统根：ARMv5、ARMv4和ARMv7 Thumb-2
**编译器会更具gcc标志自动选择正确的sysroot**
**每个sysroot都有不同的库变体**

### SDK和工具链的区别
- 工具链：只有编译器、binutils（二进制工具集）和C库。
- SDK（software development kit）:一个工具链，加上一些（可能很大）为Target目标架构构建的库头文件，以及在构建软件时有用的其他本地工具（比如QT）。
- OpenEmbedded或Yocto Buildroot等构建系统通常可以：
> 使用现有工具链作为输入，或构建自己的工具链
> 除了生成根文件系统之外，他们还可以生成SDK以允许应用程序开发人员为目标构建应用程序库。
 
 ### sysroot概念
- sysroot是头文件和库的逻辑根目录。
- gcc寻找头文件，ld寻找库的地方
- gcc和binutils都是使用 --with-sysroot=<SYSROOT>参数构建的。
- 内核头文件和C库安装在<SYSROOT>。
- 如果工具链已经移动到不同的位置，如果它在 --prefix的子目录中，gcc仍然会找到它的sysroot,但是需要在编译时指定如下参数。
> --prefix=
> --with-sysroot=
- 可以在允许时使用gcc的 --sysroot选项覆盖。
- 可以使用 -print-sysroot选项打印当前的sysroot。