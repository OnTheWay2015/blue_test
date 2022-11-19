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

function test01()

end

-- 入口方法 
function run_main()
  test01()
end
 