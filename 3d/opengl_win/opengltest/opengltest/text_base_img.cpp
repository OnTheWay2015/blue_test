/*
_faq:
    当加载纹理数据和 设置的 internalFormat 不一样时，可能是灰白，点点,错乱图样 
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, heigh, 0, internalFormat, GL_UNSIGNED_BYTE, image);



*/



#include "windows.h"
#include <iostream>

//#define STB_IMAGE_IMPLEMENTATION
#include "../../3rd/assimp/contrib/stb/stb_image.h"

#include "../../3rd/glew-2.2.0/include/GL/glew.h"
#include "../../3rd/glfw/include/GLFW/glfw3.h"

namespace TEST_BASE_IMG
{
    GLuint g_shaderProgram;
    GLuint g_VAOs[1], g_VBOs[1], g_EBOs[1];
    GLuint g_textureID; // 存储纹理ID

    // 检查着色器编译/链接错误的工具函数
    void checkShaderError(GLuint shader, GLenum type, const char* name) {
        GLint success;
        GLchar infoLog[512];
        if (type == GL_COMPILE_STATUS) {
            glGetShaderiv(shader, type, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::" << name << "::COMPILATION_FAILED\n" << infoLog << std::endl;
            }
        }
        else if (type == GL_LINK_STATUS) {
            glGetProgramiv(shader, type, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
            }
        }
    }

    // 加载纹理的函数
	GLuint loadTexture(const char* texturePath) {
		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// 设置纹理环绕/过滤方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// 加载纹理图片（使用SOIL库）
		int width, heigh, nrChannels;
		//unsigned char* image = SOIL_load_image(texturePath, &width, &height, 0, SOIL_LOAD_RGB);
		unsigned char* image = stbi_load(texturePath, &width, &heigh, &nrChannels, 0);
		if (image == nullptr) {
			std::cout << "ERROR::TEXTURE::LOAD_FAILED: " << texturePath << std::endl;
			stbi_image_free(image);
			return 0;
		}


		GLenum internalFormat, dataFormat;
		if (nrChannels == 1) {
			internalFormat = GL_RED;
			dataFormat = GL_RED;
		}
		else if (nrChannels == 3) {
			internalFormat = GL_RGB;
			dataFormat = GL_RGB;
		}
		else if (nrChannels == 4) { // 带透明的PNG会是4通道
			internalFormat = GL_RGBA;
			dataFormat = GL_RGBA;
		}

        /*
        internalFormat：告诉 GPU 纹理在显存中如何存储（相当于 “仓库的存储格式”）
        dataFormat：告诉 GPU 你传入的 CPU 数据是什么格式（相当于 “待入库货物的包装格式”）
        OpenGL 在调用 glTexImage2D 时，会自动把 dataFormat 格式的 CPU 数据转换成 internalFormat 格式存储到显存中。 
        */
		// 将纹理数据传入GPU
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, heigh, 0, dataFormat, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D); // 生成多级渐远纹理

		// 释放图片内存
		stbi_image_free(image);
		glBindTexture(GL_TEXTURE_2D, 0); // 解绑纹理

