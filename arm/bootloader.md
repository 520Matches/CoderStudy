## BootLoader简单编写步骤

### 步骤总体
- 初始化硬件：关闭看门狗，设置时钟，设置SDRAM，初始化flash(nand flash或者其它)
- 如果bootloader比较大，要把它重定位到SDRAM
- 把内核从flash读到SDRAM中
- 设置要传递给内核的参数(把参数按照规定格式放在特定的地址)
- 跳转到内核并进行执
