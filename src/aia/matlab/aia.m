function immune_algorithm()
    % 参数设置
    num_antibodies = 50;       % 抗体数量
    clone_factor = 5;          % 克隆倍数
    max_generations = 100;     % 最大代数
    mutation_rate = 0.1;       % 变异率
    dimension = 2;             % 问题维度
    search_range = [-5, 5];    % 搜索范围
    convergence_threshold = 1e-6; % 收敛阈值

    % 初始化抗体
    antibodies = initialize_population(num_antibodies, dimension, search_range);
    best_fitness = inf;
    
    for generation = 1:max_generations
        % 计算适应度
        fitness = evaluate_fitness(antibodies, @test_function);
        
        % 更新最优解
        [current_best_fitness, best_idx] = min(fitness);
        if current_best_fitness < best_fitness
            best_fitness = current_best_fitness;
            best_antibody = antibodies(best_idx, :);
        end
        
        % 打印进度
        fprintf('Generation %d: Best Fitness = %.6f\n', generation, best_fitness);
        
        % 收敛检查
        if best_fitness < convergence_threshold
            disp('Convergence achieved.');
            break;
        end
        
        % 克隆选择
        clones = clone_and_mutate(antibodies, fitness, clone_factor, mutation_rate, search_range);
        
        % 更新抗体池
        antibodies = select_new_population(antibodies, clones, @test_function, num_antibodies);
    end
    
    % 输出结果
    fprintf('Optimal solution found: x = [%s], f(x) = %.6f\n', ...
        num2str(best_antibody), best_fitness);
end

function population = initialize_population(pop_size, dimension, range)
    population = rand(pop_size, dimension) * (range(2) - range(1)) + range(1);
end

function fitness = evaluate_fitness(population, fitness_function)
    fitness = arrayfun(@(i) fitness_function(population(i, :)), 1:size(population, 1))';
end

function clones = clone_and_mutate(antibodies, fitness, clone_factor, mutation_rate, range)
    % 按适应度选择克隆数量
    num_clones = ceil(clone_factor * (1 ./ fitness / sum(1 ./ fitness)) * size(antibodies, 1));
    clones = [];
    for i = 1:length(num_clones)
        % 克隆并变异
        for j = 1:num_clones(i)
            clone = antibodies(i, :) + mutation_rate * (rand(size(antibodies, 2), 1) - 0.5)' * (range(2) - range(1));
            clone = max(min(clone, range(2)), range(1)); % 确保在范围内
            clones = [clones; clone];
        end
    end
end

function new_population = select_new_population(antibodies, clones, fitness_function, pop_size)
    combined_population = [antibodies; clones];
    fitness = evaluate_fitness(combined_population, fitness_function);
    [~, sorted_indices] = sort(fitness);
    new_population = combined_population(sorted_indices(1:pop_size), :);
end

function ret_val = test_function(x)
    ret_val = sum(100 * (x(2:end) - x(1:end-1).^2).^2 + (1 - x(1:end-1)).^2); % Rosenbrock
end
