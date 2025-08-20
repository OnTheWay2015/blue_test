package com.fqwz.springbootproj.eventtest;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.ApplicationEventPublisher;
import org.springframework.stereotype.Service;

@Service
public class UserService {
    @Autowired
    private ApplicationEventPublisher publisher;
     // 用户注册后发布事件
    public void register(Long userId) {
        // 执行注册逻辑...
        System.out.println("UserService 用户 " + userId + " 注册成功, 发布事件!");
        // 发布事件
        publisher.publishEvent(new UserRegisteredEvent(this, userId));
    }
}