
target("netlib")
    set_pcxxheader("stdafx.h")
    set_kind("static")
    --add_includedirs(
    --    "D:/__work/server_xmake/netlib"
    --    ,"D:/__work/server_xmake/utils"
    --    ,"D:/__work/server_xmake/"
    --)

    add_includedirs(
        "./"
        ,"../utils"
        ,"../utils/iconv"
        ,"../"
    )


    add_deps("utils")

    add_files("*.cpp")
    
    if is_plat("linux") then
        add_files("./linux/*.cpp")
    end

    if is_plat("windows") then
        add_files("./win/*.cpp")
    end



