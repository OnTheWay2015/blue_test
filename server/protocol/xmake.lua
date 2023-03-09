
set_config("mode", "debug")
target("protocol")

if is_plat("linux") then
    add_includedirs(
        "./../protocol/linux"
        ,"./../3rds/include/linux"
        ,"./../3rds/include"
    )
elseif is_plat("windows") then
    add_defines("WIN32_LEAN_AND_MEAN") 
    add_includedirs(
        "./../protocol/win"
        ,"./../3rds/include/win"
        ,"./../3rds/include"
    )
end


set_pcxxheader("stdafx.h")
set_kind("static")
add_includedirs(
    "./"
)



if is_plat("linux") then
    --add_files("linux/*.cpp")
    add_files("linux/*.cc")
elseif is_plat("windows") then
    --add_files("win/*.cpp")
    add_files("win/*.cc")
end



