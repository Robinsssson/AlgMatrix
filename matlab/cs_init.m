function nests = cs_init(n, dim, lb, ub)
    nests = lb + rand(n, dim) .* (ub - lb);
end
