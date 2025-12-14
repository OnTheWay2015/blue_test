#include "windows.h"
#include <iostream>


#include <math.h>
#include <string>



//GLEW
//#define GLEW_STATIC
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

#pragma comment(lib,"glew32.lib")


using namespace std;
//using namespace std;

// Window dimensions
 
// Other Libs
#include "..\SimpleOpenGLImageLibrary\src\SOIL.h"


#include "glm/glm.hpp"
 
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
 
const char *vertexShaderSource1 = 0;
const char *fragmentShaderSource1 = 0;
GLuint VAO[2];
GLuint VBO[2];
GLuint EBO[2];
GLuint texture1;
GLuint texture;
GLuint *texs;

GLuint shaderProgram;

int width, height;

bool initflag = false;
GLuint vertexShader;
GLuint fragmentShader;


//char *_test_img = "g:\\source_accumulation_vs\\ogltest\\test\\Debug\\test.jpg";
char *_test_img1 = "g:\\source_accumulation_vs\\ogltest\\test\\Debug\\_test_rect.png";
char *_test_img = "g:\\source_accumulation_vs\\ogltest\\test\\Debug\\test.png";
//char *_test_img = "g:\\source_accumulation_vs\\ogltest\\test\\Debug\\_test_rect.png";
//char *_test_img = "g:\\source_accumulation_vs\\ogltest\\test\\Debug\\_img_test.png";




char *_test_img_front = "g:\\source_accumulation_vs\\ogltest\\test\\Debug\\_test_rect_front.png";
char *_test_img_back = "g:\\source_accumulation_vs\\ogltest\\test\\Debug\\_test_rect_back.png";
char *_test_img_up = "g:\\source_accumulation_vs\\ogltest\\test\\Debug\\_test_rect_up.png";
char *_test_img_down = "g:\\source_accumulation_vs\\ogltest\\test\\Debug\\_test_rect_down.png";
char *_test_img_right = "g:\\source_accumulation_vs\\ogltest\\test\\Debug\\_test_rect_right.png";
char *_test_img_left = "g:\\source_accumulation_vs\\ogltest\\test\\Debug\\_test_rect_left.png";
 
 

int rot = 0;
float ag = 0;
int ri = 5;
float camera_z = 0.0;
float pi = 3.1415926;

float lookat_x = 0;
float lookat_y = 0;
float lookat_z = 0;

int readfile1(const char *file, const char **buff)
{
	FILE *stream;

	if ((stream = fopen(file, "r")) == NULL)
	{
		fprintf(stderr, "cannot open output file.\n");
		return 1;
	}

	fseek(stream, 0, SEEK_END);
	int lSize = ftell(stream) + 1;
	*buff = (char*)malloc(lSize);
	memset((void*)*buff, 0, lSize);
	/*seek to the beginning of the file*/
	fseek(stream, 0, SEEK_SET);
	/*read the data and display it*/
	fread((void*)*buff, 1, lSize, stream);
	//printf("%s\n\n", *buff);
	fclose(stream);
	return 0;
}
void key_callback1(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// 当用户按下ESC键,我们设置window窗口的WindowShouldClose属性为true
	// 关闭应用程序
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	switch (key)
	{
	case GLFW_KEY_RIGHT:
		ag+=2;
		break;
	case GLFW_KEY_LEFT:
		ag-=2;
		break;
	case GLFW_KEY_DOWN:
		camera_z--;
		break;
	case GLFW_KEY_UP:
		camera_z++;
		break;
	case GLFW_KEY_SPACE:
		ag = 0;
		ri = 3;
		camera_z = 0.0;

		lookat_x = 0;
		lookat_y = 0;
		lookat_z = 0;

		break;
	case GLFW_KEY_A:	
		//lookat_x--;
		break;
		
	case GLFW_KEY_D:
		//lookat_x++;
		break;

	case GLFW_KEY_W:
		//lookat_y++;
		break;
	case GLFW_KEY_S:
		//lookat_y--;
		break;
	default:
		break;
	} 


	
	if (ag > 360)
	{
		ag = 0;
	}
}
 


