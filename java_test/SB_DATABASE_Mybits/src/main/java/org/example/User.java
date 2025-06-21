package org.example;

//不管结构里定义的属性，对应到表里的元素例是多了还是少了
//只要是例和结构里的属性能对应上,就会把对应例数据填到属性上
public class User {
    private int id;
    private String name;
    private String name1;

    public int getId() {
        return id;
    }
    public void setId(int id) {
        this.id = id;
    }
    public String getName() {
        return name;
    }
    public void setName(String username) {
        this.name = username;
    }

    public String getName1() {
        return name1;
    }
    public void setName1(String username) {
        this.name1 = username;
    }
}
