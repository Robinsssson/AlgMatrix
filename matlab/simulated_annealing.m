function simulated_annealing()
    % 定义参数
    dim = 2; % 维度数
    max_iterations = 10000; % 最大迭代次数
    initial_temperature = 1000; % 初始温度
    cooling_rate = 0.99; % 温度下降速率
    lower_bound = -10; % 搜索空间下界
    upper_bound = 10; % 搜索空间上界

    % 初始化
    current_solution = lower_bound + rand(1, dim) * (upper_bound - lower_bound); % 初始解
    current_energy = test_function(current_solution); % 当前解的目标值
    best_solution = current_solution; % 最优解
    best_energy = current_energy; % 最优目标值
    temperature = initial_temperature;

    % 模拟退火过程
    for iteration = 1:max_iterations
        % 生成新解
        perturbation = (rand(1, dim) - 0.5) * 2; % 随机扰动
        new_solution = current_solution + perturbation;
        new_solution = min(max(new_solution, lower_bound), upper_bound); % 限制在边界内
        new_energy = test_function(new_solution);

        % 判断是否接受新解
        if new_energy < current_energy || ...
           exp((current_energy - new_energy) / temperature) > rand()
            current_solution = new_solution;
            current_energy = new_energy;
        end

        % 更新最优解
        if current_energy < best_energy
            best_solution = current_solution;
            best_energy = current_energy;
        end

        % 降温
        temperature = temperature * cooling_rate;

        % 打印当前状态
        fprintf('Iteration %d: Current Energy = %.5f, Best Energy = %.5f\n', ...
                iteration, current_energy, best_energy);
    end

    % 输出结果
    fprintf('Optimal Solution: x = [%s], f(x) = %.5f\n', ...
            num2str(best_solution, '%.5f '), best_energy);
end

% 测试函数（可以替换为你需要的高维函数）
function y = test_function(x)
    % 示例目标函数：Rastrigin函数
    % 定义为 f(x) = 10*n + sum(x_i^2 - 10*cos(2*pi*x_i))
    y = (x(1) - 1) ^ 2 + (x(2) - 2) ^ 2;
end
