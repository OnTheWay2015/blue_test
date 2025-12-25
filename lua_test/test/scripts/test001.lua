-- 常用写法

mystr = "I'm lua"

myTable = {name = "xiaomign", id = 123456}
myAry= {"a1","a2"}

--func1
function myTable.test()
  print("hello myTable")
end

--func2
function myTable:test_self()
  print("hello:".. self.name)
end


--func3
function print_hello()
  print("hello lua")
end

--func4
function _add(a, b)
  return a + b
end


--func5
function run_main()
  print("hello lua")
end

--print_hello()



--[[   p_func_proto(Proto *p) 结果
 Proto: --------------------

//加载的脚本默认是一个执行块  function, 脚本里共有 5 个 func,所以打印出来一共是 6 个
main <@../../../test/scripts/test001.lua:0,0> (24 instructions, 96 bytes at 00000298C3C81450)
0+ param, 3 slots(maxstacksize), 0 upvalue, sizeupvalues[0]
0 local, 15 constants, 5 functions
function Instructions start:  //lua 执行编码
        1       line[3] LOADK                   ; "I'm lua"
        2       line[3] SETGLOBAL               ; mystr
        3       line[5] NEWTABLE        iABC argA[0] argB[0] argC[2]
        4       line[5] SETTABLE        iABC argA[0] argB[-4] argC[-5]  ; "name" "xiaomign"
        5       line[5] SETTABLE        iABC argA[0] argB[-6] argC[-7]  ; "id" 123456
        6       line[5] SETGLOBAL               ; myTable
        7       line[6] NEWTABLE        iABC argA[0] argB[2] argC[0]
        8       line[6] LOADK                   ; "a1"
        9       line[6] LOADK                   ; "a2"
        10      line[6] SETLIST         iABC argA[0] argB[2] argC[1]    ; 1
        11      line[6] SETGLOBAL               ; myAry
        12      line[9] GETGLOBAL               ; myTable
        13      line[11]        CLOSURE                 ; 00000298C3C81510
        14      line[9] SETTABLE        iABC argA[0] argB[-11] argC[1]  ; "test" -
        15      line[13]        GETGLOBAL               ; myTable
        16      line[15]        CLOSURE                 ; 00000298C3C7B8A0
        17      line[13]        SETTABLE        iABC argA[0] argB[-12] argC[1]  ; "test_self" -
        18      line[20]        CLOSURE                 ; 00000298C3C8AAB0
        19      line[18]        SETGLOBAL               ; print_hello
        20      line[24]        CLOSURE                 ; 00000298C3C8AC50
        21      line[22]        SETGLOBAL               ; _add
        22      line[29]        CLOSURE                 ; 00000298C3C8AD10
        23      line[27]        SETGLOBAL               ; run_main
        24      line[29]        RETURN          iABC argA[0] argB[1]
function Instructions end!
constants (15) for 00000298C3C81450:
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
        11      "test"
        12      "test_self"
        13      "print_hello"
        14      "_add"
        15      "run_main"
locals (0) for 00000298C3C81450:
upvalues (0) for 00000298C3C81450:

//func1
function <@../../../test/scripts/test001.lua:9,11> (4 instructions, 16 bytes at 00000298C3C81510)
0 param, 2 slots(maxstacksize), 0 upvalue, sizeupvalues[0]
0 local, 2 constants, 0 function   
function Instructions start:
        1       line[10]        GETGLOBAL               ; print
        2       line[10]        LOADK                   ; "hello myTable"
        3       line[10]        CALL            iABC argA[0] argB[2] argC[1]
        4       line[11]        RETURN          iABC argA[0] argB[1]
function Instructions end!
constants (2) for 00000298C3C81510:
        1       "print"
        2       "hello myTable"
locals (0) for 00000298C3C81510:
upvalues (0) for 00000298C3C81510:

function <@../../../test/scripts/test001.lua:13,15> (6 instructions, 24 bytes at 00000298C3C7B8A0)
1 param, 4 slots(maxstacksize), 0 upvalue, sizeupvalues[0]
1 local, 3 constants, 0 function
function Instructions start:
        1       line[14]        GETGLOBAL               ; print
        2       line[14]        LOADK                   ; "hello:"
        3       line[14]        GETTABLE        iABC argA[3] argB[0] argC[-3]   ; "name"
        4       line[14]        CONCAT          iABC argA[2] argB[2] argC[3]
        5       line[14]        CALL            iABC argA[1] argB[2] argC[1]
        6       line[15]        RETURN          iABC argA[0] argB[1]
function Instructions end!
constants (3) for 00000298C3C7B8A0:
        1       "print"
        2       "hello:"
        3       "name"
locals (1) for 00000298C3C7B8A0:
        0       self    1       6
upvalues (0) for 00000298C3C7B8A0:

function <@../../../test/scripts/test001.lua:18,20> (4 instructions, 16 bytes at 00000298C3C8AAB0)
0 param, 2 slots(maxstacksize), 0 upvalue, sizeupvalues[0]
0 local, 2 constants, 0 function
function Instructions start:
        1       line[19]        GETGLOBAL               ; print
        2       line[19]        LOADK                   ; "hello lua"
        3       line[19]        CALL            iABC argA[0] argB[2] argC[1]
        4       line[20]        RETURN          iABC argA[0] argB[1]
function Instructions end!
constants (2) for 00000298C3C8AAB0:
        1       "print"
        2       "hello lua"
locals (0) for 00000298C3C8AAB0:
upvalues (0) for 00000298C3C8AAB0:

function <@../../../test/scripts/test001.lua:22,24> (3 instructions, 12 bytes at 00000298C3C8AC50)
2 params, 3 slots(maxstacksize), 0 upvalue, sizeupvalues[0]
2 locals, 0 constant, 0 function
function Instructions start:
        1       line[23]        ADD             iABC argA[2] argB[0] argC[1]
        2       line[23]        RETURN          iABC argA[2] argB[2]
        3       line[24]        RETURN          iABC argA[0] argB[1]
function Instructions end!
constants (0) for 00000298C3C8AC50:
locals (2) for 00000298C3C8AC50:
        0       a       1       3
        1       b       1       3
upvalues (0) for 00000298C3C8AC50:

function <@../../../test/scripts/test001.lua:27,29> (4 instructions, 16 bytes at 00000298C3C8AD10)
0 param, 2 slots(maxstacksize), 0 upvalue, sizeupvalues[0]
0 local, 2 constants, 0 function
function Instructions start:
        1       line[28]        GETGLOBAL               ; print
        2       line[28]        LOADK                   ; "hello lua"
        3       line[28]        CALL            iABC argA[0] argB[2] argC[1]
        4       line[29]        RETURN          iABC argA[0] argB[1]
function Instructions end!
constants (2) for 00000298C3C8AD10:
        1       "print"
        2       "hello lua"
locals (0) for 00000298C3C8AD10:
upvalues (0) for 00000298C3C8AD10:
I'm lua  I'm lua
xiaomign
123456
Key:id
Value:123456
Key:test
Value: function
Key:name
Value:xiaomign
Key:test_self
Value: function
请按任意键继续. . .



--]]

