---
layout: post
title: Shell脚本编写函数
date: 2015-12-10 23:45:00 +0800
categories: shell
---
   shell脚本函数命名规则:  

+ 都由小写字母和下划线组成，并以字母开头  
+ 不要使用命令作为函数名称 
+ 不要在函数名中使用特殊字符   
    
    
    	#!/bin/bash
   		# 等号两边不能有空格
    	# 判断语句里面要有空格

    	function hello()
    	{
        	echo "Hello World!"
    	}

    	hello    #调用函数

    	directory()
    	{
        let "filenum = 0"
        let "dirnum = 0"

        ls
        echo ""

        for file in $( ls )
        do
            if [ -d $file ]; then
                let "dirnum += 1"
            else
                let "filenum += 1"
            fi
        done

        echo "the number of file is $filenum"
        echo "the number of dirnum is $dirnum"
    	}

    	directory   # 文件数 文件夹数

    	function a()
    	{
       	  	echo $1
    	}

    	a "Hello World!"    #向函数传递参数

    	half()
    	{
         	let "n = $1"
         	let "n /= 2"
         	echo "The half is $n"
    	}

    	let "m = $1"
    	echo "Before half function m is $m"
    	half $m                 # 传递型参
    	echo "After half function m is $m"

    	let "d=0"
    	function returnByVar()
    	{
         	d=$1
    	}

    	returnByVar "Hello World!(on function returnByVar)!"

    	echo d=$d

    	text="Global Var" #全局变量
    	function fn()
    	{
         	local text="Local Var!" #局部变量
         	echo $text
    	}

    	fn
    	echo "out of function text=$text"

    	fabnaci()
    	{
        	let "c=$1"
        	if [ $c -le 0 ]; then
             	return 0
       	 	elif [ $c -eq 1 ]; then
            	return 1
        	elif [ $c -eq 2 ]; then
            	return 1
        	else
             	local ans=0
             	let "arg=$1-1"
             	fabnaci $arg
             	let "ans+=$?"
             	let "arg=$1-2"
             	fabnaci $arg
             	let "ans+=$?"
             	return $ans
         	fi
    	}

    	fabnaci 10
    	echo $?     #上次函数调用的返回值


### Result:

<img src="/assets/shell-1.png" alt="shell-1" class="img-center" width="1154" height="522"/>

### 参考：

<http://blog.csdn.net/mdx20072419/article/details/9381339>