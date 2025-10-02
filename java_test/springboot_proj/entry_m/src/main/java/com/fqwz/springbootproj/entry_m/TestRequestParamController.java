package com.fqwz.springbootproj.entry_m;



import org.springframework.web.bind.annotation.*;
import org.springframework.web.multipart.MultipartFile;

import java.util.Map;

@RestController
@RequestMapping("/params")
public class TestRequestParamController {

    //  GET /params/example?id=12345&page=3&sort=desc
    @GetMapping("/example")
    public String handleRequest(
            @RequestParam("id") String userId,
            @RequestParam(value = "page", defaultValue = "1") int page,
            @RequestParam(name = "sort", required = false) String sortOrder) {

        return String.format("ID: %s, Page: %d, Sort: %s", userId, page, sortOrder);
    }


    /*
    Spring 需要参数名称来绑定请求参数
        这里的参数没有用 @RequestParam 绑定名称.所以会报错.
        则需要在编译时加上 -parameters 编译选项,保留参数名信息.当前通过pom配置插件添加!!
        <build>
            <plugins>
                <plugin>
                    <groupId>org.apache.maven.plugins</groupId>
                    <artifactId>maven-compiler-plugin</artifactId>
                    <configuration>
                        <compilerArgs>
                            <arg>-parameters</arg>
                        </compilerArgs>
                    </configuration>
                </plugin>
            </plugins>
        </build>
    * */
    // 1. 接收表单数据（application/x-www-form-urlencoded）
    @PostMapping("/form-data")
    public String handleFormData(@RequestParam String username,
                                 @RequestParam String password) {
        return "Received: " + username + ", " + password;
    }

    // 2. 接收JSON数据（application/json）
    @PostMapping("/json-data")
    public String handleJsonData(@RequestBody Map<String, Object> payload) {
        return "Received JSON: " + payload.toString();
    }

    // 3. 接收多部分文件（multipart/form-data）
    @PostMapping("/upload")
    public String handleFileUpload(@RequestParam("file") MultipartFile file) {
        return "Received file: " + file.getOriginalFilename();
    }

    // 4. 接收原始请求体
    @PostMapping("/raw-data")
    public String handleRawData(@RequestBody String rawBody) {
        return "Raw body: " + rawBody;
    }


}
/*

必填参数请求：
    GET /example?id=12345
带默认值参数请求（page不传时默认为1）
    GET /example?id=12345&sort=asc

完整参数请求：
    GET /example?id=12345&page=3&sort=desc
错误请求（缺少必填参数）：
    GET /example?page=2
* */

/*
@RequestParam
    name属性：
        用于指定HTTP请求中参数的名称
        当请求参数名与方法参数名不一致时使用
        示例：@RequestParam(name = "sort")表示接收名为"sort"的请求参数
    value属性：
        是name属性的别名（功能完全相同）
        可以互换使用，@RequestParam("sort")等价于@RequestParam(value="sort")
        通常更简洁的写法是直接省略属性名：@RequestParam("sort")
        这两个属性都用于将HTTP请求参数绑定到控制器方法的参数上，当 required=false 时表示该参数是可选的

* */
