function [pso_structure] = pso_init(number, dim, w, c1, c2, func, type, min_position, max_position, min_speed, max_speed)
    speed = rand(number, dim) * (max_speed - min_speed) + min_speed; % 初始化速度
    position = rand(number, dim) * (max_position - min_position) + min_position; % 初始化位置
    fitness = func(position); % 计算初始适应度
    p_fitness = fitness; % 保存初始适应度

    if strcmp(type, 'max')
        [g_fitness, g_idx] = max(p_fitness);
    else
        [g_fitness, g_idx] = min(p_fitness);
    end

    p_position = position; % 个人最优位置
    g_position = position(g_idx, :); % 全局最优位置

    % 将结构体的属性赋值
    pso_structure.speed = speed;
    pso_structure.position = position;
    pso_structure.fitness = fitness;
    pso_structure.p_fitness = p_fitness;
    pso_structure.type = type;
    pso_structure.p_position = p_position;
    pso_structure.g_position = g_position;
    pso_structure.g_idx = g_idx;
    pso_structure.g_fitness = g_fitness;
    pso_structure.number = number;
    pso_structure.dim = dim;
    pso_structure.w = w;
    pso_structure.c1 = c1;
    pso_structure.c2 = c2;
    pso_structure.func = func;
    pso_structure.min_position = min_position;
    pso_structure.max_position = max_position;
    pso_structure.min_speed = min_speed;
    pso_structure.max_speed = max_speed;
end
