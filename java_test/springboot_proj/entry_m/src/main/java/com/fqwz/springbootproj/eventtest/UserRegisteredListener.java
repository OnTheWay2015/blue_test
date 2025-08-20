package com.fqwz.springbootproj.eventtest;

import org.springframework.context.ApplicationListener;
import org.springframework.stereotype.Component;

// 监听 UserRegisteredEvent 事件
@Component
public class UserRegisteredListener implements ApplicationListener<UserRegisteredEvent> {
    @Override
    public void onApplicationEvent(UserRegisteredEvent event) {
        Long userId = event.getUserId();
        System.out.println("UserRegisteredListener 监听到用户注册事件，用户ID：" + userId + "，执行发送欢迎邮件逻辑");
    }
}