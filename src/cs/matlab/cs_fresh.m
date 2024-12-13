function [bestNest, bestFit, fitnessHistory] = cs_fresh(nests, fitness, objective, lb, ub, pa, maxGen)
    n = size(nests, 1);
    dim = size(nests, 2);
    fitnessHistory = zeros(1, maxGen);
    
    for gen = 1:maxGen
        % Generate new solutions by Lévy flight
        for i = 1:n
            newSolution = nests(i, :) + levyFlight(dim);
            newSolution = boundSolution(newSolution, lb, ub);
            newFitness = objective(newSolution);
            if newFitness < fitness(i)
                nests(i, :) = newSolution;
                fitness(i) = newFitness;
            end
        end
        
        % Replace worst nests with new ones (abandon some nests)
        [~, worstIdx] = sort(fitness, 'descend');
        abandoned = rand(n, 1) < pa;
        for i = 1:n
            if abandoned(i)
                nests(worstIdx(i), :) = cs_init(1, dim, lb, ub);
                fitness(worstIdx(i)) = objective(nests(worstIdx(i), :));
            end
        end
        
        % Update best solution
        [bestFit, bestIdx] = min(fitness);
        bestNest = nests(bestIdx, :);
        
        % Record fitness history
        fitnessHistory(gen) = bestFit;
        fprintf('Generation %d: Best fitness = %f\n', gen, bestFit);
    end
end

% Function: Lévy flight
function step = levyFlight(dim)
    beta = 1.5; % Lévy distribution parameter
    sigma = (gamma(1 + beta) * sin(pi * beta / 2) / ...
            (gamma((1 + beta) / 2) * beta * 2^((beta - 1) / 2)))^(1 / beta);
    u = randn(1, dim) * sigma;
    v = randn(1, dim);
    step = u ./ abs(v).^(1 / beta);
end

% Function: Ensure solution stays within bounds
function solution = boundSolution(solution, lb, ub)
    solution = max(solution, lb);
    solution = min(solution, ub);
end