package org.example;
import org.mybatis.spring.annotation.MapperScan;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.web.servlet.ServletComponentScan;
import org.springframework.context.ApplicationContext;

@SpringBootApplication
/*

@MapperScan 是一个 Spring 注解，通过 @Import 导入了 MapperScannerRegistrar 类。其核心属性包括：
basePackages：指定扫描包路径
annotationClass：指定自定义注解过滤
sqlSessionFactoryRef：指定SqlSessionFactory bean名称

 */
@MapperScan("org.example")

//标注 SpringBoot启动时扫描注册注解标注的Web组件,  比如 @WebServlet("/first")
@ServletComponentScan
public class Main {
    public static void main(String[] args) {
        System.out.println("Hello world!");

        ApplicationContext context = SpringApplication.run(Main.class, args);
        //test t = context.getBean(test.class);
        //test t = new test();  //不在 spring 容器中管理 error
        //t.run();
        //test();
        //test1(); //error
    }




}

