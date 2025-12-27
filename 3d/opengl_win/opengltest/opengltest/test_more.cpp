#include "windows.h"
#include <iostream>

#include "../../3rd/glew-2.2.0/include/GL/glew.h"
#include "../../3rd/glfw/include/GLFW/glfw3.h"

//#include "../../3rd/Simple OpenGL Image Library/src/SOIL.h"

#include "glm/glm.hpp"
 
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
 
#include "../../3rd/assimp/contrib/stb/stb_image.h"

void key_callback1(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// 当用户按下ESC键,我们设置window窗口的WindowShouldClose属性为true
	// 关闭应用程序
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

}

/*
设置数据以后，我们需要用 glGenVertexArrays 创建一个Vertex Array Object, 然后使用glBindVertexArray绑定VAO，一旦VAO绑定后，
使glGenBuffers 创建一个Vertex Buffer Object, 当然仍然需要使用glBindBuffer绑定VBO；
顺序如下：
1. Generate Vertex Array Object
2. Bind Vertex Array Object
3. Generate Vertex Buffer Object
4. Bind Vertex Buffer Object


3.8开始渲染
3.8.1填充（清理）屏幕
glClearColor(0.1f, 0.9f, 0.5f, 1);//指定填充屏幕的RGBA值
glClear(GL_COLOR_BUFFER_BIT);//指定要清除哪些缓冲区，GL_COLOR_BUFFER_BIT表示颜色缓冲区，GL_DEPTH_BUFFER_BIT表示深度缓冲区，GL_STENCIL_BUFFER_BIT表示模板缓冲区
3.8.2从着色器代码中获取属性信息
GLuint m_simpleProgram = programHandle;
GLuint positionSlot = glGetAttribLocation(m_simpleProgram, "Position");//从着色器源程序中的顶点着色器中获取Position属性
GLuint colorSlot = glGetAttribLocation(m_simpleProgram, "SourceColor");//从着色器源程序中的顶点着色器中获取SourceColor属性
3.8.3开启顶点属性数组
glEnableVertexAttribArray(positionSlot);
glEnableVertexAttribArray(colorSlot);
3.8.4为着色器属性赋值
GLsizei stride = sizeof(Vertex);//单个顶点的数据长度
const GLvoid *pCoords = &vertices[0].Position[0];//顶点数组中的位置数组首地址
const GLvoid *pColors = &vertices[0].Color[0];/顶点数组中的颜色数组首地址
glVertexAttribPointer(positionSlot, 2, GL_FLOAT, GL_FALSE, stride, pCoords);//为顶点着色器位置信息赋值，positionSlot表示顶点着色器位置属性（即，Position）；2表示每一个顶点信息由几个值组成，这个值必须位1，2，3或4；GL_FLOAT表示顶点信息的数据类型；GL_FALSE表示不要将数据类型标准化（即fixed-point）；stride表示数组中每个元素的长度；pCoords表示数组的首地址
glVertexAttribPointer(colorSlot, 4, GL_FLOAT, GL_FALSE, stride, pColors);//同上
3.8.5渲染顶点
GLsizei vertexCount = sizeof(vertices)/sizeof(Vertex);//顶点个数
glDrawArrays(GL_TRIANGLES, 0, vertexCount);//将顶点数组使用三角形渲染，GL_TRIANGLES表示三角形， 0表示数组第一个值的位置，vertexCount表示数组长度
3.8.6渲染完毕，关闭顶点属性数组
glDisableVertexAttribArray(positionSlot);
glDisableVertexAttribArray(colorSlot);

*/


//char *_test_img = "E:\\source_accumulation_vs\\ogltest\\test\\Debug\\test.jpg";
const char *_test_img1 = "D:\\git_work\\blue_test\\3d\\opengl_win\\3rd\\Simple OpenGL Image Library\\test_rect.png";
const char *_test_img = "D:\\git_work\\blue_test\\3d\\opengl_win\\3rd\\Simple OpenGL Image Library\\img_test.png";
//const char *_test_img = "E:\\source_accumulation_vs\\ogltest\\test\\Debug\\test.png";
//char *_test_img = "E:\\source_accumulation_vs\\ogltest\\test\\Debug\\_test_rect.png";
//char *_test_img = "E:\\source_accumulation_vs\\ogltest\\test\\Debug\\_img_test.png";
void draww1()
{




	//postion color 之类的可理解为显卡寄存器,顶点数据是读入到这些地方

			//gl_Position =   transprojection * transview * transform * vec4(position, 1.0f); \
	
	//vertexShader
	GLuint vertexShader;
	const char *vertexShaderSource = R"(#version 330 core
		layout (location = 0) in vec3 position; 
		layout (location = 1) in vec3 color; 
		layout (location = 2) in vec2 texCoord; 
		out vec3 ourColor;
		out vec2 ourTexCoord;
		uniform mat4 transform; 
		uniform mat4 transview; 
		uniform mat4 transprojection; 
		void main() 
		{ 
			gl_Position = vec4(position, 1.0f); 
			ourColor = color; 
			ourTexCoord = vec2(texCoord.x, 1.0 - texCoord.y); 
		})";

	//ourTexCoord = texCoord;\
	//gl_Position =  vec4(position.x, position.y, position.z, 1.0); \
	//gl_Position = transform * vec4(position.x, position.y, position.z, 1.0); \
	//gl_Position =  projection * view * model * vec4(position, 1.0f); \

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


	//fragmentShader
	GLuint fragmentShader;
	const char *fragmentShaderSource = R"(#version 330 core
		in vec3 ourColor; 
		in vec2 ourTexCoord; 
		out vec4 color; 
		uniform sampler2D ourTexture; 
		uniform sampler2D ourTexture1; 
		void main() 
		{ 
			//color = vec4(ourColor.r, ourColor.g, ourColor.b, 1.0);
			color = texture(ourTexture, ourTexCoord);
		})";


			 //color = mix(texture(ourTexture, ourTexCoord), texture(ourTexture1, ourTexCoord), 0.2); \

		//color = vec4(ourColor.r, ourColor.g, ourColor.b, 1.0);
	// color = texture(ourTexture, ourTexCoord);
	// color = texture(ourTexture, ourTexCoord);  
	// color = mix(texture(ourTexture, ourTexCoord), texture(ourTexture1, ourTexCoord), 0.2); \


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



	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::shaderProgram::Link_FAILED\n" << infoLog << std::endl;
	}


	
	//GLfloat vertices1[] = {//tex, rgb, uv
	//	-1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	//	-1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	//	1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	//	1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
	//};


	GLfloat vertices1[] = {//tex, rgb, uv
		-1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
	};


	GLfloat vertices2[] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f
	};


	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 2, // First Triangle		 
		2, 3, 0  // Second Triangle
		//3, 1, 2   // Second Triangle
	};




	GLuint VAO[2];
	GLuint VBO[2];
	GLuint EBO[2];

	glGenVertexArrays(1, &VAO[0]);
	// 绑定VAO, VBO
	glBindVertexArray(VAO[0]);

	glGenBuffers(1, &EBO[0]);


	glGenBuffers(1, &VBO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

	//把之前定义的顶点数据复制到缓冲的内存中
	/*
	它的第一个参数是目标缓冲的类型：顶点缓冲对象当前绑定到GL_ARRAY_BUFFER目标上。第二个参数指定传输数据的大小(以字节为单位)；
	用一个简单的sizeof计算出顶点数据大小就行。第三个参数是我们希望发送的实际数据。第四个参数指定了我们希望显卡如何管理给定的数据
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// 设置顶点属性指针 第一个参数和顶点shader的 layout location 对应, 第二个是当前属性元素个数
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	/* Specify that our color data is going into attribute index 1, and contains three floats per vertex */  
	//6 * sizeof(GLfloat) 步长,每个顶点数据长度, 注意最后一个参数,偏移？？
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); 
	/* Enable attribute index 1 as being used */
	glEnableVertexAttribArray(1);



	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	/* Enable attribute index 1 as being used */
	glEnableVertexAttribArray(2);


	// Load image, create texture and generate mipmaps
	int width, height,nrComponents;
	//unsigned char* image = SOIL_load_image(_test_img, &width, &height, 0, SOIL_LOAD_RGB);
	unsigned char* image = stbi_load(_test_img, &width, &height, &nrComponents, 0);


	int width1, height1,nrComponents1;
	unsigned char* image1 = stbi_load(_test_img1, &width1, &height1, &nrComponents1, 0);


	// Load and create a texture 
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
	 
	// Set the texture wrapping parameters
	/*		

		glTexParameteri 第一个参数指定了纹理目标；我们使用的是2D纹理，因此纹理目标是GL_TEXTURE_2D。
		第二个参数需要我们指定设置的选项与应用的纹理轴。我们打算配置的是WRAP选项，并且指定S和T轴。坐标轴设置（s、t（如果是使用3D纹理那么还有一个r）它们和x、y、z是等价的）
		最后一个参数需要我们传递一个环绕方式
			1 重复（GL_REPEAT）
			2 截取（GL_CLAMP）
			3 镜像重复（GL_MIRRORED_REPEAT_ARB）
			4 边界截取（CLAMP_TO_BORDER_ARB）
			5 边缘截取（GL_CLAMP_TO_EDGE）
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	/*
		当进行放大(Magnify)和缩小(Minify)操作的时候可以设置纹理过滤的选项，比如你可以在纹理被缩小的时候使用邻近过滤，被放大时使用线性过滤。
		GL_NEAREST  选择最近的像素
		GL_LINEAR	选择最近的像素的混合

		纹理坐标不依赖于分辨率(Resolution)，它可以是任意浮点值，所以OpenGL需要知道怎样将纹理像素(Texture Pixel，也叫Texel)
		映射到纹理坐标。当你有一个很大的物体但是纹理的分辨率很低的时候这就变得很重要了。你可能已经猜到了，OpenGL也有对于纹
		理过滤(Texture Filtering)的选项。纹理过滤有很多个选项，但是现在我们只讨论最重要的两种：GL_NEAREST和GL_LINEAR。

		Texture Pixel也叫Texel，你可以想象你打开一张.jpg格式图片，不断放大你会发现它是由无数像素点组成的，这个点就是纹理像素；
		注意不要和纹理坐标搞混，纹理坐标是你给模型顶点设置的那个数组，OpenGL以这个顶点的纹理坐标数据去查找纹理图像上的像素，然
		后进行采样提取纹理像素的颜色。
		GL_NEAREST（也叫邻近过滤，Nearest Neighbor Filtering）是OpenGL默认的纹理过滤方式。当设置为GL_NEAREST的时候，OpenGL会
		选择中心点最接近纹理坐标的那个像素。

	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	/*
	 
	 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	 生成纹理
	· 第一个参数指定了纹理目标(Target)。设置为GL_TEXTURE_2D意味着会生成与当前绑定的纹理对象在同一个目标上的纹理（任何绑定到GL_TEXTURE_1D和GL_TEXTURE_3D的纹理不会受到影响）。
	· 第二个参数为纹理指定多级渐远纹理的级别，如果你希望单独手动设置每个多级渐远纹理的级别的话。这里我们填0，也就是基本级别。
	· 第三个参数告诉OpenGL我们希望把纹理储存为何种格式。我们的图像只有RGB值，因此我们也把纹理储存为RGB值。
	· 第四个和第五个参数设置最终的纹理的宽度和高度。我们之前加载图像的时候储存了它们，所以我们使用对应的变量。
	· 下个参数应该总是被设为0（历史遗留问题）。
	· 第七第八个参数定义了源图的格式和数据类型。我们使用RGB值加载这个图像，并把它们储存为char(byte)数组，我们将会传入对应值。
	· 最后一个参数是真正的图像数据。


		当调用glTexImage2D时，当前绑定的纹理对象就会被附加上纹理图像。然而，目前只有基本级别(Base-level)的纹理图像被加载了，如果要使用多级渐远纹理，
		我们必须手动设置所有不同的图像（不断递增第二个参数）。或者，直接在生成纹理之后调用 glGenerateMipmap。这会为当前绑定的纹理自动生成所有需要的多级渐远纹理。

		

	*/
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D); //直接在生成纹理之后调用 glGenerateMipmap。这会为当前绑定的纹理自动生成所有需要的多级渐远纹理


	//生成了纹理和相应的多级渐远纹理后，释放图像的内存并解绑纹理对象是一个很好的习惯。
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.




