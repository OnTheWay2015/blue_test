#include "stdafx.h"
#include <iostream>
#include <unordered_map>
#include <memory>
#include <string>
#include <stdexcept>
#include <type_traits>

// ===================== 1. 抽象产品基类（所有产品的通用接口） =====================
class Product {
public:
    // 虚析构函数：确保子类析构被正确调用
    virtual ~Product() = default;

    // 纯虚函数：产品的核心接口
    virtual void doSomething() const = 0;

    // 获取产品类型标识（可选，便于调试）
    virtual std::string getType() const = 0;
};

// ===================== 2. 具体产品类（继承自Product） =====================
// 产品A
class ProductA : public Product {
public:
    void doSomething() const override {
        std::cout << "ProductA: 执行A类的核心逻辑" << std::endl;
    }

    std::string getType() const override {
        return "ProductA";
    }
};

// 产品B
class ProductB : public Product {
public:
    void doSomething() const override {
        std::cout << "ProductB: 执行B类的核心逻辑" << std::endl;
    }

    std::string getType() const override {
        return "ProductB";
    }
};

// 产品C（带构造参数的示例）
class ProductC : public Product {
private:
    int m_param;  // 自定义参数
    std::string m_name;
public:
    // 带参数的构造函数
    ProductC(int param, const std::string& name) : m_param(param), m_name(name) {}

    void doSomething() const override {
        std::cout << "ProductC: 执行C类的核心逻辑 | 参数："
            << m_param << "，名称：" << m_name << std::endl;
    }

    std::string getType() const override {
        return "ProductC";
    }
};

// ===================== 3. 通用工厂类模板（核心） =====================
template <typename BaseProduct>
class Factory {
public:
    // 定义创建函数的类型（无参数）
    using CreateFunc = std::unique_ptr<BaseProduct>(*)();

    // 定义带参数的创建函数类型（示例：int + string）
    template <typename... Args>
    using CreateFuncWithArgs = std::unique_ptr<BaseProduct>(*)(Args...);

    // 单例模式（可选，全局唯一工厂）
    static Factory& getInstance() {
        static Factory instance;
        return instance;
    }

    // 禁止拷贝和移动（单例安全）
    Factory(const Factory&) = delete;
    Factory& operator=(const Factory&) = delete;
    Factory(Factory&&) = delete;
    Factory& operator=(Factory&&) = delete;

    // ========== 注册无参数的类 ==========
    template <typename DerivedProduct>
    void registerProduct(const std::string& key) {
        // 静态断言：确保DerivedProduct继承自BaseProduct
        static_assert(std::is_base_of_v<BaseProduct, DerivedProduct>,
            "DerivedProduct must inherit from BaseProduct");

        // 注册创建函数：创建DerivedProduct实例并返回基类指针
        m_createMap[key] = []() -> std::unique_ptr<BaseProduct> {
            return std::make_unique<DerivedProduct>();
            };
    }

    // ========== 注册带参数的类（模板重载） ==========
    template <typename DerivedProduct, typename... Args>
    void registerProductWithArgs(const std::string& key, CreateFuncWithArgs<Args...> createFunc) {
        static_assert(std::is_base_of_v<BaseProduct, DerivedProduct>,
            "DerivedProduct must inherit from BaseProduct");

        // 存储带参数的创建函数（通过map的<void*>键区分，简化示例）
        m_createMapWithArgs[key] = reinterpret_cast<void*>(createFunc);
    }


    //template <typename DerivedProduct>
    //void registerProductWithArgs(const std::string& key, CreateFuncWithArgs<int,std::string> createFunc) {
    //    static_assert(std::is_base_of_v<BaseProduct, DerivedProduct>,
    //        "DerivedProduct must inherit from BaseProduct");

    //    // 存储带参数的创建函数（通过map的<void*>键区分，简化示例）
    //    m_createMapWithArgs[key] = reinterpret_cast<void*>(createFunc);
    //}

    // ========== 创建无参数的实例 ==========
    std::unique_ptr<BaseProduct> createProduct(const std::string& key) {
        auto it = m_createMap.find(key);
        if (it == m_createMap.end()) {
            throw std::invalid_argument("Factory: 未注册的产品类型 - " + key);
        }
        // 调用创建函数生成实例
        return it->second();
    }

    // ========== 创建带参数的实例 ==========
    template <typename... Args>
    std::unique_ptr<BaseProduct> createProductWithArgs(const std::string& key, Args&&... args) {
        auto it = m_createMapWithArgs.find(key);
        if (it == m_createMapWithArgs.end()) {
            throw std::invalid_argument("Factory: 未注册的带参数产品类型 - " + key);
        }

        // 转换为对应参数类型的创建函数并调用
        auto createFunc = reinterpret_cast<CreateFuncWithArgs<Args...>>(it->second);
        return createFunc(std::forward<Args>(args)...);
    }

    // ========== 获取所有已注册的产品类型 ==========
    std::vector<std::string> getRegisteredProducts() const {
        std::vector<std::string> keys;
        for (const auto& pair : m_createMap) {
            keys.push_back(pair.first);
        }
        for (const auto& pair : m_createMapWithArgs) {
            keys.push_back(pair.first);
        }
        return keys;
    }

private:
    // 私有构造函数（单例）
    Factory() = default;

    // 存储无参数创建函数的映射（键：产品标识，值：创建函数）
    std::unordered_map<std::string, CreateFunc> m_createMap;

    // 存储带参数创建函数的映射（简化版，实际可优化为类型安全的多态）
    std::unordered_map<std::string, void*> m_createMapWithArgs;
};

// ===================== 4. 辅助函数：简化带参数产品的创建函数定义 =====================
// 为ProductC生成创建函数（适配工厂的参数类型）
std::unique_ptr<Product> createProductC(int param, std::string& name) {
    return std::make_unique<ProductC>(param, name);
}

// ===================== 5. 测试代码 =====================
int test_templatefactory() {
    try {
        // 获取工厂单例（BaseProduct=Product）
        auto& factory = Factory<Product>::getInstance();

        // ========== 步骤1：注册产品 ==========
        // 注册无参数产品A、B
        factory.registerProduct<ProductA>("ProductA");
        factory.registerProduct<ProductB>("ProductB");

        // 注册带参数产品C（关联自定义创建函数）
        factory.registerProductWithArgs<ProductC>(
            "ProductC", createProductC
        );
        //factory.registerProductWithArgs<ProductC, int, std::string>(
        //    "ProductC", createProductC
        //);

        // ========== 步骤2：打印已注册产品 ==========
        std::cout << "已注册的产品类型：" << std::endl;
        for (const auto& type : factory.getRegisteredProducts()) {
            std::cout << "- " << type << std::endl;
        }
        std::cout << "------------------------" << std::endl;

        // ========== 步骤3：创建无参数实例并调用接口 ==========
        std::unique_ptr<Product> productA = factory.createProduct("ProductA");
        std::unique_ptr<Product> productB = factory.createProduct("ProductB");
        productA->doSomething();
        productB->doSomething();
        std::cout << "------------------------" << std::endl;

        // ========== 步骤4：创建带参数实例并调用接口 ==========
        std::unique_ptr<Product> productC = factory.createProductWithArgs(
            "ProductC", 100, "TestC"
        );
        productC->doSomething();
        std::cout << "------------------------" << std::endl;

        // ========== 步骤5：测试错误场景（未注册的产品） ==========
        // factory.createProduct("ProductD"); // 抛出异常：未注册的产品类型
    }
    catch (const std::exception& e) {
        std::cerr << "错误：" << e.what() << std::endl;
    }

    return 0;
}


