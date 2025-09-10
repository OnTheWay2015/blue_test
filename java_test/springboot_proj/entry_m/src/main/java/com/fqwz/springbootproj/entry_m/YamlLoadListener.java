package com.fqwz.springbootproj.entry_m;


import org.springframework.boot.context.event.ApplicationReadyEvent;
import org.springframework.context.ApplicationEvent;
import org.springframework.context.ApplicationListener;
import org.springframework.context.annotation.Configuration;
import org.springframework.core.env.Environment;

@Configuration
public class YamlLoadListener implements ApplicationListener<ApplicationReadyEvent>   {
    @Override
    public void onApplicationEvent(ApplicationReadyEvent event) {
        Environment env = event.getApplicationContext().getEnvironment();
        //System.out.println("最终配置: " + env.getProperty("custom.property"));
        System.out.println("配置加载完成! \n spring.redis.host:" );
        System.out.println(env.getProperty("spring.redis.host"));
        System.out.printf("springboot_proj.flag:%b\n",env.getProperty("springboot_proj.flag"));


    }
}

