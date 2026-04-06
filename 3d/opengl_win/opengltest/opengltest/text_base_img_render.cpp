/*
_faq:



*/



#include "stdafx.h"
#include "windows.h"
#include <iostream>

//#define STB_IMAGE_IMPLEMENTATION
#include "../../3rd/assimp/contrib/stb/stb_image.h"

#include "../../3rd/glew-2.2.0/include/GL/glew.h"
#include "../../3rd/glfw/include/GLFW/glfw3.h"


#include "../../3rd/glm/glm.hpp"
#include "../../3rd/glm/gtc/matrix_transform.hpp" // 包含各种变换矩阵生成函数
#include "../../3rd/glm/gtc/type_ptr.hpp"    



namespace TEST_BASE_IMG_RENDER
{
    using namespace TEST_3D ;
    
    int g_window_w = 800;
    int g_window_h = 600;

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
	GLuint loadTexture(const char* texturePath, int& width, int& heigh) {
		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// 设置纹理环绕/过滤方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// 加载纹理图片（使用SOIL库）
		int nrChannels;
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
        //float vertices[] = {
        //    // 位置              // 纹理坐标  纹理坐标以左上角为基点
        //    -1.0f, 1.0f, 0.0f,  0.0f, 0.0f, // 左上
        //    -1.0f, -1.0f, 0.0f,  0.0f, 1.0f, // 左下
        //     1.0f, -1.0f, 0.0f,  1.0f, 1.0f, // 右下
        //     1.0f, 1.0f, 0.0f,  1.0f, 0.0f, // 右上
        //};

        VEC4_POS_UV vertices= {
            // 位置 opengl ndc 取值为 [-1,1],窗口中心为(0,0)  // 纹理坐标 取值[0,1]  纹理坐标以左上角为基点(0,0)
             - 1.0f, 1.0f, 0.0f,  0.0f, 0.0f, // 左上
            -1.0f, -1.0f, 0.0f,  0.0f, 1.0f, // 左下
             1.0f, -1.0f, 0.0f,  1.0f, 1.0f, // 右下
             1.0f, 1.0f, 0.0f,  1.0f, 0.0f // 右上
        };

        //vertices.vec4[0].v.pos.x = 100;

        // 正确的四边形索引（两个按顶点逆时针,画的三角形）
        GLuint indices[] = {
            0, 1, 2, // 第一个三角形：左上→左下→右下
            3, 0,2  // 第二个三角形：右上→左上→右下
        };

        int width=0,heigh = 0;
        //加载纹理
        g_textureID = loadTexture("../../../run/img_test.png",width,heigh); 


        glm::vec3 translateVec(0.0f, 0.0f, 0.0f);    // 平移向量
        glm::vec3 scaleVec(width/2.0f, heigh/2.0f, 1.0f); // 缩放向量

        float rotateAngle = glm::radians(0.0f);     // 旋转角度（绕 Z 轴）
        glm::vec3 rotateAxis(0.0f, 0.0f, 1.0f);      // 旋转轴（Z 轴）

        //组合变换矩阵（核心：先缩放 → 再旋转 → 最后平移，代码中反向相乘）
        glm::mat4 modelMat = glm::mat4(1.0f);        // 初始化为单位矩阵
        modelMat = glm::translate(modelMat, translateVec);  // 平移
        modelMat = glm::rotate(modelMat, rotateAngle, rotateAxis);  // 旋转
        modelMat = glm::scale(modelMat ,scaleVec);

        //正交投影
        {
            float left = -g_window_w/2.0f;     // 左裁剪面
            float right = g_window_w/2.0f;     // 右裁剪面
            float top = g_window_h/2.0f;       // 上裁剪面
            float bottom =-g_window_h/2.0f;   // 下裁剪面
            float nearPlane = -1.0f; // 近裁剪面（必须 >0，且 < far）
            float farPlane =  1.0f;// 远裁剪面

            // 创建正交投影矩阵
            glm::mat4 orthoMat = glm::ortho(left, right, bottom, top, nearPlane, farPlane);
            modelMat= orthoMat * modelMat; // 投影矩阵 * 视图矩阵 * 模型矩阵  /2D渲染无视图矩阵

        }


        for (int i=0;i<4;i++)
		{
			auto& d = vertices.vec4[i];
            glm::vec3 originalPos(
                d.v.pos.x,
                d.v.pos.y,
                d.v.pos.z);

			//将三维点转换为齐次坐标（vec4，w=1）
			glm::vec4 posHomogeneous(originalPos, 1.0f);

			//矩阵与齐次坐标相乘（GLM重载了*运算符，顺序：矩阵 * 坐标）
			glm::vec4 transformedHomogeneous = modelMat * posHomogeneous;
            

        

			//将齐次坐标转回三维点（取出x,y,z即可，w通常仍为1）
			glm::vec3 transformedPos = glm::vec3(transformedHomogeneous);
            d.v.pos.x = transformedPos.x;
            d.v.pos.y = transformedPos.y;
            d.v.pos.z = transformedPos.z;
		}

        // 可选：将 glm::mat4 转换为 float 数组（用于传给 GPU 着色器）
        //const float* matArray = glm::value_ptr(modelMat);


        // 5. 配置VAO/VBO/EBO
        glGenVertexArrays(1, g_VAOs);
        glGenBuffers(1, g_VBOs);
        glGenBuffers(1, g_EBOs);

        glBindVertexArray(g_VAOs[0]);

        glBindBuffer(GL_ARRAY_BUFFER, g_VBOs[0]);
        auto len1 = sizeof(VEC4_POS_UV);
        auto len2 = sizeof(uni_POS_UV);
        auto len2_1 = sizeof(uni_POS_UV::POS_UV);
        auto len3 = sizeof(POS);
        auto len4 = sizeof(UV);
        auto len = sizeof(vertices);
        glBufferData(GL_ARRAY_BUFFER, len, vertices.vec4[0].ves, GL_STATIC_DRAW);
        //glBufferData(GL_ARRAY_BUFFER, len, vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_EBOs[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // 配置位置属性（location 0）
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // 配置纹理坐标属性（location 1）
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
	

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
        GLFWwindow* window = glfwCreateWindow(g_window_w, g_window_h, "OpenGL Texture Demo", nullptr, nullptr);
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