
--
--[[  
Lua是动态类型语言，变量不要类型定义。Lua中有8个基本类型分别为：
	nil
	boolean
	number
	string
	userdata
	function
	thread
	table   //table是初始化为数组时,第一个元素索引为1
--]]
function base_type()
	print(type("Hello world")) -- string
	print(type(10.4*3)) -- number
	print(type(print)) -- function
	print(type(type)) -- function
	print(type(true)) -- boolean
	print(type(nil)) -- nil
	print(type(type(X))) -- string	
	

	local v = nil
	if not v then
		print(" not v!!")
	else	-- 没有 else if  
		if (false) then
			-- 此处可添加其他分支的逻辑
		else
			-- 此处可添加其他分支的逻辑
		end
	end
end

local function localfunction()
	print("localfunction act!")
end


local function arraytest()
    local array = { "Lua", "Tutorial" }
	table.insert(array, "add01" ) -- 在后追加
	table.insert(array, 1,"add02" )  -- 在 指定位置 1 处插入
	-- 不要在数组中间插入 nil
    print("-----------------")
    for i = 1, #array do
        print(array[i])
    end
    print("-----------------")


	-- 语法1：删除table的最后一个元素（最常用，无需指定索引）
	table.remove(array) -- 数组型 table，会自动维护索引连续性
    for i = 1, #array do
        print(array[i])
    end
	
    print("-----------------")
	-- 语法2：删除table中指定索引pos的元素
	table.remove(array, 1) -- 数组型 table，会自动维护索引连续性
    for i = 1, #array do
        print(array[i])
    end
    print("-----------------")
	
end

function normal_use()
    local tab = { name = "abc", v = 1 }
    -- 用 nil 来删除 table 表中的值
    tab.name = nil

    -- nil 类型表示一种没有任何有效值，它只有一个值 – nil
    -- nil 作比较时应该加上双引号
    if (tab.name == "nil") then
    end
    base_type()
    localfunction()
    arraytest()
end


function TestString()
	local value = 11000
	value = tostring(value)
	value = string.gsub( value, "0+$", "")
	local aaa = "121100";
	local bbb= "1101011";

	--local iter1 =string.gfind(aaa,"^"..value)
	--local iter11 = string.gfind(aaa,""..value)
	--local iter2 = string.gfind(bbb,"^"..value)
	--local iter22 = string.gfind(bbb,""..value)

	--for key in string.gmatch(aaa,"^"..value) do
	--	print(key)	
	--end


	--for key in string.gmatch(aaa,""..value)  do
	--	print(key)	
	--end


	--for key in string.gmatch(bbb,"^"..value) do
	for key1,key2 in string.gmatch(bbb,"(%d+)11(%d+)") do
		print(key1.."/"..key2)	
	end

	for key in string.gmatch(bbb,""..value) do
		print(key)	
	end


	local iter1 =string.gmatch(aaa,"^"..value)
	local iter11 = string.gmatch(aaa,""..value)
	local iter2 = string.gmatch(bbb,"^"..value)
	local iter22 = string.gmatch(bbb,""..value)


end




-- 入口方法 
function run_main()
	normal_use()
	TestString()
end
 
 

 