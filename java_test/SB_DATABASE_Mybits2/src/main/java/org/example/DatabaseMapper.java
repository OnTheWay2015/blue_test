package org.example;

import org.apache.ibatis.annotations.Select;

import java.util.List;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import org.apache.ibatis.annotations.Mapper;

@Mapper  //MyBatis-Plus
public interface DatabaseMapper extends BaseMapper<User> {
    // 这里可以定义额外的自定义 SQL 方法，也可以不用定义任何内容，直接使用 BaseMapper 的方法。
    @Select("SHOW TABLES")   //库中所有表名
    List<String> getAllTables();


    @Select("SELECT * FROM users WHERE id = #{id}")
    User getUser(int id);

    @Select("SELECT * FROM users")
    List<User> getAllUser();


}

