-- 线程


function Func1(param1)
    
    print("Func1 act param1:"..param1)

    Func2(param1 + 10)
    print("Func1 ended.")
    return 30
end

function Func2(value)
    print("Func2 act value:"..value)
    a = coroutine.yield(10, value)
    print("Func2 ended. a:"..a)
end

function test01(arg1,arg2)
    print("test01 act.")

end

g_tt = {a=100,b=300}

-- 入口方法 
function run_main()
    --print(g_tt)
    print({a=1,b=2,c=test01})
    local tt= {a=1,b=2,c=test01}
    tt.c(1,2)

    tt.d = function ()
        local x = 123
        test01(1,2)
    end
    tt1.d()

  --test01()
end
 