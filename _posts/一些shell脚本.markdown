---
layout: post
title:	"һЩshell�ű�"
date:	2015-12-07 15:00:00 +0800
categories: shell
---

# Talk is cheap, show me the code!

## don't tested.


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
	
	
	shellһ���������͵ģ�����basn shell �ṩ��declare��typeset������������ָ������������
	������������ȫ�ȼ�
	
	declare [ѡ��] ������
	-r ����������Ϊֻ������
	-i ����������Ϊ������
	-a ����������Ϊ����
	-f ��ʾ�˽ű�ǰ����������к�������������
	-F ����ʾ�˽ű�ǰ����������к�����
	-x ����������Ϊ��������
	
	
	eg��
	#!/bin/bash
	var1=2012
	var2=$var1+1	#no declared�� so it's string.
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
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	