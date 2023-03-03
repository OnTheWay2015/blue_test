
--target("DObjectsLib")
--    set_pcxxheader("stdafx.h")
--    set_kind("static")
--    add_includedirs(
--        "./"
--        ,"../"
--    )
--    
--    add_deps("netlib")
--    add_deps("utils")
--
--    add_files("*.cpp")



target("corelib")
    set_pcxxheader("stdafx.h")
    set_kind("static")

    add_includedirs(
        "./"
        ,"../"
    )

if is_plat("windows") then
   add_defines("WIN32_LEAN_AND_MEAN") 
end


    add_deps("utils")
    add_deps("netlib")

    add_files("*.cpp")
    




 
