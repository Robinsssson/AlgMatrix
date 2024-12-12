clear, clc;
close all;
%% 主程序调用示例
% 定义目标函数
obj_func = @(x) sum(x.^2); % Sphere函数

% 参数设置
pop_size = 50; % 种群大小
dim = 2; % 维度
bounds = [-5, -5; 5, 5]; % 搜索空间上下界
F = 0.8; % 缩放因子
CR = 0.9; % 交叉概率
max_gen = 100; % 最大迭代次数

% 初始化种群
[pop, fitness, bounds, F, CR, max_gen] = de_init(obj_func, pop_size, dim, bounds, F, CR, max_gen);

% 迭代过程
for gen = 1:max_gen
    [pop, fitness] = de_fresh(pop, fitness, obj_func, bounds, F, CR); % 执行一次迭代
end

% 绘制结果
de_plot(obj_func, pop, bounds);
