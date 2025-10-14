
/*
TypeScript（TS）支持多种数据类型，包括基本类型和高级类型，以下是主要分类及说明：
基本数据类型
    ‌number‌ 表示数值类型，包括整数、浮点数、二进制（0b1010）、八进制（0o744）、十六进制（0xf00d）等
    ‌string‌ 字符串类型，支持单引号、双引号及模板字符串（反引号）嵌入表达式（如`${name}年龄${years}`）
    ‌boolean‌ 布尔值，仅限true或false，不可用其他类型隐式转换
    ‌null/undefined‌ 默认是所有类型的子类型，但开启--strictNullChecks后仅能赋值给自身或void

复合数据类型
‌   array‌ 定义方式有两种：number[]或泛型Array<number>，元素类型需一致
   tuple‌ 固定长度和类型的数组，如[string, number]，位置类型必须匹配
   object‌ 非原始类型的对象，如{ name: string }，或通过接口定义结构

特殊类型
    ‌any‌ 动态类型，绕过类型检查，慎用以避免失去TS优势
    ‌void ‌表示无返回值，常用于函数声明（如function log(): void）
    ‌never‌ 表示永不返回的函数（如抛出异常或无限循环）

    ‌enum‌ 枚举类型，定义命名常量集合（如enum Roles { ADMIN, USER }）

其他类型
‌    symbol‌ 唯一标识符，通过Symbol()创建，常用于对象属性键
    ‌bigint‌ ES2020引入，表示大整数（如10n），解决number溢出问题。

联合类型‌
    组合多种类型（如string | number），变量可接受其中任意类型

TS通过静态类型检查增强代码可靠性，变量声明后类型不可变更（如let a: string后续不能赋值为number

*/





class helloworld{
    private key:string = "hello"
    public run():void{
        let self = this;
        console.log(self.key);
    }
}


function go() {
    let t = new helloworld()
    t.run();
}




