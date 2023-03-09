
target("monitorD")
    set_pcxxheader("stdafx.h")
    set_kind("binary")
    add_includedirs(
        "./"
        ,"../"
        ,"../utils"
    )

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



    add_files("*.cpp")


if is_plat("linux") then
    add_linkdirs("./../3rds/libs/linux")
    --add_linkdirs("./../build/linux/x64/debug")
    add_linkdirs("$(buildir)/$(os)/$(arch)/$(mode)")

    add_links("protocol")
    add_links("corelib")
    add_links("protobuf")
    add_links("netlib")
    add_links("utils")
    add_links("pthread","dl")
elseif is_plat("windows") then

    add_deps("protocol")
    add_deps("corelib")
    add_deps("netlib")
    add_deps("utils")

    add_linkdirs("./../3rds/libs/win")
    add_links("libprotobufd")
end



