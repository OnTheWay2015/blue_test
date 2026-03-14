#include "windows.h"
#include <iostream>

//#define STB_IMAGE_IMPLEMENTATION
#include "../../3rd/assimp/contrib/stb/stb_image.h"

#include "../../3rd/glew-2.2.0/include/GL/glew.h"
#include "../../3rd/glfw/include/GLFW/glfw3.h"


namespace TEST_BASE_IMG1
{
    // settings
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;
    
    GLuint g_shaderProgram;
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
    void createShader() {
        // 1. 顶点着色器（新增纹理坐标输入）
        const char* vertexShaderSource = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec3 aColor;
            layout (location = 2) in vec2 aTexCoord; // 纹理坐标属性

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
            //uniform sampler2D texture1; // 纹理采样器

            void main() {
                FragColor = texture(ourTexture, TexCoord); // 采样纹理颜色
                //FragColor = texture(ourTexture, vec2(0.3,0.2) ); // 采样纹理颜色
                //FragColor = vec4(0.1,1,0,1); //rgba 固定值
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

    }


    void test()
    {
        // glfw: initialize and configure
        // ------------------------------
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        // glfw window creation
        // --------------------
        GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return ;
        }
        glfwMakeContextCurrent(window);
        
        if (glewInit() != GLEW_OK) {
            std::cout << "Failed to initialize GLEW" << std::endl;
            return ;
        }

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float vertices[] = {
            // positions          // colors           // texture coords
             0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
             0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
        };
        unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };
        unsigned int VBO, VAO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);


        // load and create a texture 
        // -------------------------
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
        //unsigned char* data = stbi_load(FileSystem::getPath("resources/textures/container.jpg").c_str(), &width, &height, &nrChannels, 0);
		unsigned char* data = stbi_load("../../../run/img_test.png", &width, &height, &nrChannels, 0);
        if (data)
        {
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

            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, internalFormat, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        createShader();
        // render loop
        // -----------
        while (!glfwWindowShouldClose(window))
        {
            // render
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);



            // 绑定纹理（激活纹理单元0，并绑定纹理）
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            // 将纹理单元0赋值给采样器（也可以省略，默认绑定0单元）
            glUniform1i(glGetUniformLocation(g_shaderProgram, "ourTexture"), 0);
            // render container
            //ourShader.use();
            
            glUseProgram(g_shaderProgram);
            
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            glfwSwapBuffers(window);
        }

        // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);

        // glfw: terminate, clearing all previously allocated GLFW resources.
        // ------------------------------------------------------------------
        glfwTerminate();
    }




}
