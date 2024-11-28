target("algmath")
    set_kind("shared")
    add_files("**.c|test/*.c")
    add_defines("ALG_EXPORT")
    if is_plat("windows") then
        set_toolchains("mingw")
    else 
        set_toolchains("gcc")
    end 


target("test-algmath-vector")
    set_kind("binary")
    add_files("test/alg_vector_test.c", "test/test_framework.c")
    add_deps("algmath")
    add_includedirs(".")
    if is_plat("windows") then
        set_toolchains("mingw")
    else 
        set_toolchains("gcc")
    end 

target("test-algmath-matrix")
    set_kind("binary")
    add_files("test/alg_matrix_test.c", "test/test_framework.c")
    add_deps("algmath")
    add_includedirs(".")
    if is_plat("windows") then
        set_toolchains("mingw")
    else 
        set_toolchains("gcc")
    end 

