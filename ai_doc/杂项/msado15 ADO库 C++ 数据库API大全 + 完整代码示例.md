# msado15 ADO库 C\+\+ 数据库API大全 \+ 完整代码示例

## 一、前置环境说明

### 1\. 库引入方式（VS C\+\+通用）

msado15 是微软原生COM组件，无需额外安装第三方库，VS直接引入即可，两种引入方式任选其一：

```cpp
// 方式1：代码内直接引入（推荐，无需配置项目属性）
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF") rename("BOF","adoBOF")

// 关键说明：
// 1. 重命名EOF/BOF：避免和C++原生文件EOF宏冲突，必加
// 2. 64位系统dll路径一致，32位系统路径不变
// 3. 编译报错缺少dll：直接复制msado15.dll到程序运行目录
```

### 2\. ADO三大核心对象（所有API围绕这三个对象）

1. **\_ConnectionPtr**：数据库连接对象，负责建立/关闭连接、执行无返回值SQL、事务控制

2. **\_RecordsetPtr**：记录集对象，负责查询结果集遍历、分页、字段读取

3. **\_CommandPtr**：命令对象，负责参数化SQL、调用存储过程、防止SQL注入

### 3\. 通用初始化与反初始化（所有代码必须加）

```cpp
// COM初始化（程序入口第一行）
CoInitialize(NULL);
// 程序退出前释放COM
CoUninitialize();
```

---

## 二、msado15 全部常用API分类 \+ C\+\+示例

### 1\. \_ConnectionPtr 连接对象核心API（数据库连接、执行SQL、事务）

|API函数|功能说明|
|---|---|
|Open\(\)|打开数据库连接|
|Close\(\)|关闭数据库连接|
|Execute\(\)|执行增删改SQL，无返回结果集|
|BeginTrans\(\)|开启数据库事务|
|CommitTrans\(\)|提交事务|
|RollbackTrans\(\)|回滚事务|
|State|属性，判断连接是否开启|

#### 完整示例：连接SQL Server \+ 执行增删改 \+ 事务

```cpp
#include <iostream>
#include <windows.h>
#include <comdef.h>
using namespace std;

// 引入ADO库
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF") rename("BOF","adoBOF")

int main()
{
    // 1. 初始化COM
    CoInitialize(NULL);
    _ConnectionPtr pConn(__uuidof(Connection));
    _variant_t vRecordsAffected; // 接收受影响行数

    try
    {
        // 2. 数据库连接字符串（SQL Server）
        // 格式：驱动;服务器;数据库名;用户名;密码
        _bstr_t strConn = "Provider=SQLOLEDB.1;Server=127.0.0.1;Database=testdb;Uid=sa;Pwd=123456;";
        
        // 3. Open API：打开数据库连接
        pConn->Open(strConn, "", "", adConnectUnspecified);
        cout << "数据库连接成功" << endl;

        // ========== 事务API演示 ==========
        pConn->BeginTrans(); // 开启事务

        // 4. Execute API：执行新增SQL
        _bstr_t strInsert = "INSERT INTO userinfo(name,age) VALUES('张三',20)";
        pConn->Execute(strInsert, &vRecordsAffected, adCmdText);
        cout << "新增数据，受影响行数：" << vRecordsAffected.lVal << endl;

        // 执行修改SQL
        _bstr_t strUpdate = "UPDATE userinfo SET age=21 WHERE name='张三'";
        pConn->Execute(strUpdate, &vRecordsAffected, adCmdText);
        cout << "修改数据，受影响行数：" << vRecordsAffected.lVal << endl;

        // 提交事务
        pConn->CommitTrans();
        cout << "事务提交成功" << endl;
    }
    catch (_com_error &e)
    {
        // 事务回滚
        pConn->RollbackTrans();
        cout << "SQL执行异常，事务回滚：" << e.ErrorMessage() << endl;
    }

    // 5. Close API：关闭连接
    if (pConn->State == adStateOpen)
    {
        pConn->Close();
    }
    pConn.Release(); // 释放COM对象
    CoUninitialize();
    return 0;
}
```

### 2\. \_RecordsetPtr 记录集对象核心API（查询数据、遍历结果）

|API函数|功能说明|
|---|---|
|Open\(\)|执行查询SQL，获取结果集|
|adoEOF|判断是否到达结果集末尾|
|MoveNext\(\)|指针下移一行|
|MoveFirst\(\)|指针移动到第一行|
|GetCollect\(\)|根据字段名/下标读取字段值|
|Close\(\)|关闭记录集|

#### 完整示例：查询数据表，遍历所有数据