void bindtex(int len, char ** ary)
{
	texs = new GLuint[len];

	int *width = new int[len];
	int *height = new int[len];
	int i=0;
	char *file;
	unsigned char* image;
	for (; i < len; i++)
	{
		image = SOIL_load_image(ary[i], &width[i], &height[i], 0, SOIL_LOAD_RGB);
		glGenTextures(1, &texs[i]);
		glBindTexture(GL_TEXTURE_2D, texs[i]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width[i], height[i], 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		//glTexSubImage2D  释放显存

		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	

}

void bindvetex(int idx)
{
	GLfloat vertices1[] = {//vex[3], rgb[3], tex_uv[2], tex_idx[2]
		-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0, 0,
		-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0, 0,
		1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0, 0,
		1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0, 0,

		-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1, 0,
		-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1, 0,
		1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1, 0,
		1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1, 0,

		-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2, 0,
		-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 2, 0,
		-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 2, 0,
		-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 2, 0,

		1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 3, 0,
		1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 3, 0,
		1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 3, 0,
		1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 3, 0,

		-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 4, 0,
		1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 4, 0,
		1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 4, 0,
		-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 4, 0,

		-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 5, 0,
		1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 5, 0,
		1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 5, 0,
		-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 5, 0
	};


	GLfloat vertices2[] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f
	};


	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 2 // First Triangle		 
		, 2, 3, 0  // Second Triangle

		, 4, 5, 6
		, 6, 7, 4

		, 8, 9, 10
		, 10, 11, 8

		, 12, 13, 14
		, 14, 15, 12

		, 16, 17, 18
		, 18, 19, 16
		, 20, 21, 22
		, 22, 23, 20
	};



	GLuint *_indices = &indices[idx * 6];


	glGenVertexArrays(1, &VAO[0]);
	// 绑定VAO, VBO
	glBindVertexArray(VAO[0]);

	glGenBuffers(1, &EBO[0]);


	glGenBuffers(1, &VBO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);


	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6, _indices, GL_STATIC_DRAW);


	int vexdatalen = (10 - 2)* sizeof(GLfloat) + 2 * sizeof(GLint);

	// 设置顶点属性指针 第一个参数和顶点shader的 layout location 对应, 第二个是当前属性元素个数
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vexdatalen, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	/* Specify that our color data is going into attribute index 1, and contains three floats per vertex */
	//6 * sizeof(GLfloat) 步长,每个顶点数据长度, 注意最后一个参数,偏移？？
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vexdatalen, (GLvoid*)(3 * sizeof(GLfloat)));
	/* Enable attribute index 1 as being used */
	glEnableVertexAttribArray(1);



	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vexdatalen, (GLvoid*)(6 * sizeof(GLfloat)));
	/* Enable attribute index 2 as being used */
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 2, GL_INT, GL_FALSE, vexdatalen, (GLvoid*)(8 * sizeof(GLfloat)));
	/* .faq 怎样绑定整型数据到 shader ?? */
	glEnableVertexAttribArray(3);


	 

	float rx = 0;
	float ry = 0;

	glUseProgram(shaderProgram);//在glUseProgram函数调用之后，每个着色器调用和渲染调用都会使用这个程序对象（也就是之前写的着色器)了。

	rot++;

	rx = ri*cos(2 * ag*pi / 360);
	ry = ri*sin(2 * ag*pi / 360);
	
	glm::mat4 trans; //modle -> world

	trans = glm::scale(trans, glm::vec3(1.0, 1.0, 1.0));//

	GLuint transformLoc = glGetUniformLocation(shaderProgram, "transform");//必须在使用shader后生效. glUseProgram(shaderProgram)后
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	glm::mat4 view;// world  -> view 
	
	view = glm::lookAt(glm::vec3(5, camera_z, 5),  //.faq 为什么对角看的时候图会出错？？ glm::vec3(5, camera_z, 0)  glm::vec3(0, camera_z, 5) 正常
		glm::vec3(0, 0, 0),
		glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 projection; // view   -> clip
	projection = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 10000.0f); //透视
	
	GLuint viewLoc = glGetUniformLocation(shaderProgram, "transview");//必须在使用shader后生效. glUseProgram(shaderProgram)后
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	GLuint projectionLoc = glGetUniformLocation(shaderProgram, "transprojection");//必须在使用shader后生效. glUseProgram(shaderProgram)后
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	char strtmp[30];
	 

	glActiveTexture(GL_TEXTURE0 + idx);
	glBindTexture(GL_TEXTURE_2D, texs[idx]);	

	glUniform1i(glGetUniformLocation(shaderProgram, "tex0"), 0);

	glEnable(GL_DEPTH_TEST);
	
	glBindVertexArray(VAO[0]);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteShader(vertexShader);   
	glDeleteShader(fragmentShader);

}

