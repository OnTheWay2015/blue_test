
set_config("mode", "debug")
target("protocol")
    set_pcxxheader("stdafx.h")
    set_kind("static")
    add_includedirs(
        "./"
    )

if is_plat("linux") then
    add_includedirs(
        "./../3rds/include/linux"
        ,"./../3rds/include"
    )
elseif is_plat("windows") then
    add_includedirs(
        "./../3rds/include/win"
        ,"./../3rds/include"
    )
end



    add_files("*.cpp")
    add_files("*.cc")

