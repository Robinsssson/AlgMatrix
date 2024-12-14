% Function: Plot fitness history
function cs_plot(fitnessHistory)
    figure;
    plot(fitnessHistory, 'LineWidth', 2);
    title('Convergence of Cuckoo Search');
    xlabel('Generation');
    ylabel('Best Fitness');
    grid on;
end
