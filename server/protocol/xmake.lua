
set_config("mode", "debug")
target("protocol")
    set_pcxxheader("stdafx.h")
    set_kind("static")
    add_includedirs(
        "./",
        "./../3rds/include"
    )
    
    add_files("*.cpp")
    add_files("*.cc")