void bindvetexs()
{
	int i=0;
	while (i<6)
	{
		i++;
		bindvetex(i);
			
	}

}
void draww1()
{
 
	//vertexShader
	

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource1, NULL);
	glCompileShader(vertexShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	//fragmentShader
	 

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource1, NULL);
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



	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::shaderProgram::Link_FAILED\n" << infoLog << std::endl;
	} 

	
	/* 查看加载的shader，看看是不是和写的一样*/
	GLchar infoLog1[1024];
	memset(infoLog1, 0, 1024);
	glGetShaderSource(fragmentShader, 1024, NULL, infoLog1);
	std::cout << "glGetShaderSource ==> \n" << infoLog1 << std::endl;


	//GLfloat vertices1[] = {//vex[3], rgb[3], tex_uv[2], tex_idx[1]
	//	-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	//	-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	//	1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	//	1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	//	
	//	-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	//	-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	//	1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	//	1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

	//	-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 0.0f,
	//	-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f,
	//	-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 2.0f, 0.0f,
	//	-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 2.0f, 0.0f,
	//	
	//	1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 3.0f, 0.0f,
	//	1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 3.0f, 0.0f,
	//	1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 3.0f, 0.0f,
	//	1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 3.0f, 0.0f,

	//	-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 4.0f, 0.0f,
	//	1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 4.0f, 0.0f,
	//	1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 4.0f, 0.0f,
	//	-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 4.0f, 0.0f,

	//	-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 5.0f, 0.0f,
	//	1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 5.0f, 0.0f,
	//	1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 5.0f, 0.0f,
	//	-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 5.0f,0.0f
	//};



	GLfloat vertices1[] = {//vex[3], rgb[3], tex_uv[2], tex_idx[2]
		-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0, 0,
		-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0, 0,
		1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0, 0,
		1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0, 0,

		-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1, 0,
		-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1, 0,
		1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1, 0,
		1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1, 0,

		-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2, 0,
		-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 2, 0,
		-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 2, 0,
		-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 2, 0,

		1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 3, 0,
		1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 3, 0,
		1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 3, 0,
		1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 3, 0,

		-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 4, 0,
		1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 4, 0,
		1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 4, 0,
		-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 4, 0,

		-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 5, 0,
		1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 5, 0,
		1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 5, 0,
		-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 5, 0
	};


	GLfloat vertices2[] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f
	};


	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 2 // First Triangle		 
		, 2, 3, 0  // Second Triangle

		, 4, 5, 6
		, 6, 7, 4

		, 8, 9, 10
		, 10, 11, 8

		, 12, 13, 14
		, 14, 15, 12

		, 16, 17, 18
		, 18, 19, 16
		, 20, 21, 22
		, 22, 23, 20
	};
	


	char* ary[] = {
		_test_img_front,
		_test_img_back,
		_test_img_up,
		_test_img_down,
		_test_img_right,
		_test_img_left
	};


 
	glGenVertexArrays(1, &VAO[0]);
	// 绑定VAO, VBO
	glBindVertexArray(VAO[0]);

	glGenBuffers(1, &EBO[0]);


	glGenBuffers(1, &VBO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);


	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	 


	int vexdatalen = (10 - 2)* sizeof(GLfloat) + 2 * sizeof(GLint);

	// 设置顶点属性指针 第一个参数和顶点shader的 layout location 对应, 第二个是当前属性元素个数
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vexdatalen, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	/* Specify that our color data is going into attribute index 1, and contains three floats per vertex */
	//6 * sizeof(GLfloat) 步长,每个顶点数据长度, 注意最后一个参数,偏移？？
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vexdatalen, (GLvoid*)(3 * sizeof(GLfloat)));
	/* Enable attribute index 1 as being used */
	glEnableVertexAttribArray(1);



	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vexdatalen, (GLvoid*)(6 * sizeof(GLfloat)));
	/* Enable attribute index 2 as being used */
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 2, GL_INT, GL_FALSE, vexdatalen, (GLvoid*)(8 * sizeof(GLfloat)));
	/* .faq 怎样绑定整型数据到 shader ?? */
	glEnableVertexAttribArray(3);


	bindtex(6,ary);
	return;
	// Load and create a texture 
	// Load image, create texture and generate mipmaps
	int width, height;
	unsigned char* image = SOIL_load_image(_test_img, &width, &height, 0, SOIL_LOAD_RGB);


	int width1, height1;
	unsigned char* image1 = SOIL_load_image(_test_img1, &width1, &height1, 0, SOIL_LOAD_RGB);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
	 
 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D); //直接在生成纹理之后调用 glGenerateMipmap。这会为当前绑定的纹理自动生成所有需要的多级渐远纹理
	
	 
	SOIL_free_image_data(image); 
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.




