package com.fqwz.springbootproj.dir01;



import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

@Controller
public class testpath{
    @RequestMapping("/testpath")
    @ResponseBody
    public String hello(){
        System.out.println("testpath Controller!");
        return "testpath Controller!";
    }
}


