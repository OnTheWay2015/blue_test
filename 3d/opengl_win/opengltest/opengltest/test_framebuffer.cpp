// 必须先定义STB_IMAGE_IMPLEMENTATION才能编译stb_image
#define STB_IMAGE_IMPLEMENTATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <cstring>
#include "../../3rd/stb_image.h"  // 替换为你的stb_image.h路径

// Windows下链接必要的库（Linux/Mac需手动链接）
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")   // 确保glew32.lib在链接路径中
#pragma comment(lib, "glfw3.lib")    // 确保glfw3.lib在链接路径中

// 全局变量
GLuint fbo;                  // 离屏渲染帧缓冲
GLuint blendTexture;         // 存储两张图混合结果的纹理
GLuint texA, texB, texMask;  // 三张输入纹理
GLuint shaderMix;            // 混合两张图的着色器（FBO渲染用）
GLuint shaderMask;           // 混合纹理+遮罩的着色器（屏幕渲染用）
GLuint quadVAO;              // 全屏四边形VAO（渲染纹理用）
int windowWidth = 800;       // 窗口宽度
int windowHeight = 600;      // 窗口高度

// 错误检查宏（简化调试）
#define CHECK_GL_ERROR() \
    do { \
        GLenum err = glGetError(); \
        if (err != GL_NO_ERROR) { \
            std::cout << "OpenGL错误 " << err << " 出现在 " << __FILE__ << ":" << __LINE__ << std::endl; \
        } \
    } while(0)

// 加载纹理（支持JPG/PNG，需stb_image）
GLuint loadTexture(const std::string& path) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    CHECK_GL_ERROR();

    // 设置纹理过滤和环绕模式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    CHECK_GL_ERROR();

    // 加载图片（翻转Y轴，适配OpenGL纹理坐标）
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    
    if (data) {
        GLenum format = GL_RGB;
        if (nrChannels == 4) format = GL_RGBA;
        
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        CHECK_GL_ERROR();
        std::cout << "纹理加载成功：" << path << " (" << width << "x" << height << ")" << std::endl;
    } else {
        std::cerr << "纹理加载失败：" << path << "（请检查文件路径）" << std::endl;
    }
    
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

// 创建全屏四边形VAO（用于渲染纹理到屏幕）
void createQuadVAO() {
    // 全屏四边形顶点数据（NDC坐标，覆盖整个屏幕）
    float quadVertices[] = {
        // 位置          // 纹理坐标
        -1.0f,  1.0f,   0.0f, 1.0f,
        -1.0f, -1.0f,   0.0f, 0.0f,
         1.0f, -1.0f,   1.0f, 0.0f,
        -1.0f,  1.0f,   0.0f, 1.0f,
         1.0f, -1.0f,   1.0f, 0.0f,
         1.0f,  1.0f,   1.0f, 1.0f
    };

    GLuint VBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    
    // 位置属性（layout 0）
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // 纹理坐标属性（layout 1）
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // 解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    CHECK_GL_ERROR();
}

// 创建离屏渲染FBO（存储两张图的混合结果）
bool createFBO() {
    // 1. 创建FBO对象
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    CHECK_GL_ERROR();

    // 2. 创建FBO附属纹理（存储混合结果）
    glGenTextures(1, &blendTexture);
    glBindTexture(GL_TEXTURE_2D, blendTexture);
    // 创建空纹理（尺寸与窗口一致）
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windowWidth, windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    CHECK_GL_ERROR();

    // 3. 将纹理附着到FBO的颜色附件0
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blendTexture, 0);
    CHECK_GL_ERROR();

    // 4. 检查FBO完整性
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "FBO创建失败！错误码：" << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
        return false;
    }

    // 5. 解绑FBO（回到默认帧缓冲）
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

// 编译着色器程序（输入顶点/片段着色器源码，返回程序ID）
GLuint compileShaderProgram(const char* vertSource, const char* fragSource) {
    GLint success;
    char infoLog[512];

    // 编译顶点着色器
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertSource, NULL);
    glCompileShader(vertShader);
    
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
        std::cerr << "顶点着色器编译失败：\n" << infoLog << std::endl;
    }

    // 编译片段着色器
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragSource, NULL);
    glCompileShader(fragShader);
    
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
        std::cerr << "片段着色器编译失败：\n" << infoLog << std::endl;
    }

    // 链接着色器程序
    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);
    
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        std::cerr << "着色器程序链接失败：\n" << infoLog << std::endl;
    }

    // 删除临时着色器对象（链接后不再需要）
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    CHECK_GL_ERROR();

    return program;
}

