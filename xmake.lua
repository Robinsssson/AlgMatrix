add_toolchains("mingw")
add_requires("gsl")

target("algmath")
    set_kind("shared")
    add_files("algmath/*/*.c")
    add_defines("ALG_EXPORT")

target("pso")
    set_kind("binary")
    add_files("pso/c/*.c")
    add_deps("algmath")
    add_includedirs("algmath")
    

target("test-matrix")
    set_kind("binary")
    add_files("test/alg_matrix_test.c")
    add_files("test/test_framework.c")
    add_includedirs("algmath")
    add_deps("algmath")

target("test-vector")
    set_kind("binary")
    add_files("test/alg_vector_test.c")
    add_files("test/test_framework.c")
    add_includedirs("algmath")
    add_deps("algmath")
