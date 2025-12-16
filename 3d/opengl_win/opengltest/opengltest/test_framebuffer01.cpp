/*
代码说明
1. FBO 封装（Framebuffer 结构体）
	create()：创建 FBO + 颜色纹理 + 深度缓冲，自动检查完整性；
	bind()：绑定 FBO 并设置视口（匹配 FBO 尺寸）；
	unbind()：解绑 FBO，回到屏幕默认帧缓冲；
	destroy()：释放 FBO 关联的所有资源（避免内存泄漏）。
2. 多 FBO 渲染流程
	步骤	目标 FBO	操作	输出
	1	FBO1	绘制橙色三角形	FBO1.colorTexture（存储三角形纹理）
	2	FBO2	对 FBO1 纹理做反色	FBO2.colorTexture（存储反色后的纹理）
	3	屏幕	渲染 FBO2 纹理	屏幕显示反色后的三角形
3. 关键注意事项
	每个 FBO 独立资源：颜色纹理、深度缓冲不能共用，否则会导致渲染结果混乱；
	视口匹配：绑定 FBO 后必须调用 glViewport 设置为 FBO 尺寸，否则纹理会拉伸 / 错位；
	深度缓冲：如果是 3D 渲染，FBO 必须绑定深度缓冲（RBO / 纹理），否则深度测试失效；
	资源销毁：每个 FBO 的纹理、RBO 都要单独销毁，避免内存泄漏。
	扩展场景（多 FBO 典型用法）
	后处理流水线：FBO1（基础渲染）→ FBO2（高斯模糊）→ FBO3（高光强化）→ 屏幕；
	多通道渲染：FBO1 存储颜色、FBO2 存储法线、FBO3 存储深度，最后合并为最终画面；
	纹理复用：将 FBO 纹理作为材质贴到 3D 模型上（如镜面反射、动态纹理）。


*/









#define STB_IMAGE_IMPLEMENTATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>

// Windows 链接库（Linux/Mac 需手动调整链接参数）
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")

// 错误检查宏
#define CHECK_GL_ERROR() \
    do { \
        GLenum err = glGetError(); \
        if (err != GL_NO_ERROR) { \
            std::cerr << "OpenGL Error: " << err << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        } \
    } while(0)

// ========== 1. 封装FBO结构体（方便管理多个FBO） ==========
struct Framebuffer {
    GLuint fbo = 0;          // FBO ID
    GLuint colorTexture = 0; // 颜色附件纹理
    GLuint depthRBO = 0;     // 深度缓冲（渲染缓冲对象）
    int width = 0;
    int height = 0;

    // 创建FBO（带颜色纹理+深度缓冲）
    bool create(int w, int h) {
        width = w;
        height = h;

        // 1. 创建FBO对象
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        CHECK_GL_ERROR();

        // 2. 创建颜色附件纹理
        glGenTextures(1, &colorTexture);
        glBindTexture(GL_TEXTURE_2D, colorTexture);
        // 创建空纹理（存储RGB颜色）
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // 将纹理附着到FBO颜色附件0
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);
        CHECK_GL_ERROR();

        // 3. 创建深度缓冲（RBO，2D渲染可选，但3D必须）
        glGenRenderbuffers(1, &depthRBO);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
        // 将深度缓冲附着到FBO
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO);
        CHECK_GL_ERROR();

        // 4. 检查FBO完整性
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "FBO创建失败！尺寸：" << w << "x" << h << std::endl;
            return false;
        }

        // 5. 解绑
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return true;
    }

    // 绑定FBO（渲染到该FBO）
    void bind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glViewport(0, 0, width, height); // 匹配FBO尺寸
        CHECK_GL_ERROR();
    }

    // 解绑FBO（回到屏幕帧缓冲）
    static void unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    // 销毁FBO资源
    void destroy() {
        if (depthRBO) glDeleteRenderbuffers(1, &depthRBO);
        if (colorTexture) glDeleteTextures(1, &colorTexture);
        if (fbo) glDeleteFramebuffers(1, &fbo);
        fbo = colorTexture = depthRBO = 0;
    }
};

// ========== 2. 全局资源 ==========
Framebuffer fbo1, fbo2;       // 两个帧缓冲
GLuint quadVAO = 0;           // 全屏四边形（渲染纹理用）
GLuint shaderBasic;           // 基础渲染着色器（渲染到FBO1）
GLuint shaderInvert;          // 反色特效着色器（渲染到FBO2）
GLuint shaderScreen;          // 屏幕渲染着色器（渲染FBO2到屏幕）
int windowWidth = 800;
int windowHeight = 600;

// ========== 3. 基础工具函数 ==========
// 创建全屏四边形VAO（覆盖整个屏幕）
void createQuadVAO() {
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
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    CHECK_GL_ERROR();
}

// 编译着色器程序
GLuint compileShader(const char* vertSrc, const char* fragSrc) {
    GLint success;
    char infoLog[512];

    // 顶点着色器
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertSrc, NULL);
    glCompileShader(vertShader);
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
        std::cerr << "顶点着色器编译失败：\n" << infoLog << std::endl;
    }

    // 片段着色器
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragSrc, NULL);
    glCompileShader(fragShader);
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
        std::cerr << "片段着色器编译失败：\n" << infoLog << std::endl;
    }

    // 链接程序
    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "着色器链接失败：\n" << infoLog << std::endl;
    }

    // 清理临时对象
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    CHECK_GL_ERROR();
    return program;
}

// 初始化所有着色器
void initShaders() {
    // ========== 着色器1：基础渲染（绘制彩色三角形到FBO1） ==========
    const char* basicVert = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        void main() {
            gl_Position = vec4(aPos, 0.0, 1.0);
        }
    )";
    const char* basicFrag = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(1.0, 0.5, 0.2, 1.0); // 橙色
        }
    )";
    shaderBasic = compileShader(basicVert, basicFrag);

    // ========== 着色器2：反色特效（将FBO1纹理反色后渲染到FBO2） ==========
    const char* invertVert = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        layout (location = 1) in vec2 aTexCoord;
        out vec2 TexCoord;
        void main() {
            gl_Position = vec4(aPos, 0.0, 1.0);
            TexCoord = aTexCoord;
        }
    )";
    const char* invertFrag = R"(
        #version 330 core
        in vec2 TexCoord;
        out vec4 FragColor;
        uniform sampler2D tex;
        void main() {
            vec3 color = texture(tex, TexCoord).rgb;
            FragColor = vec4(1.0 - color, 1.0); // 反色
        }
    )";
    shaderInvert = compileShader(invertVert, invertFrag);

    // ========== 着色器3：屏幕渲染（将FBO2纹理渲染到屏幕） ==========
    const char* screenVert = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        layout (location = 1) in vec2 aTexCoord;
        out vec2 TexCoord;
        void main() {
            gl_Position = vec4(aPos, 0.0, 1.0);
            TexCoord = aTexCoord;
        }
    )";
    const char* screenFrag = R"(
        #version 330 core
        in vec2 TexCoord;
        out vec4 FragColor;
        uniform sampler2D tex;
        void main() {
            FragColor = texture(tex, TexCoord);
        }
    )";
    shaderScreen = compileShader(screenVert, screenFrag);
}

// ========== 4. 初始化资源（创建多个FBO） ==========
bool initResources() {
    // 1. 创建全屏四边形
    createQuadVAO();

    // 2. 创建两个FBO（尺寸与窗口一致）
    if (!fbo1.create(windowWidth, windowHeight)) return false;
    if (!fbo2.create(windowWidth, windowHeight)) return false;
    std::cout << "FBO1、FBO2 创建成功！" << std::endl;

    // 3. 初始化着色器
    initShaders();

    // 4. 启用深度测试（可选，2D渲染可关闭）
    glEnable(GL_DEPTH_TEST);
    CHECK_GL_ERROR();
    return true;
}

// ========== 5. 核心渲染逻辑（多FBO分步渲染） ==========
void render() {
    // ========== 步骤1：渲染到FBO1（绘制基础三角形） ==========
    fbo1.bind(); // 绑定FBO1，渲染结果存入fbo1.colorTexture
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // 背景色
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 绘制彩色三角形
    glUseProgram(shaderBasic);
    // 三角形顶点数据（NDC坐标）
    float triVertices[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.0f,  0.5f
    };
    GLuint triVBO;
    glGenBuffers(1, &triVBO);
    glBindBuffer(GL_ARRAY_BUFFER, triVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triVertices), triVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDeleteBuffers(1, &triVBO); // 临时VBO，用完销毁
    CHECK_GL_ERROR();

    // ========== 步骤2：渲染到FBO2（对FBO1的纹理做反色） ==========
    fbo2.bind(); // 绑定FBO2，渲染结果存入fbo2.colorTexture
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 使用反色着色器，采样FBO1的颜色纹理
    glUseProgram(shaderInvert);
    glUniform1i(glGetUniformLocation(shaderInvert, "tex"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo1.colorTexture);
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6); // 渲染全屏四边形
    CHECK_GL_ERROR();

    // ========== 步骤3：渲染FBO2到屏幕 ==========
    Framebuffer::unbind(); // 解绑FBO，回到屏幕帧缓冲
    glViewport(0, 0, windowWidth, windowHeight);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 使用屏幕着色器，采样FBO2的颜色纹理
    glUseProgram(shaderScreen);
    glUniform1i(glGetUniformLocation(shaderScreen, "tex"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo2.colorTexture);
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    CHECK_GL_ERROR();

    // 解绑资源
    glBindVertexArray(0);
    glUseProgram(0);
}

// ========== 6. 清理资源 ==========
void cleanup() {
    fbo1.destroy();
    fbo2.destroy();
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteProgram(shaderBasic);
    glDeleteProgram(shaderInvert);
    glDeleteProgram(shaderScreen);
}

// ========== 7. 主函数 ==========
int main() {
    // 初始化GLFW
    if (!glfwInit()) {
        std::cerr << "GLFW初始化失败！" << std::endl;
        return -1;
    }

    // 配置OpenGL上下文（3.3 Core Profile）
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Multiple FBO Example", nullptr, nullptr);
    if (!window) {
        std::cerr << "窗口创建失败！" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // 初始化GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW初始化失败！" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // 初始化资源（创建多个FBO）
    if (!initResources()) {
        std::cerr << "资源初始化失败！" << std::endl;
        cleanup();
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // 主循环
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents(); // 处理输入
        render();         // 多FBO渲染
        glfwSwapBuffers(window); // 交换缓冲
    }

    // 清理资源
    cleanup();
    glfwDestroyWindow(window);
    glfwTerminate();
    std::cout << "程序正常退出" << std::endl;
    return 0;
}