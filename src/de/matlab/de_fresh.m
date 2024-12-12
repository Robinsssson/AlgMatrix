%% de_fresh: 单次迭代算法
function [pop, fitness] = de_fresh(pop, fitness, obj_func, bounds, F, CR)
    % 参数说明：
    % pop - 当前种群
    % fitness - 当前种群的适应值
    % obj_func - 目标函数句柄
    % bounds - 搜索空间的上下界，格式：[lower_bound; upper_bound]
    % F - 缩放因子
    % CR - 交叉概率

    [pop_size, dim] = size(pop); % 获取种群大小和维度
    lower_bound = bounds(1, :); % 获取下界
    upper_bound = bounds(2, :); % 获取上界

    % 遍历种群中每个个体进行变异和交叉
    for i = 1:pop_size
        % 随机选择三个不同个体
        idxs = randperm(pop_size, 3); % 随机挑选三个不同的索引
        while any(idxs == i) % 确保所选个体不包含当前个体
            idxs = randperm(pop_size, 3);
        end
        x1 = pop(idxs(1), :); % 个体1
        x2 = pop(idxs(2), :); % 个体2
        x3 = pop(idxs(3), :); % 个体3

        % 变异操作
        v = x1 + F * (x2 - x3); % 按差分策略生成变异向量

        % 确保变异后的个体在边界内
        v = max(min(v, upper_bound), lower_bound); % 修正越界值

        % 交叉操作
        u = pop(i, :); % 初始化交叉向量
        j_rand = randi(dim); % 随机选择一个维度，保证至少有一个维度发生变化
        for j = 1:dim
            if rand <= CR || j == j_rand % 满足交叉概率条件
                u(j) = v(j); % 替换对应维度值
            end
        end

        % 选择操作
        u_fitness = obj_func(u); % 计算新向量的适应值
        if u_fitness < fitness(i) % 如果新向量优于当前个体
            pop(i, :) = u; % 更新种群
            fitness(i) = u_fitness; % 更新适应值
        end
    end
end