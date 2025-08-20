package com.fqwz.springbootproj.eventtest;

import org.springframework.context.event.EventListener;
import org.springframework.scheduling.annotation.Async;
import org.springframework.stereotype.Component;

@Component
public class AsyncUserListener {
    @Async // 异步处理事件
    @EventListener
    public void handleAsyncEvent(UserRegisteredEvent event) {
        System.out.println("AsyncUserListener 异步处理用户注册事件，线程名：" + Thread.currentThread().getName());
    }
}