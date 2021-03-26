# KConfig

### KConfig的基础语法
```
config TESTS
	bool "set version"
	depends MODULES
	help
		ok,this is test
```
- bool:布尔
- tristate:三态（内建，模块，移除）
- string:字符串
- hex:十六进制
- int:整形
