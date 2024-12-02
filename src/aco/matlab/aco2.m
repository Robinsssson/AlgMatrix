clc;
clear;
close all;

% 目标函数 (二维)
f = @(x, y) x.^2 + y.^2 + 10 * sin(x) .* sin(y);  % f(x, y) = x^2 + y^2 + 10 * sin(x) * sin(y)

% 参数设置
N = 100;             % 蚂蚁数量
max_iter = 1000;    % 最大迭代次数
alpha = 1;          % 信息素的重要性因子
beta = 2;           % 启发函数的重要性因子
rho = 0.1;          % 信息素挥发系数
Q = 10;             % 信息素增量

% 搜索空间
x_min = -10;        % x的搜索空间下界
x_max = 10;         % x的搜索空间上界
y_min = -10;        % y的搜索空间下界
y_max = 10;         % y的搜索空间上界

% 初始化信息素矩阵
pheromone_x = ones(1, N) * 1e-1; % x维度的信息素
pheromone_y = ones(1, N) * 1e-1; % y维度的信息素

% 最优解初始化
best_value = inf;
best_position = NaN(1, 2);

% 蚂蚁位置初始化 (蚂蚁的初始位置在搜索空间内随机分布)
positions = [x_min + (x_max - x_min) * rand(N, 1), ...
             y_min + (y_max - y_min) * rand(N, 1)];

% 2. 主循环
for iter = 1:max_iter
    % 每个蚂蚁计算目标函数值
    values = f(positions(:, 1), positions(:, 2));
    
    % 记录全局最优解
    [min_value, min_idx] = min(values);
    if min_value < best_value
        best_value = min_value;
        best_position = positions(min_idx, :);
    end
    
    % 更新信息素
    pheromone_x = (1 - rho) * pheromone_x; % x维度信息素挥发
    pheromone_y = (1 - rho) * pheromone_y; % y维度信息素挥发
    
    for i = 1:N
        pheromone_x(i) = pheromone_x(i) + Q / values(i); % 通过目标函数值更新x维度信息素
        pheromone_y(i) = pheromone_y(i) + Q / values(i); % 通过目标函数值更新y维度信息素
    end
    
    % 蚂蚁位置更新
    new_positions = zeros(N, 2); % 存储新的蚂蚁位置
    for i = 1:N
        % 计算x维度和y维度的选择概率
        prob_x = (pheromone_x .^ alpha) .* ((1 ./ f(positions(:, 1), positions(:, 2))) .^ beta);
        prob_y = (pheromone_y .^ alpha) .* ((1 ./ f(positions(:, 1), positions(:, 2))) .^ beta);
        
        % 对概率进行归一化
        prob_x = prob_x / sum(prob_x);
        prob_y = prob_y / sum(prob_y);
        
        % 轮盘赌选择x维度和y维度的值
        r_x = rand();
        r_y = rand();
        
        cumulative_prob_x = cumsum(prob_x);
        cumulative_prob_y = cumsum(prob_y);
        
        new_positions(i, 1) = positions(find(cumulative_prob_x >= r_x, 1), 1); % 选择新的x值
        new_positions(i, 2) = positions(find(cumulative_prob_y >= r_y, 1), 2); % 选择新的y值
    end
    
    positions = new_positions; % 更新蚂蚁位置

    % 输出当前迭代的最优解
    fprintf('Iteration %d: Best Value = %.4f, Best Position = (%.4f, %.4f)\n', iter, best_value, best_position(1), best_position(2));
end

% 输出最终最优解
disp('Best Solution:');
disp(best_position);
disp('Best Function Value:');
disp(best_value);

% 绘制目标函数的轮廓图
[x_grid, y_grid] = meshgrid(linspace(x_min, x_max, 100), linspace(y_min, y_max, 100));
z_grid = f(x_grid, y_grid);
figure;
contour(x_grid, y_grid, z_grid, 50); % 绘制等高线
hold on;
plot(best_position(1), best_position(2), 'ro'); % 绘制最优解
title('Best Solution Found by Ant Colony Algorithm');
xlabel('x');
ylabel('y');
