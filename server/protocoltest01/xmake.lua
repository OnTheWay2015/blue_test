set_config("mode", "debug")
set_runtimes("MTd")
target("protocoltest01")
    set_pcxxheader("stdafx.h")
    set_kind("binary")
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




if is_plat("windows") then
    add_linkdirs("./../3rds/libs/win")
    add_links("libprotobufd")
elseif is_plat("linux") then
    add_linkdirs("./../3rds/libs/linux")
    add_links("protobuf")
end
    


    add_files("*.cpp")
    add_files("*.cc")





if is_plat("linux") then
    add_links("pthread","dl")
end

if is_plat("windows") then
end

