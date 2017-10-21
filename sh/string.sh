#!/bin/bash

my_name="chen jin"

#单引号原封不动输出
string1='hello world ! \" ${my_name}.'
#双引号解释转义字符和变量
string2="hello world ! \" ${my_name}."
#拼接字符串
string3="hello world ! \" "${my_name}"."

echo ${string1}
echo $string2
echo "$string3"
echo "${#my_name}"
echo "${my_name:5:7}"
#反引号内执行命令，查找h的位置
echo `expr index "$string1" h`

