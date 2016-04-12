---
layout: post
title: "shell 循环"
date: 2015-12-12 12:30:00 +0800
categories: shell
---

    
    
    
    #!/bin/bash
    for var in 12 3 4 
    do
        echo $var
    done

    for file in $(ls ..)
    do
        echo "file: $file"
    done

    echo "the number of the arguments is $#"
    for argument
    do
        echo $argument
    done

    for (( i=1;i<=10; i++ ))
    do
        echo "$i time"
    done

    i=0
    sum=0
    while (( i < 10 ))
    do
        echo "i:$i, sum:$sum"
        let "sum+=$i"
        let "i++"
    done

    while [ "$*" != "" ]
    do
        echo "$1"
        shift
    done
#result: 
	<img src="/assets/2.png" alt="1.png" class="img-center" width="1544" height="1244"/>