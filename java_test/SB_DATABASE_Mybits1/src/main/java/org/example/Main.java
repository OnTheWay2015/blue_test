package org.example;


import org.apache.ibatis.io.Resources;
import org.apache.ibatis.session.SqlSessionFactory;
import org.apache.ibatis.session.SqlSessionFactoryBuilder;
import org.apache.ibatis.session.SqlSession;

import java.io.IOException;
import java.io.InputStream;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        System.out.println("Hello world!");
        test();
    }


    public static void test() {
       try {
           String resource = "mybatis-config-test.xml";
           InputStream inputStream = Resources.getResourceAsStream(resource);
           SqlSessionFactory sqlSessionFactory = new SqlSessionFactoryBuilder().build(inputStream);

           //测试连接
           testDatabaseConnection(sqlSessionFactory);

           SqlSession sqlSession = sqlSessionFactory.openSession();


           //查看数据表
           // 获取Mapper实例,使用接口注解的查询
           DatabaseMapper mapper = sqlSession.getMapper(DatabaseMapper.class);
           // 执行查询并处理结果,遍历库里的表名
           List<String> tables = mapper.getAllTables();
           System.out.println("数据库表列表：");
           tables.forEach(System.out::println);

            //遍历表里的元素
           // 执行查询并处理结果
           List<User> users = mapper.getAllUser();
           System.out.println("表元素：");
           users.forEach(System.out::println);


           User u = mapper.getUser(1);
           System.out.println("表元素：");
           System.out.println(u);


            //直接使用配置的查询
           //String o = sqlSession.selectOne("db-test.showTables"); //有多个表时，会报错
           //System.out.println(o);
       }
       catch (IOException e){
           System.out.println(e.getMessage());
           System.out.println(e.getStackTrace());
       }
    }

    //测试是否连接成功
    public static void testDatabaseConnection(SqlSessionFactory sqlSessionFactory) {
        try (SqlSession session = sqlSessionFactory.openSession()) {
            // 执行一个简单的查询操作，例如查询数据库版本
            String databaseVersion = session.selectOne("db-test.selectVersion");
            System.out.println("Database version: " + databaseVersion);
        }
    }
}

