clear, clc

syms x y z;  % 定义符号变量
A = [x-3,-7, 3; 2, x+5, -2; 4, 10, x-3];  % 创建一个2x2符号矩阵
d = det(A);  % 求行列式的符号表达式
disp(d);
