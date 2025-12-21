#include "../../3rd/glew-2.2.0/include/GL/glew.h"
#include "../../3rd/glfw/include/GLFW/glfw3.h"
#include <iostream>
#include <vector>
#include <cmath>

// 错误回调函数
void error_callback(int error, const char* description) {
	std::cerr << "GLFW Error: " << description << std::endl;
}

// 编译包含几何着色器的着色器程序
unsigned int compileShaderProgram() {
	// 1. 顶点着色器（仅传递顶点坐标）
	const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        out vec3 vPos; // 传递到几何着色器
        void main() {
            vPos = aPos;
            gl_Position = vec4(aPos, 1.0);
        }
    )";

	// 2. 几何着色器（核心：生成宽线+圆角转折）
	const char* geometryShaderSource = R"(
        #version 330 core
        layout (lines_adjacency) in; // 输入：带邻接的线（每4个顶点表示一段线，含前后邻接顶点）
        layout (triangle_strip, max_vertices = 24) out; // 输出：三角形带，最大顶点数24

        uniform float lineWidth; // 线宽（替代glLineWidth）
        uniform vec2 viewportSize; // 视口大小（用于NDC转屏幕坐标）
        out vec4 fragColor;

        // NDC坐标转屏幕像素坐标
        vec2 ndcToScreen(vec2 ndc) {
            return vec2((ndc.x + 1.0) * 0.5 * viewportSize.x, (ndc.y + 1.0) * 0.5 * viewportSize.y);
        }

        // 屏幕像素坐标转NDC坐标
        vec2 screenToNdc(vec2 screen) {
            return vec2(screen.x / viewportSize.x * 2.0 - 1.0, screen.y / viewportSize.y * 2.0 - 1.0);
        }

        // 计算垂直于方向向量的法向量（单位化）
        vec2 getNormal(vec2 dir) {
            vec2 normal = vec2(-dir.y, dir.x);
            return normalize(normal) * lineWidth * 0.5;
        }

        // 生成圆角顶点（圆弧采样）
        void generateRoundedCorner(vec2 center, vec2 startDir, vec2 endDir, float radius, int segments) {
            float angleStep = (atan(endDir.y, endDir.x) - atan(startDir.y, startDir.x)) / segments;
            if (angleStep < 0.0) angleStep += 2.0 * 3.1415926;

            for (int i = 0; i <= segments; i++) {
                float angle = atan(startDir.y, startDir.x) + i * angleStep;
                vec2 offset = vec2(cos(angle), sin(angle)) * radius;
                vec2 screenPos = center + offset;
                gl_Position = vec4(screenToNdc(screenPos), 0.0, 1.0);
                fragColor = vec4(1.0, 0.0, 0.0, 1.0); // 红色
                EmitVertex();
            }
        }

        void main() {
            // 输入顶点：v0(前邻接), v1(当前段起点), v2(当前段终点), v3(后邻接)
            vec2 v0 = ndcToScreen(gl_in[0].gl_Position.xy);
            vec2 v1 = ndcToScreen(gl_in[1].gl_Position.xy);
            vec2 v2 = ndcToScreen(gl_in[2].gl_Position.xy);
            vec2 v3 = ndcToScreen(gl_in[3].gl_Position.xy);

            // 计算方向向量
            vec2 dir1 = normalize(v2 - v1); // 当前段方向
            vec2 dir2 = normalize(v3 - v2); // 下一段方向
            vec2 dir0 = normalize(v1 - v0); // 上一段方向

            // 线宽的一半（像素单位）
            float halfWidth = lineWidth * 0.5;

            // 计算当前段的左右法向量
            vec2 normal1L = getNormal(dir1); // 左法线（屏幕坐标）
            vec2 normal1R = -normal1L;       // 右法线

            // -------------------------- 1. 处理起点转折（圆角） --------------------------
            if (v0 != v1) { // 不是第一个顶点，绘制起点圆角
                vec2 cornerCenter = v1;
                float cornerRadius = halfWidth;
                generateRoundedCorner(cornerCenter, dir0, dir1, cornerRadius, 8); // 8段圆弧，足够圆滑
            }

            // -------------------------- 2. 绘制当前段的矩形主体 --------------------------
            // 左顶点（v1）
            vec2 p1L = v1 + normal1L;
            gl_Position = vec4(screenToNdc(p1L), 0.0, 1.0);
            fragColor = vec4(1.0, 0.0, 0.0, 1.0);
            EmitVertex();

            // 右顶点（v1）
            vec2 p1R = v1 + normal1R;
            gl_Position = vec4(screenToNdc(p1R), 0.0, 1.0);
            fragColor = vec4(1.0, 0.0, 0.0, 1.0);
            EmitVertex();

            // 左顶点（v2）
            vec2 p2L = v2 + normal1L;
            gl_Position = vec4(screenToNdc(p2L), 0.0, 1.0);
            fragColor = vec4(1.0, 0.0, 0.0, 1.0);
            EmitVertex();

            // 右顶点（v2）
            vec2 p2R = v2 + normal1R;
            gl_Position = vec4(screenToNdc(p2R), 0.0, 1.0);
            fragColor = vec4(1.0, 0.0, 0.0, 1.0);
            EmitVertex();

            // -------------------------- 3. 处理终点转折（圆角） --------------------------
            if (v3 != v2) { // 不是最后一个顶点，绘制终点圆角
                vec2 cornerCenter = v2;
                float cornerRadius = halfWidth;
                generateRoundedCorner(cornerCenter, dir1, dir2, cornerRadius, 8);
            }

            EndPrimitive();
        }
    )";

	// 3. 片段着色器（输出颜色）
	const char* fragmentShaderSource = R"(
        #version 330 core
        in vec4 fragColor;
        out vec4 FragColor;
        void main() {
            FragColor = fragColor;
        }
    )";

	// 编译顶点着色器
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "Vertex Shader Error:\n" << infoLog << std::endl;
	}

	// 编译几何着色器
	unsigned int geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
	glCompileShader(geometryShader);
	glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
		std::cerr << "Geometry Shader Error:\n" << infoLog << std::endl;
	}

	// 编译片段着色器
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cerr << "Fragment Shader Error:\n" << infoLog << std::endl;
	}

	// 链接着色器程序
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, geometryShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "Program Link Error:\n" << infoLog << std::endl;
	}

	// 删除临时着色器
	glDeleteShader(vertexShader);
	glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

