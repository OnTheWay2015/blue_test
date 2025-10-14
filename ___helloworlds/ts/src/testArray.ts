/*
在 TypeScript 中遍历数组有多种方式，以下是常用的方法及示例：
1. ‌for 循环‌
    const arr: number[] = [1, 2, 3];
    for (let i = 0; i < arr.length; i++) {
      console.log(arr[i]);
    }
‌   特点‌：通过索引访问，适合需要精确控制遍历顺序或修改原数组的场景。

2. ‌forEach 方法‌
    arr.forEach((item, index) => {
      console.log(item, index);
    });
‌特点‌：简洁，但无法中断遍历（如使用 break）。

3. ‌for...of 循环‌
    for (const item of arr) {
      console.log(item);
    }
    ‌特点‌：直接遍历值，无需索引，支持 break 和 continue。

4. ‌map 方法（生成新数组）‌
    const newArr = arr.map(item => item * 2);
    ‌特点‌：返回新数组，适合需要转换数据的场景。

5. ‌for...in（不推荐用于数组）‌
    for (const index in arr) {
      console.log(arr[index]); // 注意：index 是字符串类型
    }
    ‌注意‌：会遍历继承属性，通常建议用 for...of 替代。

6. ‌迭代器方法（entries/keys/values）‌
    for (const [index, item] of arr.entries()) {
      console.log(index, item);
    }
    ‌特点‌：可同时获取索引和值，兼容 ES6+ 环境。

类型安全提示
TypeScript 会自动推断数组元素的类型，遍历时无需额外类型断言：
const strArr: string[] = ['a', 'b'];
strArr.forEach(s => console.log(s.toUpperCase())); // 自动识别 s 为 string
根据需求选择合适的方法：需中断遍历用 for 或 for...of，仅需值用 forEach，需新数组用 map。

*/

function testArray(){


}