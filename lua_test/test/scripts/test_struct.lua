package.path = package.path .. ';../../../test/scripts/?.lua' 
local struct = require("struct")  --也可以用 struct_lib.c

local GD = {}
 
GD.CONVERT_KEYS={
OBJECT="OBJECT",
ARRAY="ARRAY",
INT="i",
LONG="I",
WORD="H",
SHORT="h",
FLOAT="f",
STR="s",
BYTE="B",
BYTE_FIX="c",
BOOL="b", -- todo
}

GD.C2S_MessageId={
	TTT = 12345
}

--[[
每个字段都要有
--]]
GD.MSG_Protocol={
	[GD.C2S_MessageId.TTT]={
		[1]={"lSize",GD.CONVERT_KEYS.INT},
		[2]={"obj",
			GD.CONVERT_KEYS.OBJECT,{
			[1]={"a",GD.CONVERT_KEYS.INT},
			[2]={"b",GD.CONVERT_KEYS.INT},

		}},
		[3]={"ary1",
			GD.CONVERT_KEYS.ARRAY,
			{
				GD.CONVERT_KEYS.INT, 
				GD.CONVERT_KEYS.OBJECT,
				{
					[1]={"c",GD.CONVERT_KEYS.INT},
					[2]={"d",GD.CONVERT_KEYS.INT}, 
				}
			}
		},
		[4]={"fval",GD.CONVERT_KEYS.FLOAT},
		[5]={"str1",GD.CONVERT_KEYS.STR},
		[6]={"ary2",
			GD.CONVERT_KEYS.ARRAY,
			{
				GD.CONVERT_KEYS.INT,
				GD.CONVERT_KEYS.INT
			} 
		},		
		[7]={"str2",GD.CONVERT_KEYS.STR},
		--[8]={"bytes",GD.CONVERT_KEYS.BYTE_FIX,GD.CONVERT_KEYS.BYTE},
		
	}, 
}

function GD.packMsgKey(tp,tpEx,data)
	if tp == GD.CONVERT_KEYS.OBJECT then
		return GD.packMsgObject(tpEx,data)
	elseif tp == GD.CONVERT_KEYS.ARRAY then
		return GD.packMsgArray(tpEx,data) 
	elseif tp == GD.CONVERT_KEYS.BYTE_FIX then
			return GD.packMsgByteFix(tpEx[1],data) 
	elseif tp == GD.CONVERT_KEYS.BOOL then
		local packfmt = "<" .. tp
		if  not data then
			return struct.pack(packfmt,0)
		else	
			return struct.pack(packfmt,1)
		end

	else
		if data == nil then
			return ""
		end
		local packfmt = "<" .. tp
		return struct.pack(packfmt,data)
	end 
end

function GD.packMsgByteFix(lentp,data)
	local bytes=""
	if not data or #data <=0 then
		return bytes
	end	
	local pos = -1
	local len=#data
	local packfmt = "<" .. lentp
	bytes = struct.pack(packfmt,len)
	
	local lenstr=""..len
	packfmt = "<c" .. lenstr 
	local b = struct.pack(packfmt,data)
	return bytes..b 
end

