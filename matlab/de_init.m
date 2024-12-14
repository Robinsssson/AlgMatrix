%% de_init: 初始化函数
function [pop, fitness, bounds, F, CR, max_gen] = de_init(obj_func, pop_size, dim, bounds, F, CR, max_gen)
    % 参数说明：
    % obj_func - 目标函数句柄
    % pop_size - 种群大小
    % dim - 维度
    % bounds - 搜索空间的上下界，格式：[lower_bound; upper_bound]
    % F - 缩放因子（通常取值范围为0.5到1）
    % CR - 交叉概率（通常取值范围为0到1）
    % max_gen - 最大迭代次数

    % 初始化种群在搜索空间内的随机位置
    lower_bound = bounds(1, :); % 获取下界
    upper_bound = bounds(2, :); % 获取上界
    pop = lower_bound + (upper_bound - lower_bound) .* rand(pop_size, dim); % 生成随机种群

    % 计算初始适应值
    fitness = arrayfun(@(idx) obj_func(pop(idx, :)), 1:pop_size); % 遍历种群计算每个个体的适应值
end
