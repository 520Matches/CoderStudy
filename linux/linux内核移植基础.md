## linux内核移植基础

### cmd
- 用file命令来观察可执行文件的基本信息（包括是什么CPU架构的信息）
> file build
- 用readelf来读取文件头信息
> readelf -h abc
- size命令可以查看二进制文件的内存分布结构，（代码段，数据段，bss等等）
> size abc
- head命令和cat命令的区别
> head是显示文件的头几行,cat是显示全部
- xxd：以16进制形式读取二进制文件
> xxd aaa
- dd命令：读取文件的工具
> dd skip=52 count=64 if=aaa of=out_aaa bs=1;skip是从哪个位置开始读取，count是要读取的数据的长度，if是输入文件，of是输出文件，bs是读取的单位（1表示一个字节）
- nm命令：需要查看相关符号表来知道各个标签的意思
- strip命令用来剔除可执行文件中的符号表
- strings命令用来查看可执行文件中的常量字符串
- objdump是用来反汇编的
- objcopy是用来copy相应的代码段，数据段等程序实际要运行用的数据
- addr2line命令用来调试代码的，一般情况下用的不多，详细请百度


### uboot
- "?"可以 查看uboot支持的命令
- print：打印环境变量
- setenv:设置环境变量
> setenv abc 100	添加环境变量
> setenv abc	不加环境变量值就是删除环境变量
- saveenv:把环境变量保存到存储器中(如：flash)中
> saveenv
- tftp:基于UDP的用来传输数据的
> tftp 20008000 abc.txt	第二个参数是16进制的地址值，第三个参数是要下载的文件名
- md:memory display(内存显示)
> md 20008000	(显示内存中的值)
- nand:nand[动词] [内存地址] [nandflash的内部地址] [size]
- bootm:启动uImage
- go:启动内核命令，把PC指针指向内核

### kernel二进制文件
- uImage(专门为uboot定做的)、zImage、Image、bzImage	（后3个基本为内核原始二进制文件）
- 从uboot启动内核的条件，把PC指针指向内核，并且传递相应的启动(bootargs)参数(root:启动的根文件系统在哪个设备。设备信息:ram nfs flash。init:内核启动后的第一个可执行文件init进程从哪里来。console:内核启动时使用什么设备作为控制台。initrd:文件系统的地址和大小==initrd 0x2000000 ,8M。nfsroot:nfsroot=192.168.10.110:/home/shd/work/rootfs。ip:指开发板的ip==ip:192.168.10.111)。还要有文件系统

### 文件系统
- ramdisk:
- nfs:网络文件系统

### 编译原理
- xxx.c->编译(xxx.s)->汇编(xxx.o)->连接(链接多个.o文件，一般只能编译品台运行)->生成二进制文件(objdump)
- ld：.o文件的链接命令
- .lds:链接脚本文件

### makefile
- 执行要求:比较文件(目标文件和依赖文件)时间来判断是否需要执行命令
- 直接运行make，默认会去生成第一个目标
- .PHONY:clean 可以定义假想目标，防止目标刚好已经有存在的文件（这里是clean文件）了而不去执行
- 格式
```makefile
目标:依赖
	命令
```
- 在“命令”前添加“@”符号可以在make的时候不打印命令
- 伪目标:不是真正的目标，可以用.PHONY:xxx声明
- 自动化变量:
```
$@:目标集合
$<:所有依赖的第一项
$^:所有的依赖
```
- 模式匹配:
```
%.o:所有当前目录下的.o文件
%.c:所有当前目录下的.c文件
```
- makefile中用":="符号表示等于,"="会有交叉传递的概念

- makefile中的一些常用内置函数
```
foreach:
names := a b c d
files := $(foreach n,$(names),$(n).o)
$(files)的值是a.o b.o c.o d.o
```

```
filter:
C = a b c d/
D = $(filter %/,$(C));从$(C)中取出符合%/的值
E = $(filter-out %/,$(C));从$(E)中取出不符合%/的值
$(D)的值是d/，$(E)的值是a b c
```

```
wildcard:
files = $(wildcard,*.c)
$(files)的值等于当前目录下所有的.c文件，并且是以空格分割的字符串
```

```
patsubst:
files = a.c b.c c.c d.S
dep_files = $(patsubst,%.c,%.o,$(files))
$(dep_files)的值是a.o b.o c.o d.S
```

### 同步通讯SPI
- 缺点:通讯距离很短，一般只在同一块板子上使用。因为距离一长时钟信号和数据信号有相位差

### 异步通讯UART
- uart:全双工，发送方和接受方要有相同的波特率

### DDR内存
- 
