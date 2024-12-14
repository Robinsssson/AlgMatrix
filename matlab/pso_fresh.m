function pso_structure = pso_fresh(pso_structure)
    fitness = pso_structure.func(pso_structure.position);
    if pso_structure.type == "max"
        mask = pso_structure.p_fitness < fitness;
        pso_structure.p_fitness(mask) = fitness(mask);
        pso_structure.p_position(mask, :) = pso_structure.position(mask, :);
        [pso_structure.g_fitness, pso_structure.g_idx] = max(pso_structure.p_fitness);
    else
        mask = pso_structure.p_fitness > fitness;
        pso_structure.p_fitness(mask) = fitness(mask);
        pso_structure.p_position(mask, :) = pso_structure.position(mask, :);
        [pso_structure.g_fitness, pso_structure.g_idx] = min(pso_structure.p_fitness);
    end
    pso_structure.g_position = pso_structure.position(pso_structure.g_idx, :);

    r1 = rand(pso_structure.number, 1); % 更新随机数生成方式
    r2 = rand(pso_structure.number, 1);

    % 更新速度
    pso_structure.speed = pso_structure.w .* pso_structure.speed + ...
                         pso_structure.c1 .* r1 .* (pso_structure.p_position - pso_structure.position) + ...
                         pso_structure.c2 .* r2 .* (pso_structure.g_position - pso_structure.position);

    % 应用速度限制
    pso_structure.speed = limit_speed(pso_structure.speed, pso_structure.min_speed, pso_structure.max_speed);

    % 更新位置
    pso_structure.position = pso_structure.position + pso_structure.speed;

    % 应用位置限制
    pso_structure.position = limit_position(pso_structure.position, pso_structure.min_position, pso_structure.max_position);
end

function limited_speed = limit_speed(speed, min_speed, max_speed)
    limited_speed = min(max(speed, min_speed), max_speed);
end

function limited_position = limit_position(position, min_position, max_position)
    limited_position = min(max(position, min_position), max_position);
end
