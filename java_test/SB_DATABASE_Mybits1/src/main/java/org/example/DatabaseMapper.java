package org.example;

import org.apache.ibatis.annotations.Select;

import java.util.List;

//Mapper接口定义   , 在xml配置的 mappers 里指定 class
public interface DatabaseMapper {
    @Select("SHOW TABLES")   //库中所有表名
    List<String> getAllTables();


    @Select("SELECT * FROM users WHERE id = #{id}")
    User getUser(int id);

    @Select("SELECT * FROM users")
    List<User> getAllUser();
}
