package org.example;

import com.mysql.cj.jdbc.MysqlConnectionPoolDataSource;

import java.sql.*;

//import java.sql.*;
public class MysqlPool {
    // 数据库配置
    //private static final String URL = "jdbc:mysql://localhost:3306/testdb?useSSL=false&serverTimezone=UTC";

    private static final String JDBC_URL = "jdbc:mysql://192.168.232.123:3306"
            +"?useSSL=false&serverTimezone=UTC"
            +"&useUnicode=true&characterEncoding=UTF-8";

    //private static final String URL = "jdbc:mysql://192.168.232.123:3306/ruoyi-vue-pro?useSSL=false&serverTimezone=UTC";
    private static final String USER = "mysql";
    private static final String PASSWORD = "123456";
    private static final String DB= "ttttDB";

    //private static final String URL = "jdbc:mysql://192.168.232.123:3306/"+DB+"?useSSL=false&serverTimezone=UTC";
    //增加字符编码指定,数据库/表/列使用latin1等非UTF-8字符集，无法处理中文的3字节编码
    private static final String URL = "jdbc:mysql://192.168.232.123:3306/"+DB
            +"?useSSL=false&serverTimezone=UTC"
            +"&useUnicode=true&characterEncoding=UTF-8";
    public void run() {

        try {
            if (!databaseExists(DB)){
                System.out.println("创建数据库:"+DB);
                createDatabase(DB);
            }else{
                System.out.println("数据库已存在:"+DB);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }


        // 1. 创建连接池数据源
        MysqlConnectionPoolDataSource dataSource = new MysqlConnectionPoolDataSource();
        //dataSource.setURL("jdbc:mysql://localhost:3306/ruoyi-vue-pro");
        //dataSource.setURL("jdbc:mysql://192.168.232.123:3306/ruoyi-vue-pro");
        //dataSource.setUser("mysql");
        //dataSource.setPassword("123456");

        dataSource.setURL(URL);
        dataSource.setUser(USER);
        dataSource.setPassword(PASSWORD);
        // 2. 从连接池获取连接
        try (Connection conn = dataSource.getConnection()) {
            System.out.println("从连接池获取连接成功");
            // 执行数据库操作...

            // 3. 创建表
            createTable(conn);
            System.out.println("创建表 ok");

            // 4. 插入数据
            insertData(conn, 1, "张三");
            insertData(conn, 2, "老张");
            System.out.println("插入数据 ok");

            // 5. 查询数据
            queryData(conn);
            System.out.println("查询数据 ok");

            // 6. 更新数据
            updateData(conn, 1, "李四");
            System.out.println("更新数据 ok");

            // 7. 删除数据
            //deleteData(conn, 1);
            //deleteData(conn, 2);
            //System.out.println("删除数据 ok");
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public static boolean databaseExists(String dbName) throws SQLException {
        try (Connection conn = DriverManager.getConnection(JDBC_URL, USER, PASSWORD);
             ResultSet rs = conn.getMetaData().getCatalogs()) {
            while (rs.next()) {
                if (rs.getString(1).equalsIgnoreCase(dbName)) {
                    return true;
                }
            }
        }
        return false;
    }

    public static void createDatabase(String dbName) throws SQLException {
        try (Connection conn = DriverManager.getConnection(JDBC_URL, USER, PASSWORD);
             Statement stmt = conn.createStatement()) {
            stmt.executeUpdate("CREATE DATABASE IF NOT EXISTS " + dbName);
        }
    }

    public static void dropDatabase(String dbName) throws SQLException {
        try (Connection conn = DriverManager.getConnection(JDBC_URL, USER, PASSWORD);
             Statement stmt = conn.createStatement()) {
            stmt.executeUpdate("DROP DATABASE IF EXISTS " + dbName);
        }
    }
    private static void createTable(Connection conn) throws SQLException {
        String sql = "CREATE TABLE IF NOT EXISTS users (" +
                "id INT PRIMARY KEY, " +
                "name VARCHAR(20))" +
                 " DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='测试表'";
        try (Statement stmt = conn.createStatement()) {
            stmt.executeUpdate(sql);
        }
    }

    private static void insertData(Connection conn, int id, String name) throws SQLException {
        String sql = "INSERT INTO users VALUES (?, ?)";
        try (PreparedStatement pstmt = conn.prepareStatement(sql)) {
            pstmt.setInt(1, id);
            pstmt.setString(2, name);
            pstmt.executeUpdate();
        }
    }

    private static void queryData(Connection conn) throws SQLException {
        String sql = "SELECT * FROM users";
        try (Statement stmt = conn.createStatement();
             ResultSet rs = stmt.executeQuery(sql)) {
            while (rs.next()) {
                System.out.println("ID: " + rs.getInt("id") +
                        ", Name: " + rs.getString("name"));
            }
        }
    }

    private static void updateData(Connection conn, int id, String newName) throws SQLException {
        String sql = "UPDATE users SET name = ? WHERE id = ?";
        try (PreparedStatement pstmt = conn.prepareStatement(sql)) {
            pstmt.setString(1, newName);
            pstmt.setInt(2, id);
            pstmt.executeUpdate();
        }
    }

    private static void deleteData(Connection conn, int id) throws SQLException {
        String sql = "DELETE FROM users WHERE id = ?";
        try (PreparedStatement pstmt = conn.prepareStatement(sql)) {
            pstmt.setInt(1, id);
            pstmt.executeUpdate();
        }
    }

}






