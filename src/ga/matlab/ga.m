clear, clc;
close all;
% 参数设置
pop_size = 500;  % 种群大小
max_gen = 2000;  % 最大代数
mutation_rate = 0.1; % 变异率
crossover_rate = 0.8; % 交叉率
lb = -10 * ones(1, 2);  % 每个变量的下界 (x, y 的下界)
ub = 10 * ones(1, 2);   % 每个变量的上界 (x, y 的上界)
nvars = 2;  % 变量个数（对于此问题是x和y）

% 目标函数（即适应度函数）
fitness_function = @(x) x(1)^2 + x(2)^2 - 4*x(1) - 6*x(2) + 9;

% 初始化种群：每个个体是一个包含x和y的数组
population = lb + (ub - lb) .* rand(pop_size, nvars);

% 迭代过程
for gen = 1:max_gen
    % 评估适应度：每个个体的适应度即目标函数的值
    fitness_values = arrayfun(@(i) fitness_function(population(i, :)), 1:pop_size);
    
    % 选择：根据适应度选择父代
    [~, sorted_indices] = sort(fitness_values);
    population = population(sorted_indices, :);  % 按适应度排序
    num_parents = round(pop_size * crossover_rate);  % 选择父代的个体数量
    parents = population(1:num_parents, :);  % 选择最优秀的个体
    
    % 交叉：产生子代
    offspring = [];
    for i = 1:2:num_parents-1
        p1 = parents(i, :);
        p2 = parents(i+1, :);
        % 单点交叉
        crossover_point = randi(nvars);
        child1 = p1;
        child2 = p2;
        child1(crossover_point:end) = p2(crossover_point:end);
        child2(crossover_point:end) = p1(crossover_point:end);
        offspring = [offspring; child1; child2];
    end
    
    % 变异：变异操作，随机改变个体的一部分
    for i = 1:size(offspring, 1)
        if rand < mutation_rate  % 以一定概率进行变异
            mutation_point = randi(nvars);  % 随机选择一个变量位置
            mutation_value = lb(1) + (ub(1) - lb(1)) * rand;  % 随机生成变异值
            offspring(i, mutation_point) = mutation_value;  % 只修改变异点对应的变量
        end
    end
    
    % 更新种群：将父代和子代合并
    population = [parents; offspring];
    
    % 可视化：每代输出最佳解
    best_solution = population(1, :);
    best_fitness = fitness_function(best_solution);
    fprintf('Generation %d: Best solution = (%.4f, %.4f), Fitness = %.4f\n', gen, best_solution(1), best_solution(2), best_fitness);
end

% 最终最优解
best_solution = population(1, :);
best_fitness = fitness_function(best_solution);
fprintf('Final best solution = (%.4f, %.4f), Fitness = %.4f\n', best_solution(1), best_solution(2), best_fitness);
