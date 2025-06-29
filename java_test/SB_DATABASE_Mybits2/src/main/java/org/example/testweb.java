package org.example;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import java.util.List;

@Controller
public class testweb{


    @Autowired
    //@Resource
    private DatabaseMapper testMapper;

    @RequestMapping("/hello")
    @ResponseBody
    public String hello(){
        System.out.println("hello springboot!");
        test1();
        return "hello springboot!";
    }
    public void test1(){
        List<String> tables = testMapper.getAllTables();
        System.out.println("数据库表列表：");
        tables.forEach(System.out::println);
    }
}
