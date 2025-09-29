package com.fqwz.springbootproj.entry_m;

import com.fqwz.springbootproj.eventtest.UserService;
 import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.ApplicationEventPublisher;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.RestController;

import java.util.function.IntToLongFunction;

@RestController //要返回json时使用（它组合了@Controller和 @ResponseBody）
//@Controller
public class FirstController{

    @Autowired
    private UserService svr;


    /*
        @RequestMapping：通用注解，支持所有 HTTP 方法（GET、POST、PUT 等），
                        需通过 method 属性指定具体方法（如 method = RequestMethod.GET）。
        @GetMapping：专用注解，仅处理 HTTP GET 请求，
                    是 @RequestMapping(method = RequestMethod.GET) 的简写形式
    * */
    @RequestMapping("/hello")
    @ResponseBody
    public String hello(){
        svr.register(Long.valueOf(123));

         System.out.println("hello com.allen.entry_m.FirstController!");
        return "hello com.allen.entry_m.FirstController!";
    }


    /*
    返回JSON数据的配置
        Spring Boot 默认已包含Jackson库，会自动将对象转换为JSON
        如果需要自定义JSON格式，可以使用@JsonIgnore等Jackson注解控制序列化行为
        对于更复杂的JSON处理，可以创建统一的响应包装类


    如果仍然报错，检查是否缺少jackson-databind依赖（Spring Boot web starter已包含）
    确保 User 类是可序列化的（实现Serializable接口）, 确保User类有正确的getter方法，因为Spring MVC默认使用Jackson库进行序列化时需要getter方法访问属性
    检查是否有循环引用问题（可使用@JsonIgnoreProperties解决）
    * */
    @GetMapping("/getuser")
    public User getUser() {
        // 返回一个User对象实例
        return new User(1, "张三", "zhangsan@example.com");
    }

    // 内部静态User类
    static class User {
        private int id;
        private String name;
        private String email;

        public User(int id, String name, String email) {
            this.id = id;
            this.name = name;
            this.email = email;
        }
        // Getter和Setter方法
        public int getId() { return id; }
        public String getName() { return name; }
        //public String getEmail() { return email; }  //如果少了属性 get方法,序例化json时 就没有这个属性了

        //public void setId(int id) { this.id = id; }
        //public void setName(String name) { this.name = name; }
        //public void setEmail(String email) { this.email = email; }
    }


}