```cpp
#include <iostream>
#include <windows.h>
#include <comdef.h>
using namespace std;

#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF") rename("BOF","adoBOF")

int main()
{
    CoInitialize(NULL);
    _ConnectionPtr pConn(__uuidof(Connection));
    _RecordsetPtr pRs(__uuidof(Recordset));

    try
    {
        // 连接数据库
        _bstr_t strConn = "Provider=SQLOLEDB.1;Server=127.0.0.1;Database=testdb;Uid=sa;Pwd=123456;";
        pConn->Open(strConn, "", "", adConnectUnspecified);

        // 查询SQL
        _bstr_t strSql = "SELECT id,name,age FROM userinfo";
        
        // Recordset Open API：执行查询，获取结果集
        pRs->Open(strSql, pConn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

        // 遍历结果集
        while (!pRs->adoEOF)
        {
            // GetCollect读取字段，支持字段名/下标两种方式
            long id = pRs->GetCollect("id").lVal;
            CString name = pRs->GetCollect("name").bstrVal;
            int age = pRs->GetCollect("age").lVal;

            cout << "ID:" << id << " 姓名:" << name << " 年龄:" << age << endl;

            pRs->MoveNext(); // 指针下移
        }

        cout << "数据查询完成" << endl;
    }
    catch (_com_error &e)
    {
        cout << "查询异常：" << e.ErrorMessage() << endl;
    }

    // 释放资源
    if (pRs->State == adStateOpen) pRs->Close();
    if (pConn->State == adStateOpen) pConn->Close();
    pRs.Release();
    pConn.Release();
    CoUninitialize();
    return 0;
}
```

### 3\. \_CommandPtr 命令对象核心API（参数化查询、调用存储过程）

|API函数|功能说明|
|---|---|
|ActiveConnection|绑定数据库连接对象|
|CommandText|设置SQL语句/存储过程名|
|CreateParameter\(\)|创建SQL参数，防止SQL注入|
|Parameters\-\>Append\(\)|添加参数到命令集|
|Execute\(\)|执行带参数SQL/存储过程|

#### 示例1：参数化查询（防SQL注入）

```cpp
// 省略头文件和库引入，和上方一致
int main()
{
    CoInitialize(NULL);
    _ConnectionPtr pConn(__uuidof(Connection));
    _CommandPtr pCmd(__uuidof(Command));
    _RecordsetPtr pRs(__uuidof(Recordset));

    // 连接数据库
    _bstr_t strConn = "Provider=SQLOLEDB.1;Server=127.0.0.1;Database=testdb;Uid=sa;Pwd=123456;";
    pConn->Open(strConn, "", "", adConnectUnspecified);

    // 绑定连接
    pCmd->ActiveConnection = pConn;
    // 参数化SQL，?为占位符
    pCmd->CommandText = "SELECT * FROM userinfo WHERE age>?";
    pCmd->CommandType = adCmdText;

    // 创建参数：类型为整型
    _ParameterPtr pParam = pCmd->CreateParameter("", adInteger, adParamInput, 4, 18);
    pCmd->Parameters->Append(pParam);

    // 执行命令，获取结果集
    pRs = pCmd->Execute(NULL, NULL, adCmdText);

    // 遍历结果
    while (!pRs->adoEOF)
    {
        cout << "姓名：" << (_bstr_t)pRs->GetCollect("name") << endl;
        pRs->MoveNext();
    }

    // 资源释放
    pRs->Close();
    pConn->Close();
    pRs.Release();
    pCmd.Release();
    pConn.Release();
    CoUninitialize();
    return 0;
}
```

#### 示例2：调用SQL Server存储过程

```cpp
// 假设存储过程：sp_get_user @age INT
pCmd->ActiveConnection = pConn;
pCmd->CommandText = "sp_get_user";
pCmd->CommandType = adCmdStoredProc; // 指定为存储过程

// 传入输入参数
_ParameterPtr pParamAge = pCmd->CreateParameter("@age", adInteger, adParamInput, 4, 20);
pCmd->Parameters->Append(pParamAge);

// 执行存储过程
pRs = pCmd->Execute(NULL, NULL, adCmdStoredProc);
```

### 4\. 通用字段操作API（空值判断、类型转换）

```cpp
// 1. 判断数据库字段是否为NULL（极易踩坑）
_variant_t varVal = pRs->GetCollect("name");
if (varVal.vt == VT_NULL)
{
    cout << "字段值为空" << endl;
}

// 2. _variant_t通用类型转换（ADO所有返回值都是_variant_t）
// 转字符串：(_bstr_t)varVal
// 转整型：varVal.lVal
// 转浮点：varVal.dblVal
// 转布尔：varVal.boolVal
```

---

## 三、常见数据库连接字符串（适配不同数据库）

### 1\. MySQL 连接字符串（ADO访问MySQL）

```cpp
_bstr_t strConn = "Driver={MySQL ODBC 8.0 Unicode Driver};Server=127.0.0.1;Database=testdb;User=root;Password=123456;Option=3;";
```

### 2\. Access 本地数据库连接

```cpp
_bstr_t strConn = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=D:\\test.mdb;Persist Security Info=False;";
```

---

## 四、开发常见坑点与注意事项

1. **必须重命名EOF/BOF**：否则和C\+\+标准库EOF宏冲突，直接编译报错

2. **必须初始化COM**：缺少CoInitialize会导致所有ADO对象创建失败

3. **必须判断字段空值**：数据库NULL直接取值会程序崩溃

4. **用完必须释放对象**：Recordset、Connection、Command都需要Release，否则内存泄漏

5. **禁止字符串拼接SQL**：务必使用Command参数化接口，防止SQL注入

## 五、代码编译说明

- 编译环境：Visual Studio 2017/2019/2022

- 项目类型：控制台C\+\+项目（MFC/空项目均可）

- 字符集：默认Unicode字符集，无需修改

- 运行依赖：系统自带msado15\.dll，无需额外部署

> （注：部分内容可能由 AI 生成）
