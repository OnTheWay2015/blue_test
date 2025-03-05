//#include "DBPool.h"
#include <stdio.h> 
 

#include <mysql++.h>
#include <iostream>
  
using namespace std;
int main()
{
  const char* db = 0, *server = 0, *user = 0, *password = "";
  db = "test";
  //server = "localhost";
  server = "127.0.0.1";
  user = "root";
  password = "123456";
  auto port = 3306;
     
  mysqlpp::Connection conn(false);
  if (conn.connect(db, server, user, password,port)) 
  {
    cout << "connect db succeed. " << endl;
    mysqlpp::Query query = conn.query("SELECT * FROM student");
    if (mysqlpp::StoreQueryResult res = query.store()) 
    {
      cout.setf(ios::left); //left    输出调整为左对齐.
      cout << setw(31) << "Sid" <<
      setw(10) << "Sname" <<         //setw(10)设置输出的域宽，不足补齐
      setw(10) << "Sage" <<
      setw(10) << "Sgender" <<                 
      setw(10) << "SDepartment" << endl;
 
      mysqlpp::StoreQueryResult::const_iterator it;
      for (it = res.begin(); it != res.end(); ++it) 
      {
        mysqlpp::Row row = *it;
        cout << setw(30) << row[0] << ' ' <<
        setw(9) << row[1] << ' ' <<
        setw(9) << row[2] << ' ' <<
        setw(9) << row[3] << ' ' <<
        setw(9) << row[4] << ' ' <<
        endl;
      }
    }
  } 
  else 
  {
      cout << "connect db fail: "<< conn.error() << endl;
  }
  return 0;
}


//int main111(int argc, char *argv[]) 
//{
//  //初始化连接，创建参数中maxSize一半的连接
//  connpool.initPool("tcp://127.0.0.1:3306", "root", "123456", 100);
//
//  Connection *con = nullptr;
//  Statement *state= nullptr;
//  ResultSet *result= nullptr;
//  con = connpool.GetConnection();  // get a db conn
//  for (int i = 0; i < 1000; i++) {
//    state = con->createStatement();
//    state->execute("use mysql");
//
//    // 查询
//    result = state->executeQuery("select host,user from user");
//
//    // 输出查询
//    while (result->next()) {
//      try {
//        string user = result->getString("user");
//        string name = result->getString("host");
//        cout << user << " : " << name << endl;
//      } catch (sql::SQLException &e) {
//        std::cout << e.what() << std::endl;
//      }
//    }
//
//    /*result = state->executeQuery("select cust_id,cust_name from customers");
//    while (result->next())
//    {
//        try{
//            string user = result->getString("cust_id");
//            string name = result->getString("cust_name");
//            cout << user << " : " << name << endl;
//        }catch(sql::SQLException& e){
//          std::cout << e.what() << std::endl;
//        }
//    }
//*/
//    std::cout << i << std::endl;
//  }
//
//  if (result) delete result;
//  if (state) delete state;
//  connpool.ReleaseConnection(con);
//
//  return 0;
//}


