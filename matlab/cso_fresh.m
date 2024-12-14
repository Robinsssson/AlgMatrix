function swarm = cso_fresh(swarm)
    % CSO 更新函数
    % 参数：
    %   swarm: 粒子群状态结构体
    
    % 随机生成更新参数
    alpha = 2; % Leader与Follower的影响因子
    beta = 0.5; % 局部扰动因子
    
    for i = 1:swarm.pop_size
        % 获取当前粒子信息
        pos = swarm.position(i, :);
        
        % 选择随机Leader和随机Follower
        leader_idx = randi(swarm.pop_size);
        follower_idx = randi(swarm.pop_size);
        
        % 更新速度和位置
        swarm.velocity(i, :) = alpha * rand(1, swarm.dim) .* (swarm.position(leader_idx, :) - pos) ...
                             + beta * rand(1, swarm.dim) .* (swarm.position(follower_idx, :) - pos);
        swarm.position(i, :) = pos + swarm.velocity(i, :);
        
        % 边界处理
        swarm.position(i, :) = max(swarm.llim, min(swarm.position(i, :), swarm.rlim));
        
        % 更新适应度
        swarm.fitness(i) = swarm.func(swarm.position(i, :));
    end
    
    % 更新全局最优解
    [current_best_fitness, best_idx] = min(swarm.fitness);
    if current_best_fitness < swarm.global_best_fitness
        swarm.global_best_fitness = current_best_fitness;
        swarm.global_best_position = swarm.position(best_idx, :);
    end
end
