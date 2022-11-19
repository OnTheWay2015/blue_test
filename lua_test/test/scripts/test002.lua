--使用 c++ 方法  lua_register(pState, "average", Average)

avg, sum = average(10,20,30,40,50);
print("The average is ", avg)
print("The sum is ", sum)