//------------------------ texture 2 ---------------	

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
	glGenerateMipmap(GL_TEXTURE_2D); 

	//glTexSubImage2D  释放显存

	SOIL_free_image_data(image1);
	glBindTexture(GL_TEXTURE_2D, 0); 

	//------------------------ texture 2 end ---------------	






	 
}


void drawwxx()
{ 
	/*
		Vclip=Mprojection⋅Mview⋅Mmodel⋅Vlocal 

		V 矩阵, M 变换矩阵
	*/
	
	float rx = 0;
	float ry = 0;

	glUseProgram(shaderProgram);//在glUseProgram函数调用之后，每个着色器调用和渲染调用都会使用这个程序对象（也就是之前写的着色器)了。

	rot++;
	

	rx = ri*cos(2 * ag*pi / 360);
	ry = ri*sin(2 * ag*pi / 360);


	/*std::cout 
		<< "ri[" << ri << "]"
		<< "ag[" << ag << "]"
		<< "cx[" << rx << "]"
		<< "cy[" << ry << "]"
		<< "camera_z[" << camera_z << "]"
		<< std::endl;*/


	glm::mat4 trans; //modle -> world




	//@arg1,转换矩阵  
	//@arg2,旋转角度  
	//@arg3,旋转的基准轴
	//trans = glm::rotate(trans, (GLfloat)rot, glm::vec3(1.0, 0.0,0.0));
	//trans = glm::rotate(trans, (GLfloat)rot, glm::vec3(0.0, 0.0, -1.0));
	//trans = glm::rotate(trans, (GLfloat)rot, glm::vec3(0.0, 0.0, 1.0));

	//trans = glm::scale(trans, glm::vec3(100.0, 100.0, 100.0));// .faq 显示不了
	//trans = glm::scale(trans, glm::vec3(400.0, 300.0, 1.0));//当正交视图时，要放大，当前顶点1坐标为1像素,  ( 1,1, 0 ) 到 (1, -1, 0) 距离为 2 像素 
	trans = glm::scale(trans, glm::vec3(1.0, 1.0, 1.0));// 

	//trans= glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

	//trans = glm::translate(trans, glm::vec3(0.0f, 0.5f, 0.0f));
	//trans = glm::translate(trans, glm::vec3(0.5f, 0.5f, 0.5f));	

	GLuint transformLoc = glGetUniformLocation(shaderProgram, "transform");//必须在使用shader后生效. glUseProgram(shaderProgram)后
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));



	/*
			观察空间经常被人们称之OpenGL的摄像机(Camera)（所以有时也称为摄像机空间(Camera Space)或视觉空间(Eye Space)）。
		观察空间是将世界空间坐标转化为用户视野前方的坐标而产生的结果。因此观察空间就是从摄像机的视角所观察到的空间。
		而这通常是由一系列的位移和旋转的组合来完成，平移/旋转场景从而使得特定的对象被变换到摄像机的前方。这些组合在
		一起的变换通常存储在一个观察矩阵(View Matrix)里，它被用来将世界坐标变换到观察空间。

		将摄像机向后移动，和将整个场景向前移动是一样的。这正是观察矩阵所做的，我们以相反于摄像机移动的方向移动整个场景
	*/
	glm::mat4 view;// world  -> view 
	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -1000.0f)); //.faq vec3和尺寸的关系  -3.0f和坐标的关系?比例？
	


	//定义一个摄像机位置，一个目标位置和一个表示世界空间中的上向量的向量（我们计算右向量使用的那个上向量）。接着GLM就会创建一个LookAt矩阵.
	//view = glm::lookAt(glm::vec3(0.0f, 1.0f, 3.0f),
	//	glm::vec3(0.0f, 0.0f, 0.0f),
	//	glm::vec3(0.0f, 1.0f, 0.0f));  //lookat  





	//view = glm::lookAt(glm::vec3(5, 0, 5), //x,y,z和设置的 世界空间中的坐标     
	//	glm::vec3(0, 0, 0),
	//	glm::vec3(0.0f, 1.0f, 0.0f));  //摄相机围绕移动的那个轴, XY平面对应是Z轴，YZ平面对应X轴,ZX平面对应Y轴

	view = glm::lookAt(glm::vec3(ry, camera_z, rx),
		glm::vec3(lookat_x, lookat_y, lookat_z),
		glm::vec3(0.0f, 1.0f, 0.0f));  




	glm::mat4 projection; // view   -> clip
	projection = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 10000.0f); //透视

	//projection = glm::ortho(0.0f, (GLfloat)width, 0.0f, (GLfloat)height, 0.1f, 100.0f); //正交  .faq 为什么不能显示？ a. 显示的物体要在最近视平面和最远视平面之间.
	//projection = glm::ortho(-(GLfloat)width / 2, (GLfloat)width / 2, -(GLfloat)height / 2, (GLfloat)height / 2, 0.1f, 100000.0f); //正交 
	//projection = glm::ortho(-(GLfloat)width, (GLfloat)width , -(GLfloat)height , (GLfloat)height, 0.1f, 100.0f); //正交  



	GLuint viewLoc = glGetUniformLocation(shaderProgram, "transview");//必须在使用shader后生效. glUseProgram(shaderProgram)后
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	GLuint projectionLoc = glGetUniformLocation(shaderProgram, "transprojection");//必须在使用shader后生效. glUseProgram(shaderProgram)后
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection)); 


	// Bind Texture
	//glActiveTexture(GL_TEXTURE0);  //OpenGL至少保证有16个纹理单元供你使用，也就是说你可以激活从GL_TEXTURE0到GL_TEXTRUE15。它们都是按顺序定义的，所以我们也可以通过GL_TEXTURE0 + 8的方式获得GL_TEXTURE8，这在当我们需要循环一些纹理单元的时候会很有用。
	//glBindTexture(GL_TEXTURE_2D, texture);
	//glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);//对应到shader

	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, texture1);
	//glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture1"), 1);

	  
	//string basestr = "tex";
	char strtmp[30];
	int i = 0;
	for (; i < 6; i++)
	{		
		
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, texs[i]);
		char idx_str = i+48;
		 
		memset(strtmp, 0, 30);
		sprintf( strtmp,"tex%c",idx_str );

		glUniform1i(glGetUniformLocation(shaderProgram, strtmp), i);
	}



	glEnable(GL_DEPTH_TEST);



	

	glBindVertexArray(VAO[0]);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);


	glDeleteShader(vertexShader);   //对了，在把着色器对象链接到程序对象以后，记得删除着色器对象，我们不再需要它们了：
	glDeleteShader(fragmentShader);


	//printf(" err[%d] ?\n", glGetError());  //显示当前错误重置
}









