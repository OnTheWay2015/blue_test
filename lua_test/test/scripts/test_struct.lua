package.path = package.path .. ';../../../test/scripts/?.lua'
local struct = require("struct")

local GD = {}

-- 类型常量定义
GD.CONVERT_KEYS = {
    OBJECT = "OBJECT",
    ARRAY = "ARRAY",
    INT = "i",
    LONG = "I",
    WORD = "H",
    SHORT = "h",
    FLOAT = "f",
    STR = "s",
    BYTE = "B",
    BYTE_FIX = "c",
    BOOL = "BOOL",
}

GD.C2S_MessageId = {
    TTT = 12345
}

-- 消息协议定义 - 使用数组保证顺序
GD.MSG_Protocol = {
    [GD.C2S_MessageId.TTT] = {
        { name = "lSize", type = GD.CONVERT_KEYS.INT },
        { name = "obj", type = GD.CONVERT_KEYS.OBJECT, fields = {
            { name = "a", type = GD.CONVERT_KEYS.INT },
            { name = "b", type = GD.CONVERT_KEYS.INT },
        }},
        { name = "ary1", type = GD.CONVERT_KEYS.ARRAY, elementDef = {
            lengthType = GD.CONVERT_KEYS.INT,
            elementType = GD.CONVERT_KEYS.OBJECT,
            elementFields = {
                { name = "a", type = GD.CONVERT_KEYS.INT }, -- 修正字段名
                { name = "b", type = GD.CONVERT_KEYS.INT },
            }
        }},
        { name = "fval", type = GD.CONVERT_KEYS.FLOAT },
        { name = "str1", type = GD.CONVERT_KEYS.STR },
        { name = "ary2", type = GD.CONVERT_KEYS.ARRAY, elementDef = {
            lengthType = GD.CONVERT_KEYS.INT,
            elementType = GD.CONVERT_KEYS.INT
        }},
        { name = "str2", type = GD.CONVERT_KEYS.STR },
        -- { name = "bytes", type = GD.CONVERT_KEYS.BYTE_FIX, subtype = GD.CONVERT_KEYS.BYTE }, -- 注释掉未实现的
    }
}

-- 内部辅助函数：检查字节串和位置
local function isValidBytesAndPos(bytes, pos)
    return bytes and #bytes > 0 and pos and pos < #bytes
end

-- 内部辅助函数：安全打包
local function safePack(fmt, value)
    if value == nil then
        -- 对于某些类型，nil 可能需要特殊处理，这里简单返回空字符串
        -- 或者抛出错误，取决于协议要求
        return ""
    end
    return struct.pack(fmt, value)
end

-- 内部辅助函数：安全解包
local function safeUnpack(fmt, bytes, pos)
    if not isValidBytesAndPos(bytes, pos) then
        return nil, pos
    end
    local values, nextPos = struct.unpack(fmt, bytes, pos)
    return values, nextPos
end

