package com.fqwz.springbootproj.redislistmq;


import com.fqwz.springbootproj.constant.MessageChannel;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.autoconfigure.AutoConfiguration;
import org.springframework.boot.autoconfigure.AutoConfigureAfter;
import org.springframework.boot.autoconfigure.condition.ConditionalOnMissingBean;
import org.springframework.boot.autoconfigure.condition.ConditionalOnProperty;
import org.springframework.boot.autoconfigure.data.redis.RedisAutoConfiguration;
import org.springframework.boot.autoconfigure.jdbc.DataSourceAutoConfiguration;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.data.redis.connection.RedisConnectionFactory;
import org.springframework.data.redis.listener.ChannelTopic;
import org.springframework.data.redis.listener.RedisMessageListenerContainer;
import org.springframework.data.redis.listener.adapter.MessageListenerAdapter;
import org.springframework.data.redis.serializer.StringRedisSerializer;

//@ConditionalOnProperty("springboot_proj.flag")

@Configuration
//@AutoConfiguration
//@AutoConfiguration(after = RedisAutoConfiguration.class )

//@AutoConfiguration
//@AutoConfigureAfter(RedisAutoConfiguration.class)
public class RedisMqConfig {

    @Autowired
    private RedisMqMsgConsumer consumer;
    @Bean
    @ConditionalOnMissingBean
    public RedisMessageListenerContainer container(RedisConnectionFactory connectionFactory,
                                                   MessageListenerAdapter listenerAdapter) {
        RedisMessageListenerContainer container = new RedisMessageListenerContainer();
        container.setConnectionFactory(connectionFactory);
        container.addMessageListener(listenerAdapter, topic());
        return container;
    }
    @Bean
    public MessageListenerAdapter listenerAdapter() {
        //指定响应发布消息的方法名为 RedisMqMsgConsumer 类型里的 onMessage
        return new MessageListenerAdapter(consumer, "onMessage");
    }
    @Bean
    public ChannelTopic topic() {
        //提定监听 redis 发布频道名为 RedisMqMsgProducerKey
        return new ChannelTopic(MessageChannel.MSG_C1.getName());
    }
}