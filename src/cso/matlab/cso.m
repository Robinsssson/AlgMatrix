function cso_demo
    % 参数设置
    n = 30;                  % 鸡群总数
    maxGen = 100;            % 最大迭代次数
    dim = 10;                % 问题维度
    lb = -10;                % 下界
    ub = 10;                 % 上界
    mf = 5;                  % 母鸡数量
    rf = 15;                 % 公鸡数量
    numChicks = n - mf - rf; % 小鸡数量
    fnc = @(x) sum(x.^2);    % 目标函数（Sphere函数）

    % 初始化鸡群
    pos = lb + (ub - lb) * rand(n, dim); % 鸡群位置
    fitness = arrayfun(fnc, pos);        % 计算适应值

    % 角色分配
    [~, sortedIdx] = sort(fitness);
    roosters = sortedIdx(1:rf);            % 公鸡索引
    hens = sortedIdx(rf+1:rf+mf);          % 母鸡索引
    chicks = sortedIdx(rf+mf+1:end);       % 小鸡索引
    henToRooster = randi([1, rf], mf, 1);  % 母鸡跟随的公鸡

    % 优化过程
    for gen = 1:maxGen
        % 更新公鸡位置
        for i = 1:rf
            step = randn(1, dim) .* (1 ./ (fitness(roosters(i)) + 1e-10));
            newPos = pos(roosters(i), :) + step;
            pos(roosters(i), :) = clamp(newPos, lb, ub);
        end

        % 更新母鸡位置
        for i = 1:mf
            followIdx = roosters(henToRooster(i));
            step = rand(1, dim) .* (pos(followIdx, :) - pos(hens(i), :)) + ...
                   randn(1, dim) .* (1 ./ (fitness(hens(i)) + 1e-10));
            newPos = pos(hens(i), :) + step;
            pos(hens(i), :) = clamp(newPos, lb, ub);
        end

        % 更新小鸡位置
        for i = 1:numChicks
            motherIdx = hens(mod(i-1, mf) + 1); % 每只小鸡对应一个母鸡
            step = 0.5 * rand(1, dim) .* (pos(motherIdx, :) - pos(chicks(i), :));
            newPos = pos(chicks(i), :) + step;
            pos(chicks(i), :) = clamp(newPos, lb, ub);
        end

        % 重新计算适应值并重新分配角色
        fitness = arrayfun(fnc, pos);
        [~, sortedIdx] = sort(fitness);
        roosters = sortedIdx(1:rf);
        hens = sortedIdx(rf+1:rf+mf);
        chicks = sortedIdx(rf+mf+1:end);
        henToRooster = randi([1, rf], mf, 1);

        % 显示当前最优解
        fprintf('Generation %d: Best Fitness = %.6f\n', gen, fitness(sortedIdx(1)));
    end

    % 最优解输出
    bestPos = pos(sortedIdx(1), :);
    bestFitness = fitness(sortedIdx(1));
    fprintf('Optimal Solution Found: Best Fitness = %.6f\n', bestFitness);
    disp('Best Position:');
    disp(bestPos);
end

function bounded = clamp(pos, lb, ub)
    % 位置限制函数
    bounded = max(min(pos, ub), lb);
end
