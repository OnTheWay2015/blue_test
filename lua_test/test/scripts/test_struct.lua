package.path = package.path .. ';../../../test/scripts/?.lua' 
local struct = require("struct")  --好像有点问提，可以用 struct_lib.c

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
}

GD.C2S_MessageId={
	TTT = 12345
}


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
			GD.CONVERT_KEYS.INT,{
			[1]={"c",GD.CONVERT_KEYS.INT},
			[2]={"d",GD.CONVERT_KEYS.INT},

		}},
		[4]={"fval",GD.CONVERT_KEYS.FLOAT},
		[5]={"str1",GD.CONVERT_KEYS.STR},
		[6]={"ary2",
			GD.CONVERT_KEYS.ARRAY,
			GD.CONVERT_KEYS.INT,
			GD.CONVERT_KEYS.INT
		},		
		[7]={"str2",GD.CONVERT_KEYS.STR},
		--[8]={"bytes",GD.CONVERT_KEYS.BYTE_FIX,GD.CONVERT_KEYS.BYTE},
		
	}, 
}

function GD.packMsgKey(keysfmt,data)
	local tp = keysfmt[2]
	if tp == GD.CONVERT_KEYS.OBJECT then
		return GD.packMsgObject(keysfmt[3],data)
	elseif tp == GD.CONVERT_KEYS.ARRAY then
		return GD.packMsgArray(keysfmt[3],keysfmt[4],data) 
	elseif tp == GD.CONVERT_KEYS.BYTE_FIX then
			return GD.packMsgByteFix(keysfmt[3],data) 
	else
		if data == nil then
			return ""
		end
		local packfmt = "<" .. tp
		return struct.pack(packfmt,data)
	end 
end

function GD.packMsgByteFix(keysfmt,data)
	local bytes=""
	if not data or #data <=0 then
		return bytes
	end	
	local pos = -1
	local len=#data
	local packfmt = "<" .. keysfmt 
	bytes = struct.pack(packfmt,len)
	
	local lenstr=""..len
	packfmt = "<c" .. lenstr 
	local b = struct.pack(packfmt,data)
	return bytes..b 
end

function GD.packMsgArray(lenfmt,keysfmt,data)
	local bytes=""
	if not data or #data <=0 then
		return ""
	end
	local packfmt = "<" .. lenfmt 
	bytes = struct.pack(packfmt,#data)

	if type(keysfmt) == "table" then
		for i=1,#data do
			bytes = bytes .. GD.packMsgObject(keysfmt,data[i])
		end
	else
		for i=1,#data do
			bytes = bytes .. GD.packMsgKey({"",keysfmt} ,data[i]) 
		end
	end
	return bytes
	  
end


function GD.packMsgObject(keysfmt,data)
	local bytes="" 
	for _,v in ipairs(keysfmt) do
		local k = v[1]
		--bytes = bytes .. GD.packMsgKey(v,data[k])
		local bb = 	 GD.packMsgKey(v,data[k])
		local len = #bb;
		bytes = bytes ..bb
		len = #bytes;
	end 
	local lll = #bytes
	return bytes  
end

-----------------------------------------------------------------

function GD.unPackMsgKey(keysfmt,bytes,pos)
	local tp = keysfmt[2]
	if tp == GD.CONVERT_KEYS.OBJECT then
		return GD.unPackMsgObject(keysfmt[3],bytes,pos)
	elseif tp == GD.CONVERT_KEYS.ARRAY then
		return GD.unPackMsgArray(keysfmt[3],keysfmt[4],bytes,pos) 
	elseif tp == GD.CONVERT_KEYS.BYTE_FIX then
			return GD.unPackMsgByteFix(keysfmt[3],bytes,pos) 
	else
		local len =#bytes -- 当 struct.unpack 返回 pos 为 nil 时，可能是长度不够 
		local packfmt = "<" .. tp
		local data,p = struct.unpack(packfmt,bytes,pos)
		return data,p
	end 	 
end

function GD.unPackMsgByteFix(keysfmt,bytes,pos)
	 
	if #bytes <=0 or #bytes < pos then
		return "",pos
	end
	local nextPos = pos 
	
	local packfmt = "<" .. keysfmt
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


function GD.unPackMsgArray(lenfmt,keysfmt,bytes,pos)
	local data = {}
	local nextPos = pos 
	if #bytes <=0 or #bytes < pos then
		return data,pos
	end
	local packfmt = "<" .. lenfmt
	local len = -1	
	len,nextPos = struct.unpack(packfmt,bytes,pos)
	
	local tmp
	if type(keysfmt) == "table" then
		for i=1,len do
			tmp,nextPos = GD.unPackMsgObject(keysfmt,bytes,nextPos)
			table.insert(data,tmp)
		end
	else
		for i=1,len do 
			tmp,nextPos = GD.unPackMsgKey({"",keysfmt},bytes,nextPos )
			table.insert(data,tmp) 
		end
	end
	return data,nextPos 
end


function GD.unPackMsgObject(keysfmt,bytes,pos)
	local data = {}
	local nextPos = pos 
	for _,v in ipairs(keysfmt) do
		local k = v[1]
		data[k],nextPos = GD.unPackMsgKey(v,bytes,pos)
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
 
 

 