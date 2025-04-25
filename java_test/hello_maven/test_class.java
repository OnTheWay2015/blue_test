public class test_class {
    /*
    一般情况下，类都有一个或者多个构造方法，而在定义类的时候也不一定非要定义构造方法，
    如果在定义类的时候没有定义构造方法，那么系统会自动为这个类产生一个构造方法，
    称为 默认构造方法
    比如 test() {}
    * */

    int v1;
    int v2;
    String v3;
    test_class(int a,int b)
    {
        this.v1 = a;
        this.v2 = b;
        this.v3 = "123";
    }
    public void call(int a)
    {
        call(this.v3);
    }

    //通过参数的不同重载方法
    public void call(String a)
    {
        System.out.println("helloworld");
    }
    public void Run(String[] args){
        call(this.v1);
    }


}
