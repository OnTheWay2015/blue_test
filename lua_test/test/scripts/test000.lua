 
-- 元表
function test01()
	myTable = {}
	myTable1 = {}
	print("lua test01")
	--ttt = {name:"test"} 注意不是 json 写法.key 不需引号，中间用等号
	ttt = {name = "xiaomign", id = 123456}

	ttt.ftestget = function (x) -- 声明对象独立方法
		TestFunc(x) -- c++方法,调试断点，定位使用
		print("myTable.id:" .. myTable.id)  -- 
	end

	ttt.f = function (x) -- 声明对象独立方法
		TestFunc(x)-- c++方法,调试断点，定位使用
		x.name = "666" -- 键名虽然在元表的 __index 也有,但对象自身是没有这个键的,会创建新键 
	end

	function ttt:ff () -- 声明对象绑定传递自身方法
		TestFunc(x)-- c++方法,调试断点，定位使用
		self.id= 1000  -- 键名虽然在元表的 __index 也有,但对象自身是没有这个键的,会创建新键 
	end

	m = { __index = ttt }
	setmetatable(myTable, m)  -- 对象实例 指定 m 为元表 
	setmetatable(myTable1, m) -- 对象实例 指定 m 为元表 


	print("myTable.name:" .. myTable.name)

	myTable.ftestget(myTable)
	myTable.f(myTable) -- 修改对象自身的属性 name
	myTable1:ff() -- 修改对象自身的属性 id 
	
	print("myTable.name:" .. myTable.name)  -- xiaomign
	print("myTable1.name:" .. myTable1.name) -- 666
	
	print("myTable.id:" .. myTable.id)  -- 123456 
	print("myTable1.id:" .. myTable1.id) -- 1000 


	--在元表里的 "__index"  属性,才有特别效果!
	mt = getmetatable(myTable)  -- 
	print("mt,__index.id:" .. mt.__index.id) -- 返回元表 __index.id 
	
	mt1 = getmetatable(myTable1)  -- 
	print("mt1.__index.id:" .. mt1.__index.id) -- 返回元表 __index.id 

	mt.__index.id = 999  -- 修改了元表的 __index.id 数据
	print("myTable.id:" .. myTable.id) -- myTable.id 之前没有设置过,返回元表 __index.id 
	print("myTable1.id:" .. myTable1.id) -- myTable1.id 在 myTable1:ff() 设置过,返回 myTable1 新建的值
end

-- 入口方法 
function run_main()
  test01()
end
 
 

 