function GD.packMsgArray(tpEx,data)
	local lenfmt = tpEx[1]
	local tpCurr = tpEx[2]
	local tpExCurr= tpEx[3]

	local bytes=""
	local packfmt = "<" .. lenfmt 
	if not data or #data <=0 then
		bytes = struct.pack(packfmt,0)
		return  bytes 
	end

	bytes = struct.pack(packfmt,#data)

	for i=1,#data do
		bytes = bytes .. GD.packMsgKey(tpCurr,tpExCurr,data[i]) 
	end
	return bytes
	  
end


function GD.packMsgObject(tpEx,data)
	local bytes="" 
	if not data then
		return bytes
	end
	for _,v in ipairs(tpEx) do
		local k = v[1]
		--bytes = bytes .. GD.packMsgKey(v[2],v[3],data[k])
		local bb = 	 GD.packMsgKey(v[2],v[3],data[k])
		local len = #bb;
		bytes = bytes ..bb
		len = #bytes;
	end 
	local lll = #bytes
	return bytes  
end

-----------------------------------------------------------------

function GD.unPackMsgKey(tp,tpEx,bytes,pos)
	if #bytes <=0 or pos >=#bytes then
		return nil,pos
	end

	if tp == GD.CONVERT_KEYS.OBJECT then
		return GD.unPackMsgObject(tpEx,bytes,pos)
	elseif tp == GD.CONVERT_KEYS.ARRAY then
		return GD.unPackMsgArray(tpEx,bytes,pos) 
	elseif tp == GD.CONVERT_KEYS.BYTE_FIX then
			return GD.unPackMsgByteFix(tpEx[1],bytes,pos) 
	elseif tp == GD.CONVERT_KEYS.BYTE_FIX then
		local packfmt = "<" .. tp
		local val,p = struct.unpack(packfmt,bytes,pos)
		if val > 0 then
			return true,p
		else
			return false,p
		end
	else
		local len =#bytes -- 当 struct.unpack 返回 pos 为 nil 时，可能是长度不够 
		local packfmt = "<" .. tp
		local data,p = struct.unpack(packfmt,bytes,pos)
		return data,p
	end 	 
end

function GD.unPackMsgByteFix(lentp,bytes,pos)
	 
	if #bytes <=0 or #bytes < pos then
		return "",pos
	end
	local nextPos = pos 
	
	local packfmt = "<" ..lentp 
	local len = -1
	len ,nextPos = struct.unpack(packfmt,bytes,pos)

	if len <=0 then
		return nil,nextPos
	end
	packfmt = "<" .. len
	local bts = ""
	bts, nextPos = struct.unpack(packfmt,bytes,nextPos)
	return bts,nextPos
end


function GD.unPackMsgArray(tpEx,bytes,pos)
	local lenfmt = tpEx[1]
	local tpCurr = tpEx[2]
	local tpExCurr= tpEx[3]
	
	local data = {}
	local nextPos = pos 
	if #bytes <=0 or #bytes < pos then
		return data,pos
	end
	local packfmt = "<" .. lenfmt
	local len = -1	
	len,nextPos = struct.unpack(packfmt,bytes,pos)
	
	local tmp
	for i=1,len do 
		tmp,nextPos = GD.unPackMsgKey(tpCurr,tpExCurr,bytes,nextPos )
		table.insert(data,tmp) 
	end
	return data,nextPos 
end


function GD.unPackMsgObject(keysfmt,bytes,pos)
	local data = {}
	local nextPos = pos 
	for _,v in ipairs(keysfmt) do
		local k = v[1]
		data[k],nextPos = GD.unPackMsgKey(v[2],v[3],bytes,pos)
		pos =  nextPos 
	end  
	return data,nextPos 
end


----------------------------------------------------------------

function GD.packMsg(id,data)
	local formtab = GD.MSG_Protocol[id];
	if formtab == nil then
		return ""
	end
	--if table.numss() <=0 then
	--	return "" 
	--end
	return GD.packMsgObject(formtab,data)
end


function GD.unPackMsg(id,bytes)
	local formtab = GD.MSG_Protocol[id];
	if formtab == nil then
		return {}
	end
	--if formtab.numss() <=0 then
	--	return {} 
	--end
	local pos =1
	return GD.unPackMsgObject(formtab,bytes,pos)
end


----------------------------------------------------------------
function _testpack(id,data)
	local bytes = GD.packMsg(id,data)
	local len = #bytes

	local unpackData = GD.unPackMsg(id,bytes)
	local a = 123
end

function testpack()
	local data={
		["lSize"]=99,
		["obj"]={["a"]=11,["b"]=22},
		["ary1"]={
			{["a"]=33,["b"]=44},
			{["a"]=55,["b"]=66},
			},
		["fval"]=1.5,
		["str1"]="abc1",
		["ary2"]={1,2,3,4,5},
		["str2"]="efg2",

	} 
	_testpack(GD.C2S_MessageId.TTT,data);
end

-- 入口方法 
function run_main()
   --local gid = getGUID(); 
   --local bytes = struct.pack("<c16",gid)
   --testGUID(bytes)
   
   testpack()
   
end
 
 

 