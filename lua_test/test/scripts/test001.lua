-- 常用写法

mystr = "I'm lua"

myTable = {name = "xiaomign", id = 123456}
myAry= {"a1","a2"}

function print_hello()
  print("hello lua")
end

function _add(a, b)
  return a + b
end


function run_main()
  print("hello lua")
end

--print_hello()



--[[   p_func_proto(Proto *p) 结果
 Proto: --------------------

main <@../../test/scripts/test001.lua:0,0> (18 instructions, 72 bytes at 000001A8325A1ED0)
0+ params, 3 slots(maxstacksize), 0 upvalues, sizeupvalues[0] 0 locals, 12 constants, 2 functions
function Instructions start:
        1       line[1] LOADK                   ; "I'm lua"
        2       line[1] SETGLOBAL               ; mystr
        3       line[3] NEWTABLE        iABC argA[0]
        4       line[3] SETTABLE        iABC argA[0]    ; "name" "xiaomign"
        5       line[3] SETTABLE        iABC argA[0]    ; "id" 123456
        6       line[3] SETGLOBAL               ; myTable
        7       line[4] NEWTABLE        iABC argA[0]
        8       line[4] LOADK                   ; "a1"
        9       line[4] LOADK                   ; "a2"
        10      line[4] SETLIST         iABC argA[0]    ; 1
        11      line[4] SETGLOBAL               ; myAry
        12      line[8] CLOSURE                 ; 000001A832594A60
        13      line[6] SETGLOBAL               ; print_hello
        14      line[12]        CLOSURE                 ; 000001A832594B20
        15      line[10]        SETGLOBAL               ; _add
        16      line[14]        GETGLOBAL               ; print_hello
        17      line[14]        CALL            iABC argA[0]
        18      line[14]        RETURN          iABC argA[0]
function Instructions end!
constants (12) for 000001A8325A1ED0:
        1       "mystr"
        2       "I'm lua"
        3       "myTable"
        4       "name"
        5       "xiaomign"
        6       "id"
        7       123456
        8       "myAry"
        9       "a1"
        10      "a2"
        11      "print_hello"
        12      "_add"
locals (0) for 000001A8325A1ED0:
upvalues (0) for 000001A8325A1ED0:

function <@../../test/scripts/test001.lua:6,8> (4 instructions, 16 bytes at 000001A832594A60)
0 params, 2 slots(maxstacksize), 0 upvalues, sizeupvalues[0] 0 locals, 2 constants, 0 functions
function Instructions start:
        1       line[7] GETGLOBAL               ; print
        2       line[7] LOADK                   ; "hello lua"
        3       line[7] CALL            iABC argA[0]
        4       line[8] RETURN          iABC argA[0]
function Instructions end!
constants (2) for 000001A832594A60:
        1       "print"
        2       "hello lua"
locals (0) for 000001A832594A60:
upvalues (0) for 000001A832594A60:

function <@../../test/scripts/test001.lua:10,12> (3 instructions, 12 bytes at 000001A832594B20)
2 params, 3 slots(maxstacksize), 0 upvalues, sizeupvalues[0] 2 locals, 0 constants, 0 functions
function Instructions start:
        1       line[11]        ADD             iABC argA[2]
        2       line[11]        RETURN          iABC argA[2]
        3       line[12]        RETURN          iABC argA[0]
function Instructions end!
constants (0) for 000001A832594B20:
locals (2) for 000001A832594B20:
        0       a       1       3
        1       b       1       3
upvalues (0) for 000001A832594B20:


--]]

