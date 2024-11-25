add_toolchains("mingw")
add_requires("gsl")

target("algmath")
    set_kind("shared")
    add_files("algmath/random/alg_random.c")
    add_files("algmath/matrix/alg_matrix.c")
    add_files("algmath/vector/alg_vector.c")
    add_defines("ALG_EXPORT")

target("pso")
    set_kind("binary")
    add_files("pso/c/*.c")
    add_deps("algmath")
    

-- target("test-vector")
--     set_kind("binary")
--     add_files("algmath/vector/alg_vector_test.c")
--     add_deps("algmath")
