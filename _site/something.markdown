

+ linux执行命令的时候，会先去PATH中寻找命令所在的目录

+ HOME 记录了当前用户的根目录

+ SHELL 保存缺省Shell，默认是/bin/bash

+ USER 和 UID 是保存用户信息的环境变量，

+ USR 表示已经登录的用户的名字，UID 表示已经登录的用户的ID

+ PPID 是创建当前进程的进程号，即当前进程的父进程号

+ PS1 和 PS2

	( linux有PS1 PS2 PS3 PS4)  
	提示符变量，用于设置提示符格式  
	PS1用于设置一级shell提示符的环境变量，也称为主提示符字符串，即[root @jselab ~]#  
	like：[\u@\h \W]\$  
	\u 表示当前用户  
	\h 表示主机名  
	\W 表示当前目录名  
	\$ 则表示$号   

	echo $PS2 则显示 : >  

	提示符变量中的特殊符号及其意义  
	\d 以"周 月 日"格式来显示的日期的日期信息  
	\H 主机名和域名  host  
	\h 主机名 		 host  
	\s Shell的类型名称 shell  
	\T 以12小时制显示时间 格式为 HH:MM:SS time  
	\t 以24小时制显示时间 格式为 HH:MM:SS time  
	\@ 以12小时制显示时间 格式为 am/pm 12小时制显示时间  
	\u 当前的用户名		user  
	\v bash Shell的版本号 version  
	\V bash Shell的版本号和补丁号   
	\w 当前工作目录的完整路径 way  
	\W 当前工作目录名字  
	\# 当前命令的序列号 #sequence  
	\$ 如果UID为0 打印# 否则，打印$  

+ 几个重要的配置文件 

	- $HOME/.bash_profile 是最重要的配置文件，当某用户登录时，Shell会自动执行.bash_profile文件，当.bash_profile文件不存在时，则自动执行系统默认的配置文件/etc/profile

	- /etc/profile 全局用户文件，如果修改了此文件，修改的设置只会影响单个用户
	- ~/.bash_profile 用户的个人配置，如果修改此文中的设置，只会影响单个用户 
	- /etc/bashrc 全局变量配置文件，此文件中定义了所有用户的环境变量
	- ~/.bashrc 个人环境配置文件，此文件中定义了用户的环境变量

	eg:

		#/etc/profile: system-wide .profilefile for the Bourne shell (sh(1))
		#and Bourne compatible shells(bash(1), ksh(1), ash(1), ... )

		if ["$PS1"]; then
			if ["$BASH"] && ["BASH"!="/bin/sh"]; then
			# the file bash.bashrc already sets the default PS1 
			# PS1="\h:\W\$"
				if [-f /etc/bash.bashrc]; then
					./etc/bash.bashrc
				fi 
			else
				if ["`id -u`" -eq 0]; then
					PS1 = '#'
				else
					PS1 = '$'
				fi
			fi
		fi


		# The default umask is now handledby pam_umask
		# See pam_umask(8) and/etc/login.defs.

		if [-d /etc/profile.d]; then
			for i in /etc/profile.d/*.sh; do
				if [-r $i]; then
					.$i 
				fi
			done
			unset i
		fi

	export JAVA_HOME=/usr/lib/jvm/jdk1.7.4_04
	export JRE_HOME-${JAVA_HOME}/jre
	export CLASSPATH=$CLASSPATH::$JAVA_HOME/lib:$JAVA_HOME/jre/lib
	export CATALINA_HOME=/home/ming/prosoft/tomcat7
	export ANT_HOME=/home/ming/prosoft/apache-ant-1.8.4 
	export PATH=$PATH:/home/ming/prosoft/apache-1.8.4/bin

	如果要加入=的行立即生效,需要利用source命令执行.bash_profile文件   
	source命令也被称为点命令，即句点符号"." 和 source命令是等价的   
	eg:  
	. .bash_profile # 注意：句点符号和后面的文件用空格分开  

	子进程对变量的修改不会影响父进程，而父进程对变量的修改会影响子进程  

+ 位置参数是一种特俗的shell变量，Shell脚本传递参数
	$1 表示第一个参数，$2 表示第二个参数，等等。
	$0 表示脚本的名字，从${10}开始，参数需要括起来 ${11},${12},{$13} …

	$* 和 $@一样都表示从$1开始的全部参数。
	$# 命令行或者位置参数的个数 
	$* 所有的位置参数，被作为一个单词，注意”$*”必须被””引用
	$@ 与$*同义，每一个参数都被一个独立的””引用，注意”$@”必须被””引用

	$! 在后台工作的最后的工作的进程的PID
	$_ 保存之前执行的命令的最后一个参数 　 
	$? 命令或者脚本、函数的退出状态 用于检查上一个命令 0代表正常 非0代表不正常
	$$ 脚本自身的进程PID，

+ 引用时讲字符串用引用符号括起来，以防止特俗字符被Shell脚本重解释义，特俗字符是指除了字面意思之外还可以解释为其他意思的字符。

	双引号引用除 $ ` \  之外的所有字符，即美元符 反引号 反斜线在双引号内为特俗意义
	单引号引用了所有功能
	通常将单引号称为全引用，双引号称为部分引用，  

	反引号 `` 命令替换  
	执行被反引号括中的命令。  

	转义符  \  


+ shell中的数组 
  1) array = {1 2 3 4 5}
  2) array[0]=1; array[1]=2; …

  echo {array[1]} 显示某个元素  
  echo $array     默认引用第一个元素  
  echo ${array[@]:2} 查看数组中，下标大于等于2的所有元素  
  echo ${#array[@]} 数组元素中的个数  

  unset array[0]  清除数组中第一个元素  
  unset array     清除整个数组    


+ 间接变量引用   
   1) eval tempvar=\$$variable1  
   2) tempvar=${!variable1}  


 	    eg:
		#!/bin/bash 
		var1=var2
		var2=hadoop
		echo “var1=$var1”

		eval tempvar=\$$var1
		echo “tempvar=$tempvar”
		evho “var1=${!var1}”

		output:
		var1=var2
		tempvar=hadoop
		var1=hadoop



	
