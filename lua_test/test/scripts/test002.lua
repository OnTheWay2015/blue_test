--使用 c++ 方法  lua_register(pState, "average", Average)
--使用 c++ 方法  lua_register(pState, "testArgs", LuaTestArgs)

--使用 c++ 使用 Lua 方法 


avg, sum = average(10,20,30,40,50);
print("The average is ", avg)
print("The sum is ", sum)





function run_main(arg_number, arg_integer,arg_string,arg_bool,arg_array,arg_table)
    print("arg_number:", arg_number)
    print("arg_integer:", arg_integer)
    print("arg_string:", arg_string)
    print("arg_bool:", arg_bool)

    print("arg_array----------------------:")
    DUMP(arg_array)
    print("arg_table----------------------:")
    DUMP(arg_table)

    print("")
    print("")
    print("")
    local tab = {[1]=1, [2]=2, ["x"]=3333, [4]=4}
    local ary = {[1]=1, [2]=2, [3]=3}
    testArgs(1.0,1000,false,"ccccc",nil,ary,tab)
end
