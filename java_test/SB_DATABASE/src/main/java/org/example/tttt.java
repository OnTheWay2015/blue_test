package org.example;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;


import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;

import java.util.List;
import java.util.Map;

@Controller
public class tttt{

    @Autowired   //自动实例化
    private JdbcTemplate jdbcTemplate;


    @RequestMapping("/db")
    @ResponseBody
    public String hello(){
        System.out.println("hello springboot DB!");

        MysqlPool p = new MysqlPool();
        p.run();

       //List<Map<String,Object>>  list = getUsers();
        return "hello springboot DB!";
    }

    /*
     * List 里的对象是Map对象，而Map对象是
     * 由一个String类型的键和Object类型的值组成
     * */
        public List<Map<String,Object>> getUsers(){
            String sql="select * from system_users";//SQL查询语句
            List<Map<String,Object>> list=jdbcTemplate.queryForList(sql);
            return list;
        }
}
