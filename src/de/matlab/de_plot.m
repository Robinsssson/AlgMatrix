%% de_plot: 绘制结果
function de_plot(obj_func, pop, bounds)
    % 参数说明：
    % obj_func - 目标函数句柄
    % pop - 最终种群
    % bounds - 搜索空间的上下界，格式：[lower_bound; upper_bound]

    % 构建目标函数的网格数据
    [X, Y] = meshgrid(linspace(bounds(1, 1), bounds(2, 1), 100), linspace(bounds(1, 2), bounds(2, 2), 100));
    Z = arrayfun(@(x, y) obj_func([x, y]), X, Y); % 计算网格点处的目标函数值

    % 绘制目标函数
    figure;
    contourf(X, Y, Z, 50, 'LineColor', 'none'); % 绘制等高线图
    colorbar;
    hold on;

    % 绘制种群
    scatter(pop(:, 1), pop(:, 2), 50, 'r', 'filled'); % 标记种群点
    title('Differential Evolution Result'); % 添加标题
    xlabel('X'); % X轴标签
    ylabel('Y'); % Y轴标签
    hold off;
end