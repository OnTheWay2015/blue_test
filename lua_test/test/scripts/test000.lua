--[[
__index ��������Ϊһ���������� table


--]] 
 
 
-- Ԫ��
function test01()
	myTable = {}
	myTable1 = {}
	print("lua test01")
	--ttt = {name:"test"} ע�ⲻ�� json д��.key �������ţ��м��õȺ�
	ttt_index = { m_value="ok"}
	--ttt = {name = "xiaomign", id = 123456, __index=ttt_index}  -- __index ���ܵݹ����
	ttt = {name = "xiaomign", id = 123456, m_value="ok"}

	ttt.ftestget = function (x) -- ���������������
		TestFunc(x) -- c++����,���Զϵ㣬��λʹ��
		print("myTable.id:" .. myTable.id)  -- 
	end

	ttt.f = function (x) -- ���������������
		TestFunc(x)-- c++����,���Զϵ㣬��λʹ��
		x.name = "666" -- ������Ȼ��Ԫ��� __index Ҳ��,������������û���������,�ᴴ���¼� 
	end

	function ttt:ff () -- ��������󶨴���������
		TestFunc(x)-- c++����,���Զϵ㣬��λʹ��
		self.id= 1000  -- ������Ȼ��Ԫ��� __index Ҳ��,������������û���������,�ᴴ���¼� 
	end

	m = { __index = ttt } 
	setmetatable(myTable, m)  -- ����ʵ�� ָ�� m ΪԪ�� 
	setmetatable(myTable1, m) -- ����ʵ�� ָ�� m ΪԪ�� 


	--print("myTable.name:" .. myTable.name)

	--myTable.ftestget(myTable)
	--print("")

	
	myTable.f(myTable) -- �޸Ķ������������ name Ϊ "xiaomign"
	myTable1:ff() -- �޸Ķ������������ id Ϊ 1000
	print("myTable.name:" .. myTable.name)  -- xiaomign
	print("myTable.id:" .. myTable.id)  -- 123456 
	print("")
	print("myTable1.name:" .. myTable1.name) -- 666
	print("myTable1.id:" .. myTable1.id) -- 1000 
	print("")

	--��Ԫ����� "__index"  ����,�����ر�Ч��!
	mt = getmetatable(myTable)  -- 
	print("mt,__index.id:" .. mt.__index.id) -- ����Ԫ�� __index.id 
	
	mt1 = getmetatable(myTable1)  -- 
	print("mt1.__index.id:" .. mt1.__index.id) -- ����Ԫ�� __index.id 
	print("")


	print("before myTable.id:" .. myTable.id) -- 123456  δ�½�ֵ������Ԫ�� __index.id 
	print("before myTable1.id:" .. myTable1.id) -- 1000, myTable1.id �� myTable1:ff() ���ù�,���� myTable1 �½���ֵ
	mt.__index.id = 999  -- �޸���Ԫ��� __index.id ����
	print("myTable.id:" .. myTable.id) -- 999, myTable.id ֮ǰû�����ù�,����Ԫ�� __index.id 
	print("myTable1.id:" .. myTable1.id) -- 1000, myTable1.id �� myTable1:ff() ���ù�,���� myTable1 �½���ֵ
	print("myTable1.m_value:" .. myTable1.m_value) --  
end

-- ��ڷ��� 
function run_main()
  test01()
end
 
 

 