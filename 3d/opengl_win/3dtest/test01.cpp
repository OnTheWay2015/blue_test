#include "stdafx.h"
#include "windows.h"
#include <iostream>
#include <string>
#include <vector>

//#define STB_IMAGE_IMPLEMENTATION
#include "../../3rd/assimp/contrib/stb/stb_image.h"
#include "../../3rd/glew-2.2.0/include/GL/glew.h"
#include "../../3rd/glfw/include/GLFW/glfw3.h"
#include "../../3rd/glm/glm.hpp"
#include "../../3rd/glm/gtc/matrix_transform.hpp"
#include "../../3rd/glm/gtc/type_ptr.hpp"

using namespace TEST_3D;

namespace TEST_BASE_IMG_RENDER_CORRECT_3D_TO_2D_FIXED
{
    // ------------------------------------------------------------------
    // 1. Shader 类: 管理着色器编译与链接
    // ------------------------------------------------------------------
    class Shader
    {
    public:
        GLuint ID;

        Shader(const char* vertexSource, const char* fragmentSource)
        {
            // 编译顶点着色器
            GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader, 1, &vertexSource, NULL);
            glCompileShader(vertexShader);
            checkCompileErrors(vertexShader, "VERTEX");

            // 编译片段着色器
            GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
            glCompileShader(fragmentShader);
            checkCompileErrors(fragmentShader, "FRAGMENT");

            // 链接程序
            ID = glCreateProgram();
            glAttachShader(ID, vertexShader);
            glAttachShader(ID, fragmentShader);
            glLinkProgram(ID);
            checkCompileErrors(ID, "PROGRAM");

            // 删除着色器对象，它们已链接到程序中
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
        }

        ~Shader()
        {
            glDeleteProgram(ID);
        }

        void use()
        {
            glUseProgram(ID);
        }

        void setInt(const std::string& name, int value)
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }

        void setMat4(const std::string& name, const glm::mat4& mat)
        {
            glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
    private:
        void checkCompileErrors(GLuint shader, std::string type)
        {
            GLint success;
            GLchar infoLog[512];
            if (type != "PROGRAM")
            {
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if (!success)
                {
                    glGetShaderInfoLog(shader, 512, NULL, infoLog);
                    std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
            }
            else
            {
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                if (!success)
                {
                    glGetProgramInfoLog(shader, 512, NULL, infoLog);
                    std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
            }
        }
    };

    // ------------------------------------------------------------------
    // 2. Texture 类: 管理纹理加载
    // ------------------------------------------------------------------
    class Texture
    {
    public:
        GLuint ID;
        int width;
        int height;

        Texture(const char* path)
        {
            glGenTextures(1, &ID);
            glBindTexture(GL_TEXTURE_2D, ID);

            // 设置纹理环绕和过滤方式
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // 改为 CLAMP_TO_EDGE 避免边缘出现重复
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // 加载图像
            int nrChannels;
            stbi_set_flip_vertically_on_load(true); //翻转Y
            unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
            if (data)
            {
                GLenum format;
                if (nrChannels == 1)
                    format = GL_RED;
                else if (nrChannels == 3)
                    format = GL_RGB;
                else if (nrChannels == 4)
                    format = GL_RGBA;
                else
                    format = GL_RGB; // 默认

                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            else
            {
                std::cout << "Failed to load texture: " << path << std::endl;
            }
            stbi_image_free(data);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        ~Texture()
        {
            glDeleteTextures(1, &ID);
        }

        void bind(GLuint textureUnit = 0)
        {
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(GL_TEXTURE_2D, ID);
        }
    };

    // ------------------------------------------------------------------
    // 3. Camera 类: 管理视图矩阵和投影矩阵 (修改为适合2D显示的相机)
    // ------------------------------------------------------------------
    class Camera
    {
    public:
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;

        float NearPlant = 1.0f;
        float FarPlant = 20000.0f;
        
        float Yaw;
        float Pitch;

        float MovementSpeed;
        float MouseSensitivity;
        float Zoom; // FOV (Field of View)

        // 构造函数：根据窗口大小和期望的图片大小计算相机参数
        Camera(int windowWidth, int windowHeight, int imgWidth, int imgHeight)
            : WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), MovementSpeed(2.5f), MouseSensitivity(0.1f)
        {
            // 计算相机应距离 Z=0 平面多远，使得 Z=0 平面上的区域正好覆盖窗口大小
            // 使用 FOV 角度和窗口尺寸计算距离
            float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
            float fovRadians = glm::radians(Zoom = 30.0f); // 设定一个基础FOV

            // 计算垂直视野能覆盖的高度对应的距离
            // tan(fov/2) = (windowHeight/2) / distance -> distance = (windowHeight/2) / tan(fov/2)
            // 如果我们想让 Z=0 平面上的 imgHeight 高度正好等于 windowHeight 高度，则
            // distance = (imgHeight / 2) / tan(fov/2)
            //float desiredHeight = static_cast<float>(windowHeight);
            //float calculatedDistance = (desiredHeight / 2.0f) / glm::tan(fovRadians / 2.0f);

            /*

            // 1. 让图片填满窗口的相机距离
            float fullScreenZ = (imgHeight / 2.0f) / tan(fovRadians * 0.5f);

            // 2. 缩放系数：图片高度 / 窗口高度
            float scale = (float)imgHeight / windowHeight;

            // 3. 最终相机位置（后退，让图片缩小到 200x200）
            //float calculatedDistance = fullScreenZ / scale;
            float calculatedDistance = fullScreenZ * scale;


            // 为了简化，我们让相机看 Z=0 平面，使得 Z=0 平面的可见区域与窗口大小匹配
            // 假设图片放置在 Z=0 平面的中心，我们希望相机看到的 Z=0 平面区域至少包含图片
            // 但我们首先让相机看到的区域正好是窗口大小，然后通过模型矩阵缩放图片到合适大小
            // 这样，如果窗口大小为 800x600，我们就让 Z=0 平面上 800x600 的区域可见
            // 相机位置就是 (0, 0, calculatedDistance)，看向 (0, 0, 0)
            Position = glm::vec3(0.0f, 0.0f, calculatedDistance);
            
            */
            float dis = (windowHeight/2) / fovRadians * 0.5f;

            float NearPlant = dis;

            Position = glm::vec3(0.0f, 0.0f, dis);

            // 相机默认朝向负 Z 方向
            Front = glm::vec3(0.0f, 0.0f, -1.0f);
            Up = glm::vec3(0, 1, 0);
            Yaw = -90.0f; // 默认朝向-Z
            Pitch = 0.0f;  // 默认水平朝向
            //updateCameraVectors();
        }

        // 获取视角矩阵
        glm::mat4 GetViewMatrix()
        {
            // 相机看向 Z=0 平面的中心 (0, 0, 0)
            return glm::lookAt(Position, glm::vec3(0.0f, 0.0f, 0.0f), Up);
        }

        // 获取投影矩阵
        glm::mat4 GetProjectionMatrix(float aspectRatio)
        {
			/*
		    规则（OpenGL 标准右手坐标系）
                相机默认看向 -Z 方向
                看到的物体都在 z < 0 的空间里
                但 near 和 far 仍然必须填正数
			*/


            // 使用透视投影, nearPlane 和 farPlane 必须都为正数，而且必须满足 0 < near < far
            return glm::perspective(glm::radians(Zoom), aspectRatio,NearPlant,FarPlant ) ;
            //return glm::perspective(glm::radians(Zoom), aspectRatio,1.0f, 2000.0f);
        }

    private:
        // 更新相机的Front, Right, Up向量
        void updateCameraVectors()
        {
            // 计算前向量
            glm::vec3 front;
            front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            front.y = sin(glm::radians(Pitch));
            front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            Front = glm::normalize(front);
            // 右向量
            Right = glm::normalize(glm::cross(Front, WorldUp));  
            // 上向量
            Up = glm::normalize(glm::cross(Right, Front));
        }
    };

    // ------------------------------------------------------------------
    // 4. Mesh 类: 管理顶点数据、VAO/VBO/EBO 和绘制
    // ------------------------------------------------------------------
    class Mesh
    {
    public:
        GLuint VAO;
        
        // 构造函数：接收顶点数据和索引数据
        // 顶点数据现在代表图片的物理尺寸（像素）
        Mesh(int imgWidth, int imgHeight)
        {
            // 创建顶点数据，使其代表图片的实际像素尺寸
            float hw = static_cast<float>(imgWidth) / 2.0f; // 半宽
            float hh = static_cast<float>(imgHeight) / 2.0f; // 半高
            
            VEC4_POS_UV vertices = {
                -hw,  hh, 0.0f,  0.0f, 1.0f, // 左上
                -hw, -hh, 0.0f,  0.0f, 0.0f, // 左下
                 hw, -hh, 0.0f,  1.0f, 0.0f, // 右下
                 hw,  hh, 0.0f,  1.0f, 1.0f  // 右上
            };

            GLuint indices[] = {
                0, 1, 2,
                3, 0, 2
            };

            glGenVertexArrays(1, &VAO);
            GLuint VBO, EBO;
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);

            // 绑定 VBO 并上传顶点数据
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

            // 绑定 EBO 并上传索引数据
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            // 设置顶点属性指针
            // 位置属性 (location = 0)
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            // 纹理坐标属性 (location = 1)
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            // 解绑 VAO
            glBindVertexArray(0);
            
            this->VBO = VBO;
            this->EBO = EBO;
            this->indexCount = 6; // 总共6个索引 (两个三角形)
        }

        ~Mesh()
        {
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
        }

        void draw()
        {
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

    private:
        GLuint VBO;
        GLuint EBO;
        GLuint indexCount;
    };

    // ------------------------------------------------------------------
    // 5. Renderer / Application 类: 管理窗口、主循环和场景组合
    // ------------------------------------------------------------------
    class Renderer
    {
    public:
        Renderer(int width, int height, const std::string& title)
            : windowWidth(width), windowHeight(height), deltaTime(0.0f), lastFrame(0.0f)
        {
            initGLFW(title);
            initScene(); // 现在在 initGLFW 之后，因为需要知道窗口大小来初始化相机
        }

        ~Renderer()
        {
            cleanup();
            glfwTerminate();
        }

        void run()
        {
            while (!glfwWindowShouldClose(window))
            {
                float currentFrame = static_cast<float>(glfwGetTime());
                deltaTime = currentFrame - lastFrame;
                lastFrame = currentFrame;

                processInput();
                render();
                glfwSwapBuffers(window);
                glfwPollEvents();
            }
        }

    private:
        GLFWwindow* window;
        int windowWidth;
        int windowHeight;
        float deltaTime; // 每帧的时间差
        float lastFrame; // 上一帧的时间

        // 场景资源
        Shader* shader;
        Texture* texture;
        Mesh* mesh;
        Camera* camera;

        void initGLFW(const std::string& title)
        {
            if (!glfwInit())
            {
                std::cout << "Failed to initialize GLFW" << std::endl;
                exit(-1);
            }

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // 保持固定窗口大小便于调试

            window = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), nullptr, nullptr);
            if (window == NULL)
            {
                std::cout << "Failed to create GLFW window" << std::endl;
                glfwTerminate();
                exit(-1);
            }
            glfwMakeContextCurrent(window);

            // 设置回调函数以便访问成员变量
            glfwSetWindowUserPointer(window, this);
            glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

            glewExperimental = GL_TRUE;
            if (glewInit() != GLEW_OK)
            {
                std::cout << "Failed to initialize GLEW" << std::endl;
                exit(-1);
            }

            int fbWidth, fbHeight;
            glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
            glViewport(0, 0, fbWidth, fbHeight);
        }

        // 静态回调函数，转发给成员函数
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
        {
            Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
            renderer->onFramebufferSizeChanged(width, height);
        }

        void onFramebufferSizeChanged(int width, int height)
        {
            windowWidth = width;
            windowHeight = height;
            glViewport(0, 0, width, height);
            // 如果窗口大小改变，可以考虑重新计算相机参数
            // 但此处我们固定窗口大小，所以不处理
        }

        void initScene()
        {
            // 1. 定义着色器源码 (修改：添加uniform matrices)
            const char* vertexShaderSource = R"(
                #version 330 core
                layout (location = 0) in vec3 aPos;
                layout (location = 1) in vec2 aTexCoord;
                out vec2 TexCoord;
                
                uniform mat4 model;
                uniform mat4 view;
                uniform mat4 projection;
                
                void main() {
                    gl_Position = projection * view * model * vec4(aPos, 1.0);
                    TexCoord = aTexCoord;
                }
            )";

            const char* fragmentShaderSource = R"(
                #version 330 core
                out vec4 FragColor;
                in vec2 TexCoord;
                uniform sampler2D ourTexture;
                void main() {
                    FragColor = texture(ourTexture, TexCoord);
                }
            )";

            // 2. 创建 Shader
            shader = new Shader(vertexShaderSource, fragmentShaderSource);

            // 3. 加载 Texture
            texture = new Texture("../../../run/img_test.png");

            // 4. 创建 Mesh (使用图片的实际像素尺寸)
            mesh = new Mesh(texture->width, texture->height);

            // 5. 创建 Camera (根据窗口和图片大小计算相机参数)
            camera = new Camera(windowWidth, windowHeight, texture->width, texture->height);
        }

        void processInput()
        {
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);

            // 简单的键盘移动相机
            float cameraSpeed = 2.5f * deltaTime; // 将速度与deltaTime相乘，使移动平滑且独立于帧率
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                camera->Position += cameraSpeed * camera->Front;
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                camera->Position -= cameraSpeed * camera->Front;
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                camera->Position -= glm::normalize(glm::cross(camera->Front, camera->Up)) * cameraSpeed; // 左移
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                camera->Position += glm::normalize(glm::cross(camera->Front, camera->Up)) * cameraSpeed; // 右移
        }

        void render()
        {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 启用深度缓冲

            glEnable(GL_DEPTH_TEST); // 启用深度测试

            shader->use();

            //float scale = 10.0f / windowHeight;
            //glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(scale));

            //float pixelScale = 2.0f / (float)windowHeight;
            //glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(pixelScale));

            // --- 模型矩阵：对于 2D 显示，通常不需要额外变换 ---
            // 图片已经按照其像素尺寸放置在 Z=0 平面的原点周围
            glm::mat4 model = glm::mat4(1.0f); 
            // (可选) 如果需要进一步的 2D 变换（如旋转、缩放），可以在这里应用

            glm::mat4 view = camera->GetViewMatrix();
            float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
            glm::mat4 projection = camera->GetProjectionMatrix(aspectRatio);

            shader->setMat4("model", model); // Identity matrix
            shader->setMat4("view", view);
            shader->setMat4("projection", projection);

            // 绑定纹理
            texture->bind(0);
            shader->setInt("ourTexture", 0);

            // 绘制网格
            mesh->draw();

            //glDisable(GL_DEPTH_TEST); // 关闭深度测试
        }

        void cleanup()
        {
            delete mesh;
            delete texture;
            delete shader;
            delete camera;
        }
    };

    // 入口测试函数
    void test(void) {
        // 创建渲染器实例并运行
        Renderer app(800, 600, "OpenGL OOP 3D to 2D Corrected Demo Fixed");
        app.run();
    }
}