-- 打包固定长度字节数组
local function packFixedBytes(lengthType, subtype, data)
    if not data or #data == 0 then
        return safePack("<" .. lengthType, 0)
    end
    local lengthBytes = safePack("<" .. lengthType, #data)
    local dataBytes = safePack("<c" .. #data, data)
    return lengthBytes .. dataBytes
end

-- 解包固定长度字节数组
local function unpackFixedBytes(lengthType, subtype, bytes, pos)
    if not isValidBytesAndPos(bytes, pos) then
        return nil, pos
    end

    local length, nextPos = safeUnpack("<" .. lengthType, bytes, pos)
    if not length or length <= 0 then
        return nil, nextPos
    end

    local dataBytes, finalPos = safeUnpack("<c" .. length, bytes, nextPos)
    return dataBytes, finalPos
end

-- 打包布尔值
local function packBool(value)
    return struct.pack("<b", value and 1 or 0)
end

-- 解包布尔值
local function unpackBool(bytes, pos)
    local value, nextPos = safeUnpack("<b", bytes, pos)
    if value ~= nil then
        return value ~= 0, nextPos
    end
    return nil, nextPos
end

-- --- 定义相互递归的核心函数 ---
-- 需要先声明变量，然后定义函数并赋值给这些变量
local packKey, unpackKey, packObject, unpackObject

-- 打包对象 (按协议定义数组顺序)
packObject = function(fields, data)
    local bytes = ""
    if not data then
        return bytes
    end
    for _, fieldDef in ipairs(fields) do
        local fieldName = fieldDef.name
        local value = data[fieldName]
        -- 使用通用 packKey 函数处理各种类型
        bytes = bytes .. packKey(fieldDef, value)
    end
    return bytes
end

-- 解包对象 (按协议定义数组顺序)
unpackObject = function(fields, bytes, pos)
    local data = {}
    local currentPos = pos
    for _, fieldDef in ipairs(fields) do
        local fieldName = fieldDef.name
        -- 使用通用 unpackKey 函数处理各种类型
        local value, nextPos = unpackKey(fieldDef, bytes, currentPos)
        data[fieldName] = value
        currentPos = nextPos
    end
    return data, currentPos
end

-- 打包单个键值对 (根据类型)
packKey = function(fieldDef, dataValue)
    local fieldType = fieldDef.type
    local fieldSubDef = fieldDef.fields or fieldDef.elementDef

    if fieldType == GD.CONVERT_KEYS.OBJECT then
        -- 递归处理对象结构 - 调用已定义的 packObject
        return packObject(fieldSubDef, dataValue)
    elseif fieldType == GD.CONVERT_KEYS.ARRAY then
        local lengthFmt = fieldSubDef.lengthType
        local elemType = fieldSubDef.elementType
        local elemFields = fieldSubDef.elementFields

        local bytes = safePack("<" .. lengthFmt, #dataValue or 0)
        if not dataValue or #dataValue == 0 then
            return bytes
        end

        for i = 1, #dataValue do
            local itemData = dataValue[i]
            if elemType == GD.CONVERT_KEYS.OBJECT and elemFields then
                -- 递归处理数组中的对象元素 - 调用已定义的 packObject
                bytes = bytes .. packObject(elemFields, itemData)
            elseif elemType == GD.CONVERT_KEYS.INT then
                bytes = bytes .. safePack("<" .. elemType, itemData)
            else
                error("Unsupported array element type for packing: " .. tostring(elemType))
            end
        end
        return bytes
    elseif fieldType == GD.CONVERT_KEYS.BYTE_FIX then
        return packFixedBytes(fieldSubDef.lengthType, fieldSubDef.subtype, dataValue)
    elseif fieldType == GD.CONVERT_KEYS.BOOL then
        return packBool(dataValue)
    else
        -- 基本类型
        local fmt = "<" .. fieldType
        return safePack(fmt, dataValue)
    end
end

-- 解包单个键值对 (根据类型)
unpackKey = function(fieldDef, bytes, pos)
    local fieldType = fieldDef.type
    local fieldSubDef = fieldDef.fields or fieldDef.elementDef

    if fieldType == GD.CONVERT_KEYS.OBJECT then
        -- 递归处理对象结构 - 调用已定义的 unpackObject
        local objData, nextPos = unpackObject(fieldSubDef, bytes, pos)
        return objData, nextPos
    elseif fieldType == GD.CONVERT_KEYS.ARRAY then
        local lengthFmt = fieldSubDef.lengthType
        local elemType = fieldSubDef.elementType
        local elemFields = fieldSubDef.elementFields

        local data = {}
        if not isValidBytesAndPos(bytes, pos) then
            return data, pos
        end

        local length, nextPos = safeUnpack("<" .. lengthFmt, bytes, pos)
        if not length or length <= 0 then
            return data, nextPos
        end

        for i = 1, length do
            local itemValue
            if elemType == GD.CONVERT_KEYS.OBJECT and elemFields then
                -- 递归处理数组中的对象元素 - 调用已定义的 unpackObject
                itemValue, nextPos = unpackObject(elemFields, bytes, nextPos)
            elseif elemType == GD.CONVERT_KEYS.INT then
                itemValue, nextPos = safeUnpack("<" .. elemType, bytes, nextPos)
            else
                error("Unsupported array element type for unpacking: " .. tostring(elemType))
            end
            data[i] = itemValue -- Use numeric index for array elements
        end
        return data, nextPos
    elseif fieldType == GD.CONVERT_KEYS.BYTE_FIX then
        return unpackFixedBytes(fieldSubDef.lengthType, fieldSubDef.subtype, bytes, pos)
    elseif fieldType == GD.CONVERT_KEYS.BOOL then
        return unpackBool(bytes, pos)
    else
        -- 基本类型
        local fmt = "<" .. fieldType
        return safeUnpack(fmt, bytes, pos)
    end
end

-- --- 公共 API ---
function GD.packMsg(id, data)
    local protocolDef = GD.MSG_Protocol[id]
    if not protocolDef then
        print("Warning: No protocol definition found for message ID: " .. id)
        return ""
    end
    return packObject(protocolDef, data) -- 调用已定义的 packObject
end

function GD.unPackMsg(id, bytes)
    local protocolDef = GD.MSG_Protocol[id]
    if not protocolDef then
        print("Warning: No protocol definition found for message ID: " .. id)
        return {}
    end
    local data, _ = unpackObject(protocolDef, bytes, 1) -- 调用已定义的 unpackObject
    return data or {}
end

-- 通用打印函数，用于显示复杂数据结构
local function print_r(t, indent)
    indent = indent or ""
    if type(t) ~= "table" then
        print(indent .. tostring(t))
        return
    end
    print(indent .. "{")
    for k, v in pairs(t) do
        io.write(indent .. " [" .. tostring(k) .. "] = ")
        if type(v) == "table" then
            print()
            print_r(v, indent .. "  ")
        else
            print(tostring(v))
        end
    end
    print(indent .. "}")
end

-- 测试函数
function _testpack(id, data)
    local bytes = GD.packMsg(id, data)
    print("Packed bytes length: " .. #bytes)
    local unpackData = GD.unPackMsg(id, bytes)
    print("Original data:")
    print_r(data)
    print("\nUnpacked data:")
    print_r(unpackData)
    print("--- Test End ---")
end

function testpack()
    local data = {
        lSize = 99,
        obj = { a = 11, b = 22 },
        ary1 = {
            { a = 33, b = 44 },
            { a = 55, b = 66 },
        },
        fval = 1.5,
        str1 = "abc1",
        ary2 = { 1, 2, 3, 4, 5 },
        str2 = "efg2",
    }
    _testpack(GD.C2S_MessageId.TTT, data)
end

function run_main()
    print("---------------")
    print(bit.band(33,32))
    testpack()
end

return GD