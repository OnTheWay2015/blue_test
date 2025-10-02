package com.fqwz.springbootproj.redislistmq;


import com.fqwz.springbootproj.constant.MessageChannel;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.stereotype.Component;
@Component
public class RedisMqMsgProducer {
    @Autowired
    private RedisTemplate<String, String> redisTemplate;
    public void sendMessage1( String message) {
        redisTemplate.convertAndSend(MessageChannel.MSG_C1.getName(), message);
    }
    public void sendMessage2( String message) {
        redisTemplate.convertAndSend(MessageChannel.MSG_C2.getName(), message);
    }
}