		return textureID;
	}

    void drawInit() {
        // 1. 顶点着色器（新增纹理坐标输入）
        const char* vertexShaderSource = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec2 aTexCoord; // 纹理坐标属性

            out vec2 TexCoord; // 传递给片段着色器的纹理坐标

            void main() {
                gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
                TexCoord = aTexCoord; // 传递纹理坐标
            }
        )";

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        checkShaderError(vertexShader, GL_COMPILE_STATUS, "VERTEX");

        // 2. 片段着色器（新增纹理采样）
        const char* fragmentShaderSource = R"(
            #version 330 core
            out vec4 FragColor;

            in vec2 TexCoord; // 接收顶点着色器的纹理坐标

            uniform sampler2D ourTexture; // 纹理采样器

            void main() {
                FragColor = texture(ourTexture, TexCoord); // 采样纹理颜色
            }
        )";

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        checkShaderError(fragmentShader, GL_COMPILE_STATUS, "FRAGMENT");

        // 3. 链接着色器程序
        g_shaderProgram = glCreateProgram();
        glAttachShader(g_shaderProgram, vertexShader);
        glAttachShader(g_shaderProgram, fragmentShader);
        glLinkProgram(g_shaderProgram);
        checkShaderError(g_shaderProgram, GL_LINK_STATUS, "PROGRAM");

        // 删除临时着色器对象
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // 4. 定义带纹理坐标的顶点数据
        // 格式：x,y,z (位置) | u,v (纹理坐标)
        float vertices[] = {
            // 位置              // 纹理坐标  纹理坐标以左上角为基点
            -0.5f, 0.5f, 0.0f,  0.0f, 0.0f, // 左上
            -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, // 左下
             0.5f, -0.5f, 0.0f,  1.0f, 1.0f, // 右下
             0.5f, 0.5f, 0.0f,  1.0f, 0.0f, // 右上
        };

        //float vertices[] = {
        //    // 位置              // 纹理坐标
        //    -0.5f, 0.5f, 0.0f,  0.0f, 1.0f, // 左上 (修正UV：0,1)
        //    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // 左下 (修正UV：0,0)
        //     0.5f, 0.5f, 0.0f,  1.0f, 1.0f, // 右上 (修正UV：1,1)
        //     0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // 右下 (修正UV：1,0)
        //};


        //GLuint indices[] = { 0, 1, 2 }; // 三角形索引
        //GLuint indices[] = { 0, 1, 2, 3,2,1 }; // 四边形索引

        // 正确的四边形索引（两个按顶点逆时针,画的三角形）
        GLuint indices[] = {
            0, 1, 2, // 第一个三角形：左上→左下→右下
            3, 0,2  // 第二个三角形：右上→左上→右下
        };

        //float vertices[] = {
        //    // positions          // texture coords
        //     0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
        //     0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
        //    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
        //    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
        //};
        //unsigned int indices[] = {
        //    0, 1, 3, // first triangle
        //    1, 2, 3  // second triangle
        //};


        // 5. 配置VAO/VBO/EBO
        glGenVertexArrays(1, g_VAOs);
        glGenBuffers(1, g_VBOs);
        glGenBuffers(1, g_EBOs);

        glBindVertexArray(g_VAOs[0]);

        glBindBuffer(GL_ARRAY_BUFFER, g_VBOs[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_EBOs[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // 配置位置属性（location 0）
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // 配置纹理坐标属性（location 1）
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // 6. 加载纹理（替换为你的纹理图片路径）
        g_textureID = loadTexture("../../../run/img_test.png"); // 请修改为实际图片路径

        // 解绑VAO/VBO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    int glfwtest() {
        // 初始化GLFW
        if (!glfwInit()) {
            std::cout << "Failed to initialize GLFW" << std::endl;
            return -1;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        // 创建窗口
        GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Texture Demo", nullptr, nullptr);
        if (window == NULL) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);

        // 初始化GLEW
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            std::cout << "Failed to initialize GLEW" << std::endl;
            return -1;
        }

        // 设置视口
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        // 初始化绘制资源
        drawInit();

        // 渲染循环
        while (!glfwWindowShouldClose(window)) {
            // 清屏
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // 使用着色器程序
            glUseProgram(g_shaderProgram);

            // 绑定纹理（激活纹理单元0，并绑定纹理）
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, g_textureID);
            // 将纹理单元0赋值给采样器（也可以省略，默认绑定0单元）
            glUniform1i(glGetUniformLocation(g_shaderProgram, "ourTexture"), 0);

            // 绘制带纹理的三角形
            //glBindBuffer(GL_ARRAY_BUFFER, g_VBOs[0]);
            //glBindVertexArray(g_VAOs[0]);
            //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

            
            //glBindVertexArray(g_EBOs[0]);
            
            glBindVertexArray(g_VAOs[0]);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


            // 解绑VAO
            glBindVertexArray(0);

            // 交换缓冲区+处理事件
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        // 释放资源
        glDeleteBuffers(1, g_VBOs);
        glDeleteBuffers(1, g_EBOs);
        glDeleteVertexArrays(1, g_VAOs);
        glDeleteTextures(1, &g_textureID);
        glDeleteProgram(g_shaderProgram);

        glfwTerminate();
        return 0;
    }

    void test(void) {
        glfwtest();
    }
}