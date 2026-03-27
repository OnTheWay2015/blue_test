
-- 添加 require 搜索目录 
package.path = package.path .. ';../../../test/scripts/?.lua' 
print(package.path)

-- 查看已加载的文件 
print(package.loaded)

-- 清除已加载的文件 
package.loaded["filename"] = nil


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




function test_gmath()
    local str = "3*ABC+4*DDD+5*xxxxx"

    -- 存储最终拆分结果的表
    local result = { }

    -- 第一步：按 "+" 拆分所有项（匹配所有非"+"的字符序列）
	--  [^+]+ 这个 Lua 模式匹配表达式为什么不会匹配到 + 号，核心原因是 [^+] 这个字符集的定义本身就排除了 +，再结合 + 修饰符的作用，最终只会匹配连续的、不含 + 的字符序列。
    for item in string.gmatch(str, "[^+]+") do
        -- 第二步：对每个项按 "*" 拆分，提取数字和字符部分
        -- 模式解释：(%d+) 匹配数字部分，(.+) 匹配*后的所有字符
        local num, char_part = string.match(item, "(%d+)%*(.+)")  -- 第二个 % 是 Lua 正则（模式匹配）中的转义符，用来让后面的 * 表示 “普通的星号字符”，而非正则的特殊功能

        -- 将拆分结果存入表（做非空判断，避免异常）
        if num and char_part then
            table.insert(result, {
                number = tonumber(num),
                -- 转成数字类型（可选）
                text = char_part
            } )
        end
    end

    -- 打印拆分结果，验证效果
    print("拆分后的详细结果：")
    for i, v in ipairs(result) do
        print(string.format("第%d项：数字=%d，字符=%s", i, v.number, v.text))
    end

    -- 也可以直接遍历原始拆分的项（不整理成表）
    print("\n直接遍历拆分项：")
    for item in string.gmatch(str, "[^+]+") do
        print("原始项：" .. item)
        local num, char_part = string.match(item, "(%d+)%*(.+)")
        print("  → 拆分：数字=" .. num .. "，字符=" .. char_part)
    end

end


-- 入口方法 
function run_main()
	test_gmath()
	--normal_use()
	--TestString()
end
 
 

 