void draw_test()
{


	glUseProgram(shaderProgram);//在glUseProgram函数调用之后，每个着色器调用和渲染调用都会使用这个程序对象（也就是之前写的着色器)了。

	rot++;
	glm::mat4 trans; //modle -> world

	trans = glm::rotate(trans, (GLfloat)rot, glm::vec3(1.0, 0.0, 0.0));

	trans = glm::scale(trans, glm::vec3(0.2, 0.2, 0.2));
	trans = glm::translate(trans, glm::vec3(0.0f, 0.5f, 0.0f));

	GLuint transformLoc = glGetUniformLocation(shaderProgram, "transform");//必须在使用shader后生效. glUseProgram(shaderProgram)后
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));


	glm::mat4 view;// world  -> view 
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	glm::mat4 projection; // view   -> clip
	projection = glm::perspective(45.0f, (GLfloat)800 / (GLfloat)600, 0.1f, 100.0f);

	GLuint viewLoc = glGetUniformLocation(shaderProgram, "transview");//必须在使用shader后生效. glUseProgram(shaderProgram)后
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	GLuint projectionLoc = glGetUniformLocation(shaderProgram, "transprojection");//必须在使用shader后生效. glUseProgram(shaderProgram)后
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


	// Bind Texture
	glActiveTexture(GL_TEXTURE0);  //OpenGL至少保证有16个纹理单元供你使用，也就是说你可以激活从GL_TEXTURE0到GL_TEXTRUE15。它们都是按顺序定义的，所以我们也可以通过GL_TEXTURE0 + 8的方式获得GL_TEXTURE8，这在当我们需要循环一些纹理单元的时候会很有用。
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);//对应到shader

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture1"), 1);


	glEnable(GL_DEPTH_TEST);



	initflag = true;

	glBindVertexArray(VAO[0]);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);


	glDeleteShader(vertexShader);   //对了，在把着色器对象链接到程序对象以后，记得删除着色器对象，我们不再需要它们了：
	glDeleteShader(fragmentShader);


	//printf(" err[%d] ?\n", glGetError());  //显示当前错误重置
}


