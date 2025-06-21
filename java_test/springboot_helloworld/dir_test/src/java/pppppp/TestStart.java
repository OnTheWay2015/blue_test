package pppppp;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;


//文件和启动类不在同一层级, SpringBoot 注解不会生效
@Controller
public class TestStart {
    @RequestMapping("/hello_01")
    @ResponseBody
    public String hello(){
        System.out.println("hello_01 springboot!");
        return "hello_01  springboot!";
    }
}