#include "windows.h"
#include <iostream>


 

#include "../../3rd/glew-2.2.0/include/GL/glew.h"
#include "../../3rd/glfw/include/GLFW/glfw3.h"

namespace TEST_BASE_EX
{
	GLuint g_shaderProgram;
	// 存储批量生成的 VAO/VBO ID（数组长度=3）
	GLuint g_VAOs[3], g_VBOs[3], g_EBOs[3];

void drawInit()
{
	GLuint vertexShader;
	GLuint fragmentShader;

	//vertexShader
	const char* vertexShaderSource = R"(
								#version 330 core
								layout (location = 0) in vec3 position; 
								void main() 
								{ 
									gl_Position = vec4(position.x, position.y, position.z, 1.0); 
									//可选,在顶点着色器中动态设置点大小,优先级高于 glPointSize() 
									// gl_PointSize = 8.0f;

								})";
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	const char* fragmentShaderSource = R"(
									#version 330 core
									out vec4 FragColor; // 输出最终像素颜色 
									// 声明uniform变量（与C++中"realcolor"完全一致）
									uniform vec4 realcolor; 
									void main() 
									{
										// 使用CPU传入的realcolor作为最终颜色
										FragColor = realcolor;  
										
										//color = vec4(1.0f, 0.5f, 0.2f, 1.0f); 
									})";
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);


	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	memset(infoLog, 0, 512);
	success = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::fragmentShader::COMPILATION_FAILED\n" << infoLog << std::endl;
	}



	g_shaderProgram = glCreateProgram();
	glAttachShader(g_shaderProgram, vertexShader);
	glAttachShader(g_shaderProgram, fragmentShader);
	glLinkProgram(g_shaderProgram);

	glDeleteShader(vertexShader);   
	glDeleteShader(fragmentShader);

	//测试log
	glGetProgramiv(g_shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(g_shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::shaderProgram::Link_FAILED\n" << infoLog << std::endl;
	}




	// 模型1：红色三角形
	float vertices1[] = { 
		-0.8f, -0.5f, 0.0f,  
		0.8f, -0.5f, 0.0f,  
		0.0f,  0.5f, 0.0f 
	};
	// 模型2：绿色三角形
	float vertices2[] = { 
		0.0f, -0.5f, 0.0f,  
		0.8f,  0.5f, 0.0f, 
		-0.8f,  0.5f, 0.0f 
	};
	// 模型3：蓝色三角形
	float vertices3[] = { 
		-0.4f, -0.8f, 0.0f,  
		0.4f, -0.8f, 0.0f,  
		0.0f,  0.8f, 0.0f 
	};


//glDrawElements  顶点索引
	GLuint indices1[] = {  
		0, 1, 2 //Triangle		 
	};

	GLuint indices2[] = {  
		0, 1, 2 //Triangle		 
	};

	GLuint indices3[] = {  
		0, 1, 2 //Triangle		 
	};



	// 批量生成3个VAO（第一个参数=3）
	glGenVertexArrays(3, g_VAOs);
	// 批量生成3个VBO（第一个参数=3）
	glGenBuffers(3, g_VBOs);
	// 批量生成3个EBO（第一个参数=3）
	glGenBuffers(3, g_EBOs);


	// 配置模型1的VAO/VBO
	glBindVertexArray(g_VAOs[0]);          // 绑定第0个VAO
	
	glBindBuffer(GL_ARRAY_BUFFER, g_VBOs[0]); // 绑定第0个VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_EBOs[0]);//绑定第 0 个 EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);



	// 配置模型2的VAO/VBO
	glBindVertexArray(g_VAOs[1]);          // 绑定第1个VAO
	
	glBindBuffer(GL_ARRAY_BUFFER, g_VBOs[1]); // 绑定第1个VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_EBOs[1]);//绑定第 1 个 EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 配置模型3的VAO/VBO
	glBindVertexArray(g_VAOs[2]);          // 绑定第2个VAO
	glBindBuffer(GL_ARRAY_BUFFER, g_VBOs[2]); // 绑定第2个VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_EBOs[2]);//绑定第 2 个 EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices3), indices3, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 解绑所有VAO/VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0); // 解绑VBO
	glBindVertexArray(0);             // 解绑VAO

}


int glfwtest()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


	GLFWwindow * window = glfwCreateWindow(800, 600, "xxxxxx", nullptr, nullptr);

	glfwMakeContextCurrent(window);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	 

	glewExperimental = GL_TRUE; 
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}


	int width, height;
	glfwGetFramebufferSize(window, &width, &height); 

	glViewport(0, 0, width, height); 



	drawInit();
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 0.1f); 
		glClear(GL_COLOR_BUFFER_BIT);



		glUseProgram(g_shaderProgram);

		GLint colorLoc = glGetUniformLocation(g_shaderProgram, "realcolor");
		// 绘制模型1（红色）
		glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f); // 设置颜色
		glBindVertexArray(g_VAOs[0]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		// 绘制模型2（绿色）
		glUniform4f(colorLoc, 0.0f, 1.0f, 0.0f, 1.0f);
		glBindVertexArray(g_VAOs[1]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		// 绘制模型3（蓝色）
		glUniform4f(colorLoc, 0.0f, 0.0f, 1.0f, 1.0f);
		glBindVertexArray(g_VAOs[2]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		


		//解绑VAO（可选）
		glBindVertexArray(0);
		// 交换缓冲区
		glfwSwapBuffers(window);
		//处理事件
		glfwPollEvents();
	}

	// 删除VAO/VBO/着色器程序，释放GPU资源
	glDeleteBuffers(1, g_VBOs);
	glDeleteVertexArrays(1, g_VAOs);
	glDeleteProgram(g_shaderProgram);

	// 销毁窗口/上下文
	glfwTerminate();//释放所有的资源  
	
	return 0;
} 
 

void test(void)
{
	glfwtest();
}


}
