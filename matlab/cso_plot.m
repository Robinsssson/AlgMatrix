function cso_plot(swarm, iteration)
    % 绘制CSO搜索状态
    % 参数：
    %   swarm: 粒子群状态结构体
    %   iteration: 当前迭代次数
    
    scatter(swarm.position(:, 1), swarm.position(:, 2), 'filled');
    hold on;
    scatter(swarm.global_best_position(1), swarm.global_best_position(2), 100, 'r', 'filled');
    title(['Iteration ', num2str(iteration)]);
    xlabel('x');
    ylabel('y');
    legend('Particles', 'Global Best', 'Location', 'Best');
    grid on;
    hold off;
end
