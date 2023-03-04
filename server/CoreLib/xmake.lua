target("corelib")
    set_pcxxheader("stdafx.h")
    set_kind("static")

    add_includedirs(
        "./"
        ,"../"
        ,"../utils"
    )

if is_plat("linux") then
    add_includedirs(
        "./../3rds/include/linux"
        ,"./../3rds/include"
    )
elseif is_plat("windows") then
   add_defines("WIN32_LEAN_AND_MEAN") 
    add_includedirs(
        "./../3rds/include/win"
        ,"./../3rds/include"
    )
end




    add_deps("utils")
    add_deps("netlib")

    add_files("*.cpp")
    




 
