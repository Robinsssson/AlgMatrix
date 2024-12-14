if is_plat("windows") then
    set_toolchains("gcc")
else 
    set_toolchains("gcc")
end

target("algmath")
    set_kind("shared")
    add_files("**.c|test/*.c")
    add_defines("ALG_EXPORT")
    add_cflags(
        "-Wall", "-Wextra", "-Werror", "-pedantic", "-std=c11",
        "-Wshadow", "-Wconversion", "-Wfloat-equal", "-Wundef", 
        "-Wstrict-prototypes", "-Wmissing-prototypes", 
        "-Wredundant-decls"
    )

target("test-algmath-vector")
    set_kind("binary")
    add_files("test/test_alg_vector.c", "test/test_framework.c", "test/debug_memory.c")
    add_deps("algmath")
    add_includedirs(".")
    add_cflags(
        "-Wall", "-Wextra", "-Werror", "-pedantic", "-std=c11",
        "-Wshadow", "-Wconversion", "-Wfloat-equal", "-Wundef", 
        "-Wstrict-prototypes", "-Wmissing-prototypes", 
        "-Wredundant-decls"
    )

target("test-algmath-matrix")
    set_kind("binary")
    add_files("test/test_alg_matrix.c", "test/test_framework.c", "test/debug_memory.c")
    add_deps("algmath")
    add_includedirs(".")
    add_cflags(
        "-Wall", "-Wextra", "-Werror", "-pedantic", "-std=c11",
        "-Wshadow", "-Wconversion", "-Wfloat-equal", "-Wundef", 
        "-Wstrict-prototypes", "-Wmissing-prototypes", 
        "-Wredundant-decls"
    )

