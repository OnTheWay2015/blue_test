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
    private RedisMqMsgConsumer1 consumer1;
    @Autowired
    private RedisMqMsgConsumer2 consumer2;


    /*
@ConditionalOnMissingBean 是用于修饰Bean的一个注解，
在项目启动时开始自动化配置注册生成Bean，当某个类型的Bean被注册之后，
如果再注册相同类型的Bean就会失败，该注解会保证Spring容器中只有一个Bean类型的实例，
当注册多个相同类型的Bean时，会出现异常。
当一个接口有多个实现类时，如果只希望它有一个实现类，那就在其他的各个实现类上加上这个注解。


@ConditionalOnMissingBean 作用在@Bean定义上，也就是说在容器加载它作用的Bean时，
检查容器中是否存在目标类型（@ConditionalOnMissingBean注解的value值）的Bean了，
如果存在则跳过Bean的默认加载动作，如果不存在则加载该Bean完成注册。
    * */
    @Bean
    //@ConditionalOnMissingBean
    public RedisMessageListenerContainer container1(RedisConnectionFactory connectionFactory,
                                                   MessageListenerAdapter listenerAdapter1) {
        RedisMessageListenerContainer container = new RedisMessageListenerContainer();
        container.setConnectionFactory(connectionFactory);
        container.addMessageListener(listenerAdapter1, topic1());
         return container;
    }

    @Bean
    public RedisMessageListenerContainer container2(RedisConnectionFactory connectionFactory,
                                                    MessageListenerAdapter listenerAdapter2) {
        RedisMessageListenerContainer container = new RedisMessageListenerContainer();
        container.setConnectionFactory(connectionFactory);
        container.addMessageListener(listenerAdapter2, topic2());
        return container;
    }

    @Bean
    public MessageListenerAdapter listenerAdapter1() {
        //指定响应发布消息的方法名为 RedisMqMsgConsumer 类型里的 onMessage
        return new MessageListenerAdapter(consumer1, "onMessage");
    }
    @Bean
    public MessageListenerAdapter listenerAdapter2() {
        //指定响应发布消息的方法名为 RedisMqMsgConsumer 类型里的 onMessage
        return new MessageListenerAdapter(consumer2, "onMessage");
    }

    @Bean
    public ChannelTopic topic1() {
        //提定监听 redis 发布频道名为 RedisMqMsgProducerKey
        return new ChannelTopic(MessageChannel.MSG_C1.getName());
    }
    @Bean
    public ChannelTopic topic2() {
        //提定监听 redis 发布频道名为 RedisMqMsgProducerKey
        //return new ChannelTopic(MessageChannel.MSG_C1.getName());
        return new ChannelTopic(MessageChannel.MSG_C2.getName());
    }

}