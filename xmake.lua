add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = "."})

includes("algmath")

if is_plat("windows") then
    set_toolchains("gcc")
else 
    set_toolchains("gcc")
end 

target("pso")
    set_kind("binary")
    add_files("src/pso/c/*.c", "src/*.c")
    add_deps("algmath")
    add_includedirs("algmath")
    add_cflags(
        "-Wall", "-Wextra", "-Werror", "-pedantic", "-std=c11",
        "-Wshadow", "-Wconversion", "-Wfloat-equal", "-Wundef", 
        "-Wstrict-prototypes", "-Wmissing-prototypes", 
        "-Wredundant-decls"
    )


target("ga")
    set_kind("binary")
    add_files("src/ga/c/*.c", "src/*.c")
    add_deps("algmath")
    add_includedirs("algmath")
    add_cflags(
        "-Wall", "-Wextra", "-Werror", "-pedantic", "-std=c11",
        "-Wshadow", "-Wconversion", "-Wfloat-equal", "-Wundef", 
        "-Wstrict-prototypes", "-Wmissing-prototypes", 
        "-Wredundant-decls"
    )


target("abc")
    set_kind("binary")
    add_files("src/abc/c/*.c", "src/*.c")
    add_deps("algmath")
    add_includedirs("algmath")
    add_cflags(
        "-Wall", "-Wextra", "-Werror", "-pedantic", "-std=c11",
        "-Wshadow", "-Wconversion", "-Wfloat-equal", "-Wundef", 
        "-Wstrict-prototypes", "-Wmissing-prototypes", 
        "-Wredundant-decls"
    )

target("aco")
    set_kind("binary")
    add_files("src/aco/c/*.c", "src/*.c")
    add_deps("algmath")
    add_includedirs("algmath")
    add_cflags(
        "-Wall", "-Wextra", "-Werror", "-pedantic", "-std=c11",
        "-Wshadow", "-Wconversion", "-Wfloat-equal", "-Wundef", 
        "-Wstrict-prototypes", "-Wmissing-prototypes", 
        "-Wredundant-decls"
    )

target("aia")
    set_kind("binary")
    add_files("src/aia/c/*.c", "src/*.c")
    add_deps("algmath")
    add_includedirs("algmath")
    add_cflags(
        "-Wall", "-Wextra", "-Werror", "-pedantic", "-std=c11",
        "-Wshadow", "-Wconversion", "-Wfloat-equal", "-Wundef", 
        "-Wstrict-prototypes", "-Wmissing-prototypes", 
        "-Wredundant-decls"
    )

target("sa")
    set_kind("binary")
    add_files("src/sa/c/*.c", "src/*.c")
    add_deps("algmath")
    add_includedirs("algmath")
    add_cflags(
        "-Wall", "-Wextra", "-Werror", "-pedantic", "-std=c11",
        "-Wshadow", "-Wconversion", "-Wfloat-equal", "-Wundef", 
        "-Wstrict-prototypes", "-Wmissing-prototypes", 
        "-Wredundant-decls"
    )

target("eda")
    set_kind("binary")
    add_files("src/eda/c/*.c", "src/*.c")
    add_deps("algmath")
    add_includedirs("algmath")
    add_cflags(
        "-Wall", "-Wextra", "-Werror", "-pedantic", "-std=c11",
        "-Wshadow", "-Wconversion", "-Wfloat-equal", "-Wundef", 
        "-Wstrict-prototypes", "-Wmissing-prototypes", 
        "-Wredundant-decls"
    )

target("de")
    set_kind("binary")
    add_files("src/de/c/*.c", "src/*.c")
    add_deps("algmath")
    add_includedirs("algmath")
    add_cflags(
        "-Wall", "-Wextra", "-Werror", "-pedantic", "-std=c11",
        "-Wshadow", "-Wconversion", "-Wfloat-equal", "-Wundef", 
        "-Wstrict-prototypes", "-Wmissing-prototypes", 
        "-Wredundant-decls"
    )

target("cs")
    set_kind("binary")
    add_files("src/cs/c/*.c", "src/*.c")
    add_deps("algmath")
    add_includedirs("algmath")
    add_cflags(
        "-Wall", "-Wextra", "-Werror", "-pedantic", "-std=c11",
        "-Wshadow", "-Wconversion", "-Wfloat-equal", "-Wundef", 
        "-Wstrict-prototypes", "-Wmissing-prototypes", 
        "-Wredundant-decls"
    )