void drawact()
{
 
	void *pixels = malloc(width*height * 4);
	if (!initflag)
	{

		initflag = true;
		draww1();
		//draw_test();
		
		

		//GLAPI void GLAPIENTRY glReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);
		//glReadPixels(0, 0, 800, 600, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		//int save_result = SOIL_save_image("imgsave.tga", SOIL_SAVE_TYPE_TGA, width, height, 4, (unsigned char*)pixels);
	}
	//bindvetexs();
	drawwxx();
}

int glfwtest_more()
{

	printf("load vertexShaderSource:\n");
	readfile1("G:/source_accumulation_vs/ogltest/test/_vertexShaderSource.txt", &vertexShaderSource1);

	printf("load fragmentShaderSource:\n");
	readfile1("G:/source_accumulation_vs/ogltest/test/_fragmentShaderSource.txt", &fragmentShaderSource1);


	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//版本要求
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);// .faq?
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // 是否可以调整窗口大小


	//GLFWwindow * window = glfwCreateWindow(256, 256, "LearnOpenGL", nullptr, nullptr);
	GLFWwindow * window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);

	glfwMakeContextCurrent(window);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}


	glewExperimental = GL_TRUE; //请注意，我们在初始化GLEW之前设置glewExperimental变量的值为GL_TRUE，这样做能让GLEW在管理OpenGL的函数指针时更多地使用现代化的技术，如果把它设置为GL_FALSE的话可能会在使用OpenGL的核心模式时出现一些问题
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}


	
	glfwGetFramebufferSize(window, &width, &height);

 
	glViewport(0, 0, width, height); //glViewport数前两个参函数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）


	glfwSetKeyCallback(window, key_callback1);

	while (!glfwWindowShouldClose(window))
	{
	 
		glClearColor(0.2f, 0.3f, 0.3f, 0.1f);  
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		drawact();

		glfwPollEvents();//检验是否有任务被触发了，（比如鼠标移动和键盘的输入事件），接着调用相应的函数（我们可以通过回调方法设置他们）。我们经常在循环迭代事件中处理函数  
		glfwSwapBuffers(window);//函数会交换颜色缓冲（颜色缓冲是一个GLFW窗口为每一个像素存储的颜色数值的最大缓冲） 它是这迭代中绘制的，也作为输出到显示在屏幕上   
		//break;
	}

	while(true)
	{
		Sleep(1);
	}
	glfwTerminate();//释放所有的资源  
	return 0;
} 


/*
· 局部空间(Local Space，或者称为物体空间(Object Space))
· 世界空间(World Space)
· 观察空间(View Space，或者称为视觉空间(Eye Space))
· 裁剪空间(Clip Space)
· 屏幕空间(Screen Space)
*/



int main(void)
{	 
	glfwtest_more();
	system("pause");
	return 0;
}
