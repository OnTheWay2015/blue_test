



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
	TestString()
end
 
 

 