// 初始化所有着色器
void initShaders() {
    // ========== 1. 混合着色器（用于FBO离屏渲染两张图） ==========
    const char* mixVertSource = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        layout (location = 1) in vec2 aTexCoord;
        out vec2 TexCoord;
        void main() {
            gl_Position = vec4(aPos, 0.0, 1.0);
            TexCoord = aTexCoord;
        }
    )";

    const char* mixFragSource = R"(
        #version 330 core
        in vec2 TexCoord;
        out vec4 FragColor;
        uniform sampler2D tex;
        void main() {
            FragColor = texture(tex, TexCoord);
        }
    )";
    shaderMix = compileShaderProgram(mixVertSource, mixFragSource);

    // ========== 2. 遮罩着色器（用于屏幕渲染混合纹理+遮罩） ==========
    const char* maskVertSource = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        layout (location = 1) in vec2 aTexCoord;
        out vec2 TexCoord;
        void main() {
            gl_Position = vec4(aPos, 0.0, 1.0);
            TexCoord = aTexCoord;
        }
    )";

    const char* maskFragSource = R"(
        #version 330 core
        in vec2 TexCoord;
        out vec4 FragColor;
        uniform sampler2D blendTex;  // 混合后的纹理
        uniform sampler2D maskTex;   // 遮罩纹理
        void main() {
            // 采样混合纹理和遮罩纹理
            vec4 blendColor = texture(blendTex, TexCoord);
            vec4 maskColor = texture(maskTex, TexCoord);
            
            // 遮罩逻辑：灰度值<0.1则丢弃（黑色区域隐藏）
            float maskValue = maskColor.r;
            if (maskValue < 0.1) {
                discard;
            }
            
            // 可选：用遮罩的Alpha通道控制透明度
            // blendColor.a *= maskColor.a;
            
            FragColor = blendColor;
        }
    )";
    shaderMask = compileShaderProgram(maskVertSource, maskFragSource);
}

// 初始化所有资源（纹理、FBO、着色器、VAO）
bool initResources() {
    // 1. 创建全屏四边形VAO
    createQuadVAO();

    // 2. 创建FBO
    if (!createFBO()) {
        return false;
    }

    // 3. 加载纹理（替换为你的图片路径！！！）
    texA = loadTexture("textureA.png");   // 第一张图（如红色背景）
    texB = loadTexture("textureB.png");   // 第二张图（如蓝色半透明图标）
    texMask = loadTexture("mask.png");    // 遮罩图（灰度图，白色显示/黑色隐藏）

    // 4. 编译着色器
    initShaders();

    // 5. 启用混合模式（透明叠加）
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // 标准透明混合
    CHECK_GL_ERROR();

    return true;
}

// 核心渲染逻辑：先混合两张图到FBO，再用遮罩渲染到屏幕
void render() {
    // ========== 第一步：离屏渲染 - 混合两张图到FBO ==========
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);  // 绑定FBO（渲染到纹理）
    glViewport(0, 0, windowWidth, windowHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);    // 黑色背景
    glClear(GL_COLOR_BUFFER_BIT);
    CHECK_GL_ERROR();

    // 渲染第一张图（texA）
    glUseProgram(shaderMix);
    glUniform1i(glGetUniformLocation(shaderMix, "tex"), 0);  // 绑定到纹理单元0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texA);
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    CHECK_GL_ERROR();

    // 渲染第二张图（texB）- 开启混合后会叠加到第一张图上
    glUniform1i(glGetUniformLocation(shaderMix, "tex"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texB);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    CHECK_GL_ERROR();

    // ========== 第二步：屏幕渲染 - 混合纹理 + 遮罩 ==========
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  // 解绑FBO（回到屏幕帧缓冲）
    glViewport(0, 0, windowWidth, windowHeight);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // 浅灰背景
    glClear(GL_COLOR_BUFFER_BIT);
    CHECK_GL_ERROR();

    // 使用遮罩着色器
    glUseProgram(shaderMask);
    // 绑定混合纹理到单元0
    glUniform1i(glGetUniformLocation(shaderMask, "blendTex"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, blendTexture);
    // 绑定遮罩纹理到单元1
    glUniform1i(glGetUniformLocation(shaderMask, "maskTex"), 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texMask);
    CHECK_GL_ERROR();

    // 渲染全屏四边形（应用遮罩）
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    CHECK_GL_ERROR();

    // 解绑资源
    glBindVertexArray(0);
    glUseProgram(0);
}

// 释放所有OpenGL资源
void cleanup() {
    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &blendTexture);
    glDeleteTextures(1, &texA);
    glDeleteTextures(1, &texB);
    glDeleteTextures(1, &texMask);
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteProgram(shaderMix);
    glDeleteProgram(shaderMask);
}

int main() {
    // ========== 1. 初始化GLFW ==========
    if (!glfwInit()) {
        std::cerr << "GLFW初始化失败！" << std::endl;
        return -1;
    }

    // 配置GLFW（OpenGL 3.3 Core Profile）
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);  // 固定窗口尺寸

    // ========== 2. 创建窗口 ==========
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "OpenGL Blend + Mask", nullptr, nullptr);
    if (!window) {
        std::cerr << "GLFW窗口创建失败！" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // ========== 3. 初始化GLEW ==========
    glewExperimental = GL_TRUE;  // 必须开启，否则Core Profile可能初始化失败
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW初始化失败！" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // ========== 4. 初始化资源 ==========
    if (!initResources()) {
        std::cerr << "资源初始化失败！" << std::endl;
        cleanup();
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // ========== 5. 主循环 ==========
    while (!glfwWindowShouldClose(window)) {
        // 处理输入事件
        glfwPollEvents();

        // 渲染一帧
        render();

        // 交换双缓冲
        glfwSwapBuffers(window);
    }

    // ========== 6. 清理资源 ==========
    cleanup();
    glfwDestroyWindow(window);
    glfwTerminate();

    std::cout << "程序正常退出" << std::endl;
    return 0;
}