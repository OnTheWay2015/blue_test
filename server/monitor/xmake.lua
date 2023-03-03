
target("monitorD")
    set_pcxxheader("stdafx.h")
    set_kind("binary")
    add_includedirs(
        "./"
        ,"../"
        ,"../utils"
    )
if is_plat("windows") then
   add_defines("WIN32_LEAN_AND_MEAN") 
end
    
    add_deps("netlib")
    add_deps("utils")
    add_deps("corelib")

    add_files("*.cpp")


if is_plat("linux") then
    add_links("pthread","dl")
end

if is_plat("windows") then
end

