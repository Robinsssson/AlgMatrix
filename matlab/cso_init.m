function swarm = cso_init(pop_size, dim, llim, rlim, func)
    % CSO 初始化函数
    % 参数：
    %   pop_size: 粒子个数
    %   dim: 每个粒子的维度
    %   llim, rlim: 搜索空间的上下界
    %   func: 目标函数句柄
    
    % 粒子位置初始化
    swarm.position = llim + (rlim - llim) * rand(pop_size, dim);
    
    % 粒子速度初始化
    swarm.velocity = zeros(pop_size, dim);
    
    % 粒子适应度初始化
    swarm.fitness = arrayfun(@(idx) func(swarm.position(idx, :)), 1:pop_size)';
    
    % 群体信息
    swarm.best_position = swarm.position;
    swarm.best_fitness = swarm.fitness;
    [swarm.global_best_fitness, best_idx] = min(swarm.fitness);
    swarm.global_best_position = swarm.position(best_idx, :);
    
    % 其他参数
    swarm.pop_size = pop_size;
    swarm.dim = dim;
    swarm.llim = llim;
    swarm.rlim = rlim;
    swarm.func = func;
end
