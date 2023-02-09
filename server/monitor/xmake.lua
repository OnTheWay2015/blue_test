
target("monitorD")
    set_pcxxheader("stdafx.h")
    set_kind("binary")
    add_includedirs(
        "./"
        ,"../"
        ,"../utils"
    )
    
    add_deps("netlib")
    add_deps("utils")

    add_files("*.cpp")


if is_plat("linux") then
    add_links("pthread","dl")
end

if is_plat("windows") then
end

