local p = "D:/__work/blue_test/lua_test/test/scripts/lua"  
local m_package_path = package.path  
package.path = string.format("%s;%s/?.lua",  
    m_package_path, p)  



require("Char/CharDefault");




