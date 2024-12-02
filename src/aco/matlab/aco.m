% 蚁群算法求解旅行商问题（TSP）
clc;
clear;
close all;

% 1. 初始化
N = 5; % 城市数
city_coords = rand(N, 2); % 每个城市的x, y坐标，随机生成

% 计算城市间的欧几里得距离
dist = zeros(N, N); % 初始化距离矩阵
for i = 1:N
    for j = 1:N
        dist(i, j) = sqrt((city_coords(i, 1) - city_coords(j, 1))^2 + (city_coords(i, 2) - city_coords(j, 2))^2);
    end
end

% 蚁群算法参数设置
num_ants = 200; % 蚂蚁数目
alpha = 1; % 信息素重要程度因子
beta = 3; % 启发函数重要程度因子
rho = 0.05; % 信息素挥发系数
Q = 100; % 常数
max_iter = 100; % 最大迭代次数

% 初始化信息素矩阵
pheromone = ones(N, N); % 信息素矩阵，初始为均匀分布
best_path = []; % 最优路径
best_length = inf; % 最优路径长度

% 2. 主循环
for iter = 1:max_iter
    paths = zeros(num_ants, N); % 记录每个蚂蚁的路径
    path_lengths = zeros(num_ants, 1); % 记录每个蚂蚁的路径长度

    % 每个蚂蚁执行一次搜索
    for k = 1:num_ants
        visited = false(1, N); % 标记每个城市是否被访问
        city = randi(N); % 随机选择一个城市作为起点
        visited(city) = true;
        paths(k, 1) = city; % 蚂蚁的第一个城市

        % 选择路径
        for step = 2:N
            % 计算选择概率
            probabilities = zeros(1, N);
            for i = 1:N
                if ~visited(i)
                    probabilities(i) = (pheromone(city, i)^alpha) * ((1/dist(city, i))^beta);
                end
            end
            probabilities = probabilities / sum(probabilities); % 归一化

            % 选择下一个城市（轮盘赌选择）
            next_city = roulette_wheel_selection(probabilities);
            visited(next_city) = true;
            paths(k, step) = next_city; % 更新路径
            city = next_city; % 移动到下一个城市
        end

        % 计算路径长度
        path_length = 0;
        for i = 1:N-1
            path_length = path_length + dist(paths(k, i), paths(k, i+1));
        end
        path_length = path_length + dist(paths(k, N), paths(k, 1)); % 回到起点
        path_lengths(k) = path_length;

        % 更新全局最优解
        if path_length < best_length
            best_length = path_length;
            best_path = paths(k, :);
        end
    end

    % 更新信息素
    pheromone = (1 - rho) * pheromone; % 信息素挥发

    % 强化最优路径信息素
    for k = 1:num_ants
        for i = 1:N-1
            pheromone(paths(k, i), paths(k, i+1)) = pheromone(paths(k, i), paths(k, i+1)) + Q / path_lengths(k);
        end
        pheromone(paths(k, N), paths(k, 1)) = pheromone(paths(k, N), paths(k, 1)) + Q / path_lengths(k); % 回到起点
    end

    fprintf('Iteration %d: Best Length = %.2f\n', iter, best_length);
end

% 输出最优路径
disp('Best Path:');
disp(best_path);

% 绘制城市位置和路径
figure;
plot(city_coords(:, 1), city_coords(:, 2), 'ro'); % 绘制城市位置
hold on;

% 绘制最佳路径
for i = 1:N-1
    plot([city_coords(best_path(i), 1), city_coords(best_path(i+1), 1)], ...
         [city_coords(best_path(i), 2), city_coords(best_path(i+1), 2)], 'b-');
end
plot([city_coords(best_path(N), 1), city_coords(best_path(1), 1)], ...
     [city_coords(best_path(N), 2), city_coords(best_path(1), 2)], 'b-'); % 回到起点
title('Best Path Found by Ant Colony Algorithm');
hold off;

% 轮盘赌选择函数
function selected = roulette_wheel_selection(probabilities)
    cumulative_prob = cumsum(probabilities);
    r = rand();
    selected = find(cumulative_prob >= r, 1);
end
