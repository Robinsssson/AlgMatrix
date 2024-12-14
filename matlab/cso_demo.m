function cso_demo()
    % CSO 测试函数

    % 定义测试函数
    func = @(x) sum(x.^2);
    
    % 初始化
    pop_size = 30;
    dim = 2;
    llim = -10;
    rlim = 10;
    max_iter = 100;
    swarm = cso_init(pop_size, dim, llim, rlim, func);
    
    % 迭代
    for iter = 1:max_iter
        swarm = cso_fresh(swarm);
        if mod(iter, 10) == 0
            cso_plot(swarm, iter);
            pause(0.1);
        end
    end
    
    % 输出结果
    fprintf('Optimal Value: %.6f\n', swarm.global_best_fitness);
    fprintf('Optimal Position: [%s]\n', num2str(swarm.global_best_position));
end
