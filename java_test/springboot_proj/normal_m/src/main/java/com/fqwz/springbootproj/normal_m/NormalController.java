package com.fqwz.springbootproj.normal_m;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

@Controller
public class NormalController{
    @RequestMapping("/normal/hello")
    @ResponseBody
    public String hello(){
        System.out.println("normal  Controller!");
        return "normal Controller!";
    }
}
