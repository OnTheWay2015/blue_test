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
    BOOL = "b",
}

GD.C2S_MessageId = {
    TTT = 12345
}

-- 消息协议定义 - 使用数组保证顺序
GD.MSG_Protocol = {
    [GD.C2S_MessageId.TTT] = {
        -- 每个字段定义为一个表，包含 name 和 type，以及其他必要的嵌套定义
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
    return bytes and #bytes > 0 and pos and pos <= #bytes
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

-- 打包固定长度字节数组
local function packFixedBytes(lengthType, subtype, data)
    if not data or #data == 0 then
        return safePack("<" .. lengthType, 0)
    end
    local lengthBytes = safePack("<" .. lengthType, #data)
    -- 注意：struct.pack("<cN", str) 需要 N 等于字符串长度
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

-- --- Forward Declarations for Recursive Functions ---
local packObject, unpackObject

-- 打包数组
local function packArray(elementDef, data)
    local lengthFmt = elementDef.lengthType
    local elemType = elementDef.elementType
    local elemFields = elementDef.elementFields

    -- 打包数组长度
    local bytes = safePack("<" .. lengthFmt, #data or 0)
    if not data or #data == 0 then
        return bytes
    end

    -- 按顺序打包每个元素
    for i = 1, #data do
        local itemData = data[i]
        if elemType == GD.CONVERT_KEYS.OBJECT and elemFields then
            -- 如果元素是对象，递归打包 (调用 forward declared 函数)
            bytes = bytes .. packObject(elemFields, itemData)
        elseif elemType == GD.CONVERT_KEYS.INT then
            -- 如果元素是简单类型（如INT）
            bytes = bytes .. safePack("<" .. elemType, itemData)
        else
            error("Unsupported array element type for packing: " .. tostring(elemType))
        end
    end
    return bytes
end

-- 解包数组
local function unpackArray(elementDef, bytes, pos)
    local lengthFmt = elementDef.lengthType
    local elemType = elementDef.elementType
    local elemFields = elementDef.elementFields

    local data = {}
    if not isValidBytesAndPos(bytes, pos) then
        return data, pos
    end

    -- 解包数组长度
    local length, nextPos = safeUnpack("<" .. lengthFmt, bytes, pos)
    if not length or length <= 0 then
        return data, nextPos
    end

    -- 按顺序解包每个元素
    for i = 1, length do
        local itemValue
        if elemType == GD.CONVERT_KEYS.OBJECT and elemFields then
            -- 如果元素是对象，递归解包 (调用 forward declared 函数)
            itemValue, nextPos = unpackObject(elemFields, bytes, nextPos)
        elseif elemType == GD.CONVERT_KEYS.INT then
            -- 如果元素是简单类型（如INT）
            itemValue, nextPos = safeUnpack("<" .. elemType, bytes, nextPos)
        else
            error("Unsupported array element type for unpacking: " .. tostring(elemType))
        end
        table.insert(data, itemValue)
    end
    return data, nextPos
end

-- 打包对象 (按协议定义数组顺序)
packObject = function(fields, data) -- 赋值给 forward declared 的变量
    local bytes = ""
    if not data then
        return bytes
    end
    -- 遍历协议定义数组，保证顺序
    for _, fieldDef in ipairs(fields) do
        local fieldName = fieldDef.name
        local fieldType = fieldDef.type
        local fieldSubDef = fieldDef.fields or fieldDef.elementDef -- Handle nested structures

        local value = data[fieldName]
        local fmt = "<" .. fieldType

        if fieldType == GD.CONVERT_KEYS.OBJECT then
            -- 递归打包嵌套对象 (调用 forward declared 函数)
            bytes = bytes .. packObject(fieldSubDef, value)
        elseif fieldType == GD.CONVERT_KEYS.ARRAY then
            -- 打包数组 (调用同级定义的函数)
            bytes = bytes .. packArray(fieldSubDef, value)
        elseif fieldType == GD.CONVERT_KEYS.BYTE_FIX then
            -- 打包固定长度字节数组
            -- 注意：协议定义中需要包含长度类型和子类型信息
            -- 这里假设 elementDef 包含了这些信息
            bytes = bytes .. packFixedBytes(fieldSubDef.lengthType, fieldSubDef.subtype, value)
        elseif fieldType == GD.CONVERT_KEYS.BOOL then
            -- 打包布尔值
            bytes = bytes .. packBool(value)
        else
            -- 打包基本类型
            bytes = bytes .. safePack(fmt, value)
        end
    end
    return bytes
end

-- 解包对象 (按协议定义数组顺序)
unpackObject = function(fields, bytes, pos) -- 赋值给 forward declared 的变量
    local data = {}
    local currentPos = pos

    -- 遍历协议定义数组，保证顺序
    for _, fieldDef in ipairs(fields) do
        local fieldName = fieldDef.name
        local fieldType = fieldDef.type
        local fieldSubDef = fieldDef.fields or fieldDef.elementDef

        local value
        if fieldType == GD.CONVERT_KEYS.OBJECT then
            -- 递归解包嵌套对象 (调用 forward declared 函数)
            value, currentPos = unpackObject(fieldSubDef, bytes, currentPos)
        elseif fieldType == GD.CONVERT_KEYS.ARRAY then
            -- 解包数组 (调用同级定义的函数)
            value, currentPos = unpackArray(fieldSubDef, bytes, currentPos)
        elseif fieldType == GD.CONVERT_KEYS.BYTE_FIX then
            -- 解包固定长度字节数组
            value, currentPos = unpackFixedBytes(fieldSubDef.lengthType, fieldSubDef.subtype, bytes, currentPos)
        elseif fieldType == GD.CONVERT_KEYS.BOOL then
            -- 解包布尔值
            value, currentPos = unpackBool(bytes, currentPos)
        else
            -- 解包基本类型
            local fmt = "<" .. fieldType
            value, currentPos = safeUnpack(fmt, bytes, currentPos)
        end
        -- 将解析出的值赋给结果表
        data[fieldName] = value
    end
    return data, currentPos
end

-- 公共 API: 打包消息
function GD.packMsg(id, data)
    local protocolDef = GD.MSG_Protocol[id]
    if not protocolDef then
        print("Warning: No protocol definition found for message ID: " .. id)
        return ""
    end
    -- 使用协议定义的有序数组进行打包
    return packObject(protocolDef, data) -- 调用 forward declared 的函数
end

-- 公共 API: 解包消息
function GD.unPackMsg(id, bytes)
    local protocolDef = GD.MSG_Protocol[id]
    if not protocolDef then
        print("Warning: No protocol definition found for message ID: " .. id)
        return {}
    end
    -- 使用协议定义的有序数组进行解包
    local data, _ = unpackObject(protocolDef, bytes, 1) -- 调用 forward declared 的函数
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
    testpack()
end

return GD -- Return the module table if needed elsewhere