 
-- Ԫ��
function test01()
	myTable = {}
	myTable1 = {}
	print("lua test01")
	--ttt = {name:"test"} ע�ⲻ�� json д��.key �������ţ��м��õȺ�
	ttt = {name = "xiaomign", id = 123456}

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


	print("myTable.name:" .. myTable.name)

	myTable.ftestget(myTable)
	myTable.f(myTable) -- �޸Ķ������������ name
	myTable1:ff() -- �޸Ķ������������ id 
	
	print("myTable.name:" .. myTable.name)  -- xiaomign
	print("myTable1.name:" .. myTable1.name) -- 666
	
	print("myTable.id:" .. myTable.id)  -- 123456 
	print("myTable1.id:" .. myTable1.id) -- 1000 


	--��Ԫ����� "__index"  ����,�����ر�Ч��!
	mt = getmetatable(myTable)  -- 
	print("mt,__index.id:" .. mt.__index.id) -- ����Ԫ�� __index.id 
	
	mt1 = getmetatable(myTable1)  -- 
	print("mt1.__index.id:" .. mt1.__index.id) -- ����Ԫ�� __index.id 

	mt.__index.id = 999  -- �޸���Ԫ��� __index.id ����
	print("myTable.id:" .. myTable.id) -- myTable.id ֮ǰû�����ù�,����Ԫ�� __index.id 
	print("myTable1.id:" .. myTable1.id) -- myTable1.id �� myTable1:ff() ���ù�,���� myTable1 �½���ֵ
end

-- ��ڷ��� 
function run_main()
  test01()
end
 
 

 