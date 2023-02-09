
target("DObjectsLib")
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


 
