% ABC算法的MATLAB实现
clear, clc
% 参数设置
SN = 5000;                 % 蜂群大小
max_iter = 1000;          % 最大迭代次数
dim = 2;                 % 解的维度
lb = -5;                 % 每个维度的下界
ub = 5;                  % 每个维度的上界
limit = 50;              % 每个食物源的限制次数

% 初始化食物源位置（解）和适应度
X = lb + (ub - lb) * rand(SN, dim);  % 蜂群中SN个解
fitness = arrayfun(@(i) objective_function(X(i, :)), 1:SN); % 每个解的适应度

% 初始化蜜蜂的状态
trial = zeros(SN, 1);   % 每个解的尝试次数

% 迭代过程
for iter = 1:max_iter
    % 在职蜜蜂阶段
    for i = 1:SN/2
        % 选择一个随机解 X_j
        j = randi([1, SN]);
        while j == i
            j = randi([1, SN]);
        end

        % 随机选择一个维度 k
        k = randi([1, dim]);

        % 生成新解 V_i
        Phi = 2 * rand() - 1;  % [-1, 1] 区间
        V = X(i, :);
        V(k) = X(i, k) + Phi * (X(i, k) - X(j, k));

        % 保证新解在边界内
        V(k) = max(min(V(k), ub), lb);

        % 计算新解的适应度并进行贪婪选择
        new_fitness = objective_function(V);
        if new_fitness < fitness(i)
            X(i, :) = V;
            fitness(i) = new_fitness;
            trial(i) = 0;  % 重置尝试次数
        else
            trial(i) = trial(i) + 1;
        end
    end

    % 旁观蜜蜂阶段
    total_fitness = sum(fitness);
    prob = fitness / total_fitness;  % 计算每个食物源被选择的概率

    for i = SN/2 + 1:SN
        % 选择一个食物源
        selected = roulette_wheel_selection(prob);

        % 随机选择一个维度 k
        k = randi([1, dim]);

        % 生成新解 V_i
        Phi = 2 * rand() - 1;
        V = X(i, :);
        V(k) = X(i, k) + Phi * (X(i, k) - X(selected, k));

        % 保证新解在边界内
        V(k) = max(min(V(k), ub), lb);

        % 计算新解的适应度并进行贪婪选择
        new_fitness = objective_function(V);
        if new_fitness < fitness(i)
            X(i, :) = V;
            fitness(i) = new_fitness;
            trial(i) = 0;
        else
            trial(i) = trial(i) + 1;
        end
    end

    % 侦查蜜蜂阶段
    for i = 1:SN
        if trial(i) > limit
            % 重新初始化被遗弃的解
            X(i, :) = lb + (ub - lb) * rand(1, dim);
            fitness(i) = objective_function(X(i, :));
            trial(i) = 0;
        end
    end

    % 显示当前最佳解的适应度
    [best_fitness, best_index] = min(fitness);
    fprintf('Iteration %d: Best Fitness = %f\n', iter, best_fitness);
end
fprintf("best index is %d", best_index);
disp(X(best_index, :))

% 目标函数（例如，Rosenbrock函数）
function fit = objective_function(x)
    fit = sum(100 * (x(2:end) - x(1:end-1).^2).^2 + (1 - x(1:end-1)).^2);  % Rosenbrock
end

% 轮盘赌选择
function selected = roulette_wheel_selection(prob)
r = rand();
cumulative_prob = cumsum(prob);
selected = find(cumulative_prob >= r, 1);
end