//------------------------ texture 2 ---------------	
	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
	glGenerateMipmap(GL_TEXTURE_2D); 


	stbi_image_free(image1);
	glBindTexture(GL_TEXTURE_2D, 0); 

	//------------------------ texture 2 end ---------------	








	// 绘制物体

	glUseProgram(shaderProgram);//在glUseProgram函数调用之后，每个着色器调用和渲染调用都会使用这个程序对象（也就是之前写的着色器)了。



	glm::mat4 trans; //modle -> world
	trans = glm::rotate(trans, -55.0f, glm::vec3(1.0, 0.0, 0.0));
	//trans = glm::rotate(trans, 90.0f, glm::vec3(0.0, 0.0, 1.0));
	//trans = glm::rotate(trans, 45.0f, glm::vec3(0.0, 0.0, 1.0));

	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
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

	
	glBindVertexArray(VAO[0]);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);


	//------------ 绘制第二次
	/* 
	glGenVertexArrays(1, &VAO[1]);
	// 绑定VAO, VBO
	glBindVertexArray(VAO[1]);
	glGenBuffers(1, &VBO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

		 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	// 设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);	 
	glEnableVertexAttribArray(1);

	
	glBindVertexArray(VAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
	 */



	glDeleteShader(vertexShader);   //对了，在把着色器对象链接到程序对象以后，记得删除着色器对象，我们不再需要它们了：
	glDeleteShader(fragmentShader);

}

