---
layout: post
title: "Debug小结"
date: 2016-1-4 10:00:00 +0800
categories: debug
---

break 设置断点 
	break function  设置断点的函数
	break file:19	设置在某个文件的某行断点
break XXX if XXX 设置条件断点 

info break( info b ) 查看断点的信息

r (run) 运行程序 

n (next) 单条语句执行 

c (continue) 继续运行程序，直到遇到下一个断点

p i  	打印变量i的值 print命令的简写 

bt (backtrace) 查看堆栈

f <n> frame的简写 可以查看地n层堆栈的详细信息
info frame / info f 打印更为详细的当前栈层的信息。

q  (quit) 退出debug

finish 退出当前函数 并打印函数返回时的堆栈地址和返回值以及参数值等信息

u until 当你厌倦了在一个循环体内单步跟踪时 这个命令可以运行程序直到退出循环体

s (step) 单步步入 

si (stepi)
ni (nexti) 
单步跟踪一条机器指令! 


info args 打印当前函数的参数名及其值
info localsd 打印当前函数中所有局部变量及其值
info catch 打印当前函数中的异常处理信息

指定源文件的路径 
在某些时候，用-g编译过后的执行程序中只是包括了源文件的名字，没有路径名。
GDB提供了可以让你指定源文件路径的命令，以便GDB进行搜索。

directory <dirname ... >
dir <dirname ... >

查看内存 
x (examine) 
x/<n/f/u> <addr>
n 是一个正整数，表示显示内存的长度，也就是说从当前地址向后显示几个地址的内容
f 表示显示的格式
u 表示从前向后请求的字节数，如果不请求的话，GDB默认是4个bytes,u可以用下面的字符来代替，b表示单字节，h表示双字节，w表示四字节，g表示八字节/

x/3uh 0x54320 表示从内存地址0x54320读取内容 h表示以双自己为一个单位，3表示显示3个单位，u表示按16进制显示。



