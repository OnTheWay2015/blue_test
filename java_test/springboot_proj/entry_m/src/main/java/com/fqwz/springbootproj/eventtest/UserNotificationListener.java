package com.fqwz.springbootproj.eventtest;

import org.springframework.context.event.EventListener;
import org.springframework.stereotype.Component;

@Component
public class UserNotificationListener {
    // 监听 UserRegisteredEvent 事件
    @EventListener
    public void handleUserRegisteredEvent(UserRegisteredEvent event) {
        Long userId = event.getUserId();
        System.out.println("UserNotificationListener 通过注解监听到用户注册，用户ID：" + userId + "，执行赠送新人优惠券逻辑");
    }
}