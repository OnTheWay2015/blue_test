package com.fqwz.springbootproj.entry_m;

import com.fqwz.springbootproj.eventtest.UserService;
import com.fqwz.springbootproj.redislistmq.RedisMqMsgProducer;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.ApplicationEventPublisher;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import java.util.function.IntToLongFunction;

@Controller
public class FirstController{

    @Autowired
    private UserService svr;

    @Autowired
    private RedisMqMsgProducer producer;

    @RequestMapping("/hello")
    @ResponseBody
    public String hello(){
        svr.register(Long.valueOf(123));

        producer.sendMessage("msg");
        System.out.println("hello com.allen.entry_m.FirstController!");
        return "hello com.allen.entry_m.FirstController!";
    }
}
