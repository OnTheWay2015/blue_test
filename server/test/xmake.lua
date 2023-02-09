
target("test")
    set_pcxxheader("stdafx.h")
    set_kind("binary")
    add_includedirs(
        "./"
        ,"../"
        ,"../utils"
        ,"./../3rds/include"
    )

if is_plat("windows") then
    add_links("libprotobufd")
    add_linkdirs("./../3rds/libs")
end
    
    --add_deps("netlib")
    --add_deps("utils")
    --add_deps("protocol")


    add_files("./*.cpp")
    add_files("./**/*.cpp")





if is_plat("linux") then
    add_links("pthread","dl")
end

if is_plat("windows") then
end

