# LinuxInterview

- 使用GCC的"O0"优化选项来编译内核有什么优势？
> linux内核默认使用O2优化等级，但是在使用GDB调试的时候会出现光标乱跳，有些变量的值出现<optimezed out>等问题，影响调试效果。所以为了调试方便需要把"O2"改成"O0"，但这会导致一些编译问题，需要自己手动更改。于此同时还会让内核的运行效率变低。
- 什么是加载地址、运行地址，链接地址？
> 加载地址：存储代码的物理地址。
> 运行地址：程序执行的地址。
> 链接地址：在编译链接时指定的地址，是程序员设想将来程序运行的地址。程序中所有的标号的地址在程序链接好以后就确定了，不会因为程序在哪个地址行而改变。可以使用objdump命令来查看链接地址。
- 什么是位置无关指令？什么是位置有关指令？
- 什么是debugfs?
> debugfs可用于内核向用户空间提供信息，debugfs是个小型文件系统，与/proc和sysfs不同，debugfs根本没有较为严苛的规则和定义，开发人员可以在里面放置想要的任何信息，以便于系统开发和调试。
###### 通常使用如下命令安装`debugfs`：
```shell
mount -t debugfs none /sys/kernel/debug
```
```shell
mount -t debugfs debugfs /sys/kernel/debug/
```
###### 也可以在/etc/fstab文件下使用下面语句：
```shell
debug /sys/kernel/debug debugfs debugfs 0 0
```
- 什么是OOM？
> 当系统内存不够的时候，会产生OOM（out of memory），OOM Killer会计算出需要杀到的进程，被系统杀掉。
- 什么是OOP？
> OOP错误一般都是内存错误（越界访问，访问已经被释放的内存，重复释内存，内存泄漏，栈溢出）。和应用程序的segmentation fault（段错误）差不多。
