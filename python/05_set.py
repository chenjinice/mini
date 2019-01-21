#!/usr/bin/python3


a = {1,2,3,4}
b = {5,6,7,8}
c = set("12345678901234567890")
d = set() # 创建空集合不能用 {} ，{}是创建空字典用的

print("c = ",c)
print("a+b = ", a | b)
print("a-b = ", a - b)
print("a^b = ", a ^ b)
