package com.fqwz.springbootproj.eventtest;

import org.springframework.context.ApplicationEvent;

// 自定义事件示例，携带用户ID信息
public class UserRegisteredEvent extends ApplicationEvent {
    private Long userId;

    public UserRegisteredEvent(Object source, Long userId) {
        super(source);
        this.userId = userId;
    }

    // getter 方法
    public Long getUserId() {
        return userId;
    }
}
