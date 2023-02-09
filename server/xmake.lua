
--add_defines("WIN32","_WINDOWS", "_UNICODE","UNICODE")



if is_plat("linux") then
    set_config("cxxflags", "-fpermissive")
end



set_config("vs", "2019")
--set_config("arch", "x86")
set_config("arch","x64")


add_rules("mode.debug", "mode.release")

set_config("mode", "debug")


if is_plat("windows") then
    if is_mode("debug") then 
        set_runtimes("MTd")
    else
        set_runtimes("MT")
    end 
end

includes("./protocol")
includes("./protocoltest")
includes("./utils")
includes("./netlib")
includes("./vvv")



--includes("./test")

--target("t")
--    set_kind("binary")
--    add_files("a.cpp")
