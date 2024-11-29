add_rules("mode.debug", "mode.release")

includes("algmath")

if is_plat("windows") then
    set_toolchains("mingw")
else 
    set_toolchains("gcc")
end 

target("pso")
    set_kind("binary")
    add_files("src/pso/c/*.c")
    add_deps("algmath")
    add_includedirs("algmath")


target("ga")
    set_kind("binary")
    add_files("src/ga/c/*.c")
    add_deps("algmath")
    add_includedirs("algmath")

