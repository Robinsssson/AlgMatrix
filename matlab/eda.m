function eda_example
    % 参数设置
    numGenerations = 50;  % 最大代数
    populationSize = 100; % 种群大小
    problemDim = 2;       % 问题维度
    bounds = [-5, 5];     % 变量的上下界
    elitismRate = 0.2;    % 精英个体的比例
    
    % 目标函数（示例：Sphere函数，目标是最小化）
    objective = @(x) sum(x.^2, 2);
    
    % 初始化种群（均匀随机分布）
    population = bounds(1) + (bounds(2) - bounds(1)) * rand(populationSize, problemDim);
    
    % 迭代优化
    for gen = 1:numGenerations
        % 评估适应度
        fitness = objective(population);
        
        % 按适应度排序并选择精英
        [~, sortedIndices] = sort(fitness);
        eliteCount = round(elitismRate * populationSize);
        elites = population(sortedIndices(1:eliteCount), :);
        
        % 估计概率分布（正态分布的均值和方差）
        meanVec = mean(elites, 1);
        stdVec = std(elites, 0, 1);
        
        % 生成新种群
        population = randn(populationSize, problemDim) .* stdVec + meanVec;
        
        % 保证新种群在边界内
        population = max(min(population, bounds(2)), bounds(1));
        
        % 打印当前代的信息
        bestFitness = min(fitness);
        disp(['Generation ', num2str(gen), ': Best Fitness = ', num2str(bestFitness)]);
    end
end
