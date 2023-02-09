
target("utils")
    set_pcxxheader("stdafx.h")
    set_kind("static")
    add_includedirs(
        "./"   --  当前脚本所在目录
    )


    add_files("*.cpp")
    if is_plat("linux") then
        --add_includedirs("./linux")
        add_files("./iconv/*.c")
        add_files("./linux/*.cpp")
    end

    if is_plat("windows") then
        --add_includedirs("./win")
        add_files("./win/*.cpp")
    end
