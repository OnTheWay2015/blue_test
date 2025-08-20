package com.fqwz.springbootproj.entry_m;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

@Controller
public class FirstController{
    @RequestMapping("/hello")
    @ResponseBody
    public String hello(){
        System.out.println("hello com.allen.entry_m.FirstController!");
        return "hello com.allen.entry_m.FirstController!";
    }
}
