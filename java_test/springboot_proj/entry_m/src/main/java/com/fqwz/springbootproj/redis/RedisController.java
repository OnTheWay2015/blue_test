package com.fqwz.springbootproj.redis;

import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.web.bind.annotation.*;
import java.util.concurrent.TimeUnit;

@RestController
@RequestMapping("/redis")
public class RedisController {

     private final RedisTemplate<String, Object> redisTemplate;

    public RedisController(RedisTemplate<String, Object> redisTemplate) {
        this.redisTemplate = redisTemplate;
    }

    @PostMapping("/set")
    public String set(@RequestParam String key, @RequestParam String value) {
        redisTemplate.opsForValue().set(key, value);
        return "OK";
    }

    @PostMapping("/setWithTTL")
    public String setWithTTL(@RequestParam String key,
                             @RequestParam String value,
                             @RequestParam long ttl) {
        redisTemplate.opsForValue().set(key, value, ttl, TimeUnit.SECONDS);
        return "OK";
    }

    @GetMapping("/get")
    public Object get(@RequestParam String key) {
        return redisTemplate.opsForValue().get(key);
    }

    @DeleteMapping("/delete")
    public Boolean delete(@RequestParam String key) {
        return redisTemplate.delete(key);
    }

    @GetMapping("/hasKey")
    public Boolean hasKey(@RequestParam String key) {
        return redisTemplate.hasKey(key);
    }
}