int glfwtest_more()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


	//GLFWwindow * window = glfwCreateWindow(256, 256, "xxxxxx", nullptr, nullptr);
	GLFWwindow * window = glfwCreateWindow(800, 600, "xxxxxx", nullptr, nullptr);

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


	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	/*
	glViewport数前两个参函数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素），这里我们是直接从GLFW中获取的。我们从GLFW中获取视口的维度而不设置为800*600是为了让它在高DPI的屏幕上（比如说Apple的视网膜显示屏）也能正常工作。
	我们实际上也可以将视口的维度设置为比GLFW的维度小，这样子之后所有的OpenGL渲染将会在一个更小的窗口中显示，这样子的话我们也可以将一些其它元素显示在OpenGL视口之外。
	OpenGL幕后使用glViewport中定义的位置和宽高进行2D坐标的转换，将OpenGL中的位置坐标转换为你的屏幕坐标。例如，OpenGL中的坐标(-0.5, 0.5)有可能（最终）被映射为屏幕中的坐标(200,450)。注意，处理过的OpenGL坐标范围只为-1到1，因此我们事实上将(-1到1)范围内的坐标映射到(0, 800)和(0, 600)。

	*/
	glViewport(0, 0, width, height); //glViewport数前两个参函数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）


	glfwSetKeyCallback(window, key_callback1);

	while (!glfwWindowShouldClose(window))
	{
		////glfwWindowShouldClose函数从开始便检验每一次循环迭代中gLFW是否已经得到关闭指示，如果得到这样的指示，函数就会返回true,并且游戏循环停止运行，之后我们就可以关闭应用了。  
		glClearColor(0.2f, 0.3f, 0.3f, 0.1f); //调用了glClearColor来设置清空屏幕所用的颜色。当调用glClear函数，清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色。
		glClear(GL_COLOR_BUFFER_BIT);

		draww1();

		glfwPollEvents();//检验是否有任务被触发了，（比如鼠标移动和键盘的输入事件），接着调用相应的函数（我们可以通过回调方法设置他们）。我们经常在循环迭代事件中处理函数  
		glfwSwapBuffers(window);//函数会交换颜色缓冲（颜色缓冲是一个GLFW窗口为每一个像素存储的颜色数值的最大缓冲） 它是这迭代中绘制的，也作为输出到显示在屏幕上   

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




