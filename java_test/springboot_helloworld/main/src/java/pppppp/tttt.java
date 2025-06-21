package pppppp;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

@Controller
public class tttt{
    @RequestMapping("/hello")
    @ResponseBody
    public String hello(){
        System.out.println("hello springboot!");
        return "hello springboot!";
    }
}