// 生成带邻接的线顶点数据（几何着色器需要lines_adjacency输入）
std::vector<float> generateLineWithAdjacencyVertices() {
	// 绘制一个三角形的线带（3个顶点，带邻接）
	std::vector<float> vertices = {
		// 邻接顶点(v0), 顶点1(v1), 顶点2(v2), 邻接顶点(v3) → 第一段
		0.0f,  0.8f, 0.0f,  // v0（顶点1的前邻接，与顶点1重合）
		0.0f,  0.8f, 0.0f,  // v1（顶部）
		-0.6f, -0.5f, 0.0f, // v2（左下）
		0.6f, -0.5f, 0.0f,  // v3（顶点2的后邻接=顶点3）

		// 邻接顶点(v0=顶点1), 顶点2(v1), 顶点3(v2), 邻接顶点(v3=顶点1) → 第二段
		0.0f,  0.8f, 0.0f,  // v0（顶点2的前邻接=顶点1）
		-0.6f, -0.5f, 0.0f, // v1（左下）
		0.6f, -0.5f, 0.0f,  // v2（右下）
		0.0f,  0.8f, 0.0f,  // v3（顶点3的后邻接=顶点1）

		// 邻接顶点(v0=顶点2), 顶点3(v1), 顶点1(v2), 邻接顶点(v3) → 第三段
		-0.6f, -0.5f, 0.0f, // v0（顶点3的前邻接=顶点2）
		0.6f, -0.5f, 0.0f,  // v1（右下）
		0.0f,  0.8f, 0.0f,  // v2（顶部）
		0.6f, -0.5f, 0.0f   // v3（顶点1的后邻接，与顶点3重合）
	};
	return vertices;
}

int test_corner()
{
	// 初始化GLFW + 启用MSAA抗锯齿
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
		std::cerr << "GLFW Init Failed!" << std::endl;
		return -1;
	}
	// 启用4x MSAA抗锯齿（关键：平滑边缘）
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Smooth Wide Line (Shader Control)", NULL, NULL);
	if (!window) {
		std::cerr << "Window Create Failed!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//// 加载GLAD
	//if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
	//    std::cerr << "GLAD Init Failed!" << std::endl;
	//    glfwDestroyWindow(window);
	//    glfwTerminate();
	//    return -1;
	//}


	glewExperimental = GL_TRUE; 
	// 请注意，我们在初始化 GLEW 之前设置 glewExperimental 变量的值为 GL_TRUE
	// 这样做能让 GLEW 在管理 OpenGL 的函数指针时更多地使用现代化的技术，
	// 如果把它设置为 GL_FALSE 的话可能会在使用OpenGL的核心模式时出现一些问题
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}


	// 获取视口大小（传递到几何着色器）
	int viewportWidth = 800, viewportHeight = 600;

	// 生成带邻接的线顶点数据
	std::vector<float> lineVertices = generateLineWithAdjacencyVertices();

	// 配置VAO/VBO
	GLuint VAO, VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, lineVertices.size() * sizeof(float), lineVertices.data(), GL_STATIC_DRAW);
	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// 编译着色器程序
	unsigned int shaderProgram = compileShaderProgram();

	// 获取Uniform位置（传递线宽和视口大小到Shader）
	int lineWidthLoc = glGetUniformLocation(shaderProgram, "lineWidth");
	int viewportSizeLoc = glGetUniformLocation(shaderProgram, "viewportSize");

	// 渲染循环
	while (!glfwWindowShouldClose(window)) {
		// 清屏
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// 使用着色器程序
		glUseProgram(shaderProgram);

		// 传递Uniform（替代glLineWidth，控制线宽）
		glUniform1f(lineWidthLoc, 10.0f); // 线宽10像素
		glUniform2f(viewportSizeLoc, (float)viewportWidth, (float)viewportHeight);

		// 绑定VAO
		glBindVertexArray(VAO);

		// 绘制带邻接的线（GL_LINES_ADJACENCY：每4个顶点为一段，含邻接）
		glDrawArrays(GL_LINES_ADJACENCY, 0, 12); // 12个顶点=3段线

		// 解绑VAO
		glBindVertexArray(0);

		// 交换缓冲区 + 处理事件
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// 清理资源
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}