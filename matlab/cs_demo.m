% Main script: cs_demo.m
function cs_demo()
    % Objective function: Modify as needed
    objective = @(x) sum(x.^2); % Example: Sphere function
    
    % Initialization parameters
    n = 20; % Population size
    dim = 5; % Dimensionality of the search space
    lb = -5 * ones(1, dim); % Lower bounds
    ub = 5 * ones(1, dim); % Upper bounds
    maxGen = 100; % Maximum generations
    pa = 0.25; % Discovery rate of alien eggs
    
    % Initialize population
    nests = cs_init(n, dim, lb, ub);
    fitness = arrayfun(@(i) objective(nests(i, :)), 1:n);
    
    % Run the algorithm
    [bestNest, bestFit, fitnessHistory] = cs_fresh(nests, fitness, objective, lb, ub, pa, maxGen);
    
    % Plot results
    cs_plot(fitnessHistory);
    
    % Output results
    fprintf('Best solution found: %s\n', mat2str(bestNest));
    fprintf('Best objective value: %f\n', bestFit);
end