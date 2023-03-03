set_config("mode", "debug")
set_runtimes("MTd")
target("protocoltest01")
    set_pcxxheader("stdafx.h")
    set_kind("binary")
    add_includedirs(
        "./"
        ,"./../3rds/include"
    )



if is_plat("windows") then
    add_links("libprotobufd")
    add_linkdirs("./../3rds/libs")
end
     
    add_files("*.cpp")





if is_plat("linux") then
    add_links("pthread","dl")
end

if is_plat("windows") then
end

