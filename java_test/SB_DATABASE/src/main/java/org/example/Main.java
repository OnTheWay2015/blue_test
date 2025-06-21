package org.example;



import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.web.servlet.ServletComponentScan;


@SpringBootApplication
/*
    标注启动类
    在启动时会做注解扫描(@Controller、@Service、@Repository…)，
    扫描位置为同包或者同级包下的注解，所以我们要在启动类同级或同级包下编写代码。
* */

@ServletComponentScan
//标注 SpringBoot启动时扫描注册注解标注的Web组件,  比如 @WebServlet("/first")


public class Main {
    public static void main(String[] args) {
        System.out.println("Hello world!");
        SpringApplication.run(Main.class, args);
    }


}

