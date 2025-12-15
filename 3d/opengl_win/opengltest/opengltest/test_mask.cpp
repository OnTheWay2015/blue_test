//#include <opencv2/opencv.hpp>
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>


#include "../../3rd/glew-2.2.0/include/GL/glew.h"
#include "../../3rd/glfw/include/GLFW/glfw3.h"
#include <iostream>
#include <string>
#include <vector>

// 全局变量
GLuint shaderProgram;       // Shader程序ID
GLuint texOriginal;         // 原图纹理ID
GLuint texMask;             // Mask纹理ID（Alpha全0）
GLuint VAO, VBO;            // 顶点数组/缓冲对象

// Shader源码（顶点+片段）
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    TexCoord = aTexCoord;
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texOriginal;  // 原图纹理
uniform sampler2D texMask;      // Mask纹理（Alpha全0）

void main() {
    // 采样原图和Mask纹理
    vec4 originalColor = texture(texOriginal, TexCoord);
    vec4 maskColor = texture(texMask, TexCoord);
    
    // 核心逻辑：Mask Alpha为0时显示原图，否则透明（Alpha=0）
    // 用0.01避免浮点精度误差（Alpha=0的浮点值可能略大于0）
    if (maskColor.a < 0.01) {
        FragColor = originalColor; // 显示原图
    } else {
        FragColor = vec4(0.0, 0.0, 0.0, 0.0); // 透明
    }
}
)";

// 编译Shader函数
bool compileShader() {
    // 1. 编译顶点Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // 检查编译错误
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "顶点Shader编译失败：\n" << infoLog << std::endl;
        return false;
    }

    // 2. 编译片段Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "片段Shader编译失败：\n" << infoLog << std::endl;
        return false;
    }

    // 3. 链接Shader程序
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Shader链接失败：\n" << infoLog << std::endl;
        return false;
    }

    // 清理临时Shader
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return true;
}

// 加载纹理（支持BGRA转RGBA，适配OpenGL）
GLuint loadTexture(const cv::Mat& img) {
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    // 设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 转换OpenCV的BGRA为OpenGL的RGBA
    cv::Mat imgRGBA;
    if (img.channels() == 4) {
        cv::cvtColor(img, imgRGBA, cv::COLOR_BGRA2RGBA);
    }
    else if (img.channels() == 3) {
        cv::cvtColor(img, imgRGBA, cv::COLOR_BGR2RGBA);
    }
    else { // 灰度图
        cv::cvtColor(img, imgRGBA, cv::COLOR_GRAY2RGBA);
    }

    // 上传纹理数据
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgRGBA.cols, imgRGBA.rows, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, imgRGBA.data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    return texID;
}

// 生成Alpha全0的Mask纹理（复用之前的函数）
cv::Mat createAlphaZeroTexture(int w, int h, const cv::Scalar& bgrColor = cv::Scalar(0, 0, 0)) {
    cv::Mat texture = cv::Mat(h, w, CV_8UC4, cv::Scalar(bgrColor[0], bgrColor[1], bgrColor[2], 255));
    std::vector<cv::Mat> channels;
    cv::split(texture, channels);
    channels[3].setTo(0); // Alpha全0
    cv::merge(channels, texture);
    return texture;
}

// 初始化顶点数据（全屏矩形）
void initVertexData() {
    // 顶点坐标 + 纹理坐标（[-1,-1]到[1,1]覆盖整个窗口）
    float vertices[] = {
        // 位置          // 纹理坐标
         1.0f,  1.0f,     1.0f, 1.0f,
         1.0f, -1.0f,     1.0f, 0.0f,
        -1.0f, -1.0f,     0.0f, 0.0f,
        -1.0f,  1.0f,     0.0f, 1.0f
    };
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    GLuint EBO;
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 位置属性（vec2）
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 纹理坐标属性（vec2）
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// 窗口大小回调
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int test_mask() {
    // 1. 初始化GLFW
    if (!glfwInit()) {
        std::cerr << "GLFW初始化失败！" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 2. 创建窗口
    int windowWidth = 800, windowHeight = 600;
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Mask Alpha=0 Show Image", NULL, NULL);
    if (!window) {
        std::cerr << "窗口创建失败！" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 3. 初始化GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW初始化失败！" << std::endl;
        return -1;
    }

    // 4. 编译Shader
    if (!compileShader()) {
        glfwTerminate();
        return -1;
    }

    // 5. 加载原图
    std::string imgPath = "test.png"; // 替换为你的原图路径（建议PNG，带Alpha）
    cv::Mat imgOriginal = cv::imread(imgPath, cv::IMREAD_UNCHANGED);
    if (imgOriginal.empty()) {
        std::cerr << "读取原图失败：" << imgPath << std::endl;
        glfwTerminate();
        return -1;
    }
    // 缩放原图适配窗口
    cv::resize(imgOriginal, imgOriginal, cv::Size(windowWidth, windowHeight));
    texOriginal = loadTexture(imgOriginal);

    // 6. 生成Alpha全0的Mask纹理（尺寸与原图一致）
    cv::Mat imgMask = createAlphaZeroTexture(windowWidth, windowHeight, cv::Scalar(255, 255, 255));
    texMask = loadTexture(imgMask);

    // 7. 初始化顶点数据
    initVertexData();

    // 8. 开启透明混合（关键：显示Alpha通道）
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 9. 渲染循环
    while (!glfwWindowShouldClose(window)) {
        // 清屏
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 使用Shader程序
        glUseProgram(shaderProgram);

        // 绑定纹理到采样器
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texOriginal);
        glUniform1i(glGetUniformLocation(shaderProgram, "texOriginal"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texMask);
        glUniform1i(glGetUniformLocation(shaderProgram, "texMask"), 1);

        // 绘制矩形
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // 交换缓冲+处理事件
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 清理资源
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &texOriginal);
    glDeleteTextures(1, &texMask);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}