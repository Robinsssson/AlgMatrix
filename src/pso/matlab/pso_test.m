clear, clc;
local_pso_test()

function local_pso_test()
    % 参数设置
    number = 30; % 粒子数量
    dim = 2; % 维度
    w = 0.5; % 惯性权重
    c1 = 2; % 个人吸引因子
    c2 = 2; % 全局吸引因子
    min_position = -10; % 位置下界
    max_position = 10; % 位置上界
    min_speed = -1; % 速度下界
    max_speed = 1; % 速度上界
    type = 'max'; % 优化类型
    max_iter = 100; % 最大迭代次数

    % 目标函数（示例：简单的目标函数）
    func = @(x) -(x(:, 1).^2 + x(:, 2).^2); % 最小化x^2 + y^2，最大化则取负值

    % 初始化PSO结构
    pso_structure = pso_init(number, dim, w, c1, c2, func, type, min_position, max_position, min_speed, max_speed);

    % 迭代更新粒子群
    for iter = 1:max_iter
        pso_structure = pso_fresh(pso_structure);
        fprintf('迭代次数: %d, 全局最优适应度: %.4f, 全局最优位置: [%.4f, %.4f]\n', ...
                iter, pso_structure.g_fitness, pso_structure.g_position(1), pso_structure.g_position(2));
    end
end
