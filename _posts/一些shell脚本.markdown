---
layout: post
title:	"一些shell脚本"
date:	2015-12-07 15:00:00 +0800
categories: shell
---

# Talk is cheap, show me the code!

## don't tested.

** 1 **

	#!/bin/bash
	a=2009
	let "a+=1"
	echo "a=$a"
	b = xx09
	echo "b=$b"
	declare -i b     ##declare b as a integer
	echo "b=$b"
	let "b+=1"       ## b equal 1, when b is declared a integer it becomes 0.
	echo "b=$b"
	exit 0
	

	result:
	a=2010
	b=xx09
	b=xx09
	b=1
	

** 2 **


    shell 有两种类型，字符，整型数字
    字符默认是 空 “”
    数字默认是 0
    
	#!/bin/bash
	c=""
	echo "c=$c"
	let "c+=1"
	echo "c=$c"
	echo "e=$e"
	let "e+=1"
	echo "e=$e"
	ecit
	
	result:
	c=
	c=1
	e=
	e=1 
	

** 3 **
	
	shell变量一般是无类型的，但是bash shell提供了declare和typeset两个命令来制定变量的类型，这两个命令完全等价。
	declare [选项] 变量
	-r 声明为只读变量
	-i 声明为整形变量
	-a 将变量声明为数组
	-f 显示此脚本前定义的所有函数名及其内容
	-F 仅显示此脚本前定义过的所有函数名
	-x 将变量声明为环境变量
	
	eg
	#!/bin/bash
	var1=2012
	var2=$var1+1	#no declared£¬ so it's string.
	echo "var2=$var2"
	
	let "var3=$var1+1"	# let functioned.
	echo "var3=$var3"
	
	declare -i var4 #declare as a integer
	var4=$var1+1
	echo "var4=$var4"
	
	
	output:
	var2=2012+1
	var3=2013
	var4=2013
	
	
	#!/bin/bash
	## let equal to (())
	
	var1=9
	var2=10
	
	var3=$((var1*var2)) # equal as (($var1*$var2))
	echo $var3
	
	
** 4 **
	
	
	环境变量
	ENVIRON-VARIABLE=value  #环境变量赋值
	export ENVIRON-VARIABLE #声明环境变量
	消除环境变量是unset
	
	env可以列出已经定义的环境变量
	
	pwd是当前的目录路径 
	OLDPWD是旧的工作目录
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	