//using namespace std;

#include "test_model_loading.h"


namespace MODEL_TEST
{








	//{
	//	Model ourModel(FileSystem::getPath("resources/objects/vampire/dancing_vampire.dae"));
	//	Animation danceAnimation(FileSystem::getPath("resources/objects/vampire/dancing_vampire.dae"), &ourModel);
	//	Animator animator(&danceAnimation);
	//}


	
	glm::mat4 GetViewMatrix()
	{
		glm::vec3 Position{0.0f, 0.0f, 100.0f};
		glm::vec3 Front{ 0.0f, 0.0f, -1.0f };
		glm::vec3 Up{0.0f, 1.0f, 0.0f };

		return glm::lookAt(Position, Position + Front, Up);
	}
	
	// timing
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	void runAni()
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

		int SCR_WIDTH = 1080;
		int SCR_HEIGHT = 960;
		// glfw window creation
		// --------------------
		GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "xxxxxx", nullptr, nullptr);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(window);
		//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		//glfwSetCursorPosCallback(window, mouse_callback);
		//glfwSetScrollCallback(window, scroll_callback);

		// tell GLFW to capture our mouse
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);



		glewExperimental = GL_TRUE; //请注意，我们在初始化GLEW之前设置glewExperimental变量的值为GL_TRUE，这样做能让GLEW在管理OpenGL的函数指针时更多地使用现代化的技术，如果把它设置为GL_FALSE的话可能会在使用OpenGL的核心模式时出现一些问题
		if (glewInit() != GLEW_OK)
		{
			std::cout << "Failed to initialize GLEW" << std::endl;
			return;
		}


		/*
图像文件（如 PNG/JPG/BMP）的 Y 轴定义绝大多数图像文件格式（以及图像处理软件，如 PS、画图工具）
	遵循「自上而下」的 Y 轴方向,图像的左上角是纹理坐标的原点 (0, 0), Y 轴正方向是「向下」延伸，右下角对应纹理坐标 (1, 1)
	简单说：图像文件存储像素数据时，是先存第一行（顶部），再存第二行，直到最后一行（底部）。
3D 图形 API（如 OpenGL/Vulkan/Direct3D）的纹理 Y 轴定义所有主流 3D 图形 
	API 在处理纹理映射（将纹理贴到 3D 模型表面）时，遵循「自下而上」的 Y 轴方向（基于纹理坐标 UV 系统）：
	纹理的左下角是 UV 坐标的原点 (0, 0),Y 轴正方向是「向上」延伸，右上角对应 UV 坐标 (1, 1)
	这是 3D 图形渲染的标准约定，与模型的顶点 UV 坐标绑定一致。	

图形 API 与图像文件的「Y 轴方向不一致」,两个体系对纹理的「Y 轴正方向」定义完全相反，不翻转的话纹理会出现上下颠倒的问题
		*/
		// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
		stbi_set_flip_vertically_on_load(true);//必须在 stbi_load() 之前调用
		// configure global opengl state
		// -----------------------------
		glEnable(GL_DEPTH_TEST);

		// build and compile shaders
		// -------------------------
		Shader ourShader(
			FileSystem::getPath("resources/shaders/anim_model.vs").c_str(),
			FileSystem::getPath("resources/shaders/anim_model.fs").c_str());


		// load models
		// -----------
		Model ourModel(FileSystem::getPath("resources/models/obj/vampire/dancing_vampire.dae"));
		Animation danceAnimation(FileSystem::getPath("resources/models/obj/vampire/dancing_vampire.dae"), &ourModel);
		Animator animator(&danceAnimation);


		// draw in wireframe
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// render loop
		// -----------
		while (!glfwWindowShouldClose(window))
		{
			// per-frame time logic
			// --------------------
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			// input
			// -----
			//processInput(window);

			//run ani
			animator.UpdateAnimation(deltaTime);
			//animator.UpdateAnimation(0);

			// render
			// ------
			glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// don't forget to enable shader before setting uniforms
			ourShader.use();

			//float camera_ZOOM = 45.0f;
			float camera_ZOOM = 65.0f;
			// view/projection transformations
			glm::mat4 projection = glm::perspective(
				glm::radians(camera_ZOOM),
				(float)SCR_WIDTH / (float)SCR_HEIGHT,
				0.1f, 500.0f);
			glm::mat4 view = GetViewMatrix();// camera.GetViewMatrix;
			ourShader.setMat4("projection", projection);
			ourShader.setMat4("view", view);

			auto transforms = animator.GetFinalBoneMatrices();
			for (int i = 0; i < transforms.size(); ++i)
				ourShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);


			// render the loaded model
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f)); // translate it down so it's at the center of the scene
			model = glm::scale(model, glm::vec3(.3f, .3f, .3f));	// it's a bit too big for our scene, so scale it down
			ourShader.setMat4("model", model);
			ourModel.Draw(ourShader);


			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			// -------------------------------------------------------------------------------
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		// glfw: terminate, clearing all previously allocated GLFW resources.
		// ------------------------------------------------------------------
		glfwTerminate();
	}

	void runModel()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		int SCR_WIDTH = 1080;
		int SCR_HEIGHT = 960;
		// glfw window creation
		GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "xxxxxx", nullptr, nullptr);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(window);

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			std::cout << "Failed to initialize GLEW" << std::endl;
			return;
		}
		// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
		//stbi_set_flip_vertically_on_load(true);

		glEnable(GL_DEPTH_TEST);

		// -------------------------


		// build and compile shaders
		Shader ourShader(
			FileSystem::getPath("resources/shaders/1.model_loading.vs").c_str(),
			FileSystem::getPath("resources/shaders/1.model_loading.fs").c_str());


		// load models
		// -----------
		Model ourModel(FileSystem::getPath("resources/models/obj/backpack/backpack.obj"));

		// render loop
		// -----------
		while (!glfwWindowShouldClose(window))
		{
			// per-frame time logic
			// --------------------
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			// input
			// -----
			//processInput(window);


			// render
			// ------
			glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// don't forget to enable shader before setting uniforms
			ourShader.use();

			//float camera_ZOOM = 45.0f;
			float camera_ZOOM = 65.0f;
			// view/projection transformations
			glm::mat4 projection = glm::perspective(
				glm::radians(camera_ZOOM),
				(float)SCR_WIDTH / (float)SCR_HEIGHT,
				0.1f, 500.0f);
			glm::mat4 view = GetViewMatrix();// camera.GetViewMatrix;
			ourShader.setMat4("projection", projection);
			ourShader.setMat4("view", view);

			// render the loaded model
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f)); // translate it down so it's at the center of the scene
			model = glm::scale(model, glm::vec3(.3f, .3f, .3f));	// it's a bit too big for our scene, so scale it down
			ourShader.setMat4("model", model);
			ourModel.Draw(ourShader);


			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			// -------------------------------------------------------------------------------
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		// glfw: terminate, clearing all previously allocated GLFW resources.
		// ------------------------------------------------------------------
		glfwTerminate();
	}

	void test()
	{
		//runModel();	
		runAni();
	}
}


