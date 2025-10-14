/*
TypeScript 中的 Map 是一种键值对集合，提供了比普通对象更灵活的存储和操作方式。以下是其主要用法和特性：
1. 基本定义与创建
    Map 对象通过泛型指定键值类型，可使用 new Map() 初始化空集合，或传入初始键值对数组：
    let map: Map<string, number> = new Map(); // 空Map
    let initializedMap = new Map([['apple', 5], ['banana', 8]]); // 带初始值:ml-citation{ref="4,7" data="citationList"}
2. 核心操作方法
    ‌增/改‌：map.set(key, value)
    ‌查‌：map.get(key)（不存在返回 undefined）
    ‌删‌：map.delete(key)（返回布尔值表示是否成功）
    ‌清空‌：map.clear()
    ‌检查存在‌：map.has(key)
3. 特性与优势
    ‌键类型灵活‌：支持对象、函数等任意类型作为键（普通对象仅允许字符串或 Symbol）
    ‌顺序保留‌：遍历时按插入顺序返回键值对
    ‌直接迭代‌：可通过 map.keys()、map.values() 或 map.entries() 获取迭代器
    ‌大小获取‌：map.size 直接返回元素数量

4. 类型安全场景
    Map 的泛型参数确保键值类型一致，例如：
        const typedMap = new Map<string, Date>(); 
        typedMap.set('event', new Date()); // 值必须为Date类型
5. 与数组转换
    ‌Map → 数组‌：Array.from(map) 或 [...map.entries()]
    ‌数组 → Map‌：new Map(array)（数组需为 [key, value] 结构）

Map 适用于需要频繁增删键值对或键类型复杂的场景，相比普通对象提供更强的类型控制和操作便利性

*/



function testMap() {
    //Map 对象通过泛型指定键值类型，可使用 new Map() 初始化空集合，或传入初始键值对数组：
    let map: Map<string, number> = new Map(); // 空Map
    let initializedMap = new Map([['apple', 5], ['banana', 8]]); // 带初始值:ml-citation{ref="4,7" data="citationList"}

/*
    ‌增/改‌：map.set(key, value)
    ‌查‌：map.get(key)（不存在返回 undefined）
    ‌删‌：map.delete(key)（返回布尔值表示是否成功）
    ‌清空‌：map.clear()
    ‌检查存在‌：map.has(key)
*/

    //遍历示例
    const userMap = new Map<number, { name: string }>();
    userMap.set(1001, { name: 'Alice' });
    
    //forEach 遍历键值对
    userMap.forEach((value, key) => console.log(key, value));

    // 使用 for...of 迭代 key, value
    for (const [key, value] of userMap.entries()) {
        console.log(key, value);
    }

    // 使用for...of 迭代 key
    for (const key of userMap.keys()) {
        console.log(key);
    }

    // 使用for...of 迭代 value 
    for (const value of userMap.values()) {
        console.log(value);
    }

}

