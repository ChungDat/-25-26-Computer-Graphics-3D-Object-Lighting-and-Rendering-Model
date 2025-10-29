#define _CRT_SECURE_NO_WARNINGS

// openGL libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ImGui libraries
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// custom headers
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "ControlCollapse.h"
#include "utils.h"
#include "Object.h"

// standard
#include <string>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;
const float RADIUS = 10.0f;

float horizontalRotateRate = 0.0f;
float verticalRotateRate = 0.0f;

float fov = 45.0f;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

float pitch = 0.0f;
float yaw = -90.0f;

bool firstMouse = true;
float lastX = WIDTH / 2;
float lastY = HEIGHT / 2;

glm::vec3 cameraPos = glm::vec3(0.0f, 1.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);

glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 2.0f);
glm::vec3 lightColor = glm::vec3(1.0f);

// create camera
Camera camera = Camera(cameraPos, yaw, pitch);
//
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// --------------------------------------------------------------------------------------------------------
void keyboardInputControl(GLFWwindow* window);
//
// control camera view with right click
void mouseControl(GLFWwindow* window, double xpos, double ypos);

// control fov for zooming
void mouseScrollControl(GLFWwindow* window, double xoffset, double yoffset);

int main() {
	// set up
	// ------
	GLFWwindow* window = initWindow(WIDTH, HEIGHT);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		return -1;
	}
	glfwMakeContextCurrent(window);

	// glad: load all OpenGL function pointerss
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouseControl);
	glfwSetScrollCallback(window, mouseScrollControl);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	stbi_set_flip_vertically_on_load(true);
	// wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();


	// vertex data
	// -----------
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	// cube
	//float vertices[] = {
	//	// positions          // texture coords
	//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	//	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	//
	//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	//	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	//	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	//	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	//	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	//
	//	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	//	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	//	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	//
	//	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	//	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	//
	//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	//	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	//	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//
	//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	//	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	//	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	//};

	std::vector<float> vertices = {
		// positions          // normal vectors		// texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,	0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,	1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,	1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,	1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,	0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,	0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,	1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,	1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,	0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,	1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,	1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,	1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,	0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,	0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,	1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,	1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,	1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,	0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,	0.0f, 1.0f,
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	glm::vec3 pointLightColors[] = {
		glm::vec3(1.0f, 0.6f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0, 0.0),
		glm::vec3(0.2f, 0.2f, 1.0f)
	};

	// create VAO, VBO, EBO
	// --------------------
	unsigned int VBO, EBO, cubeVAO; // vertex buffer object, vertex array object
	glGenVertexArrays(1, &cubeVAO); // generate 1 vertex array
	glGenBuffers(1, &VBO); // generate 1 buffer
	glGenBuffers(1, &EBO); // generate 1 element buffer

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(cubeVAO);

	// copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind the buffer to the GL_ARRAY_BUFFER target
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW); // copy the vertex data to the buffer

	// copy our index array in a element buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // bind the buffer to the GL_ELEMENT_ARRAY_BUFFER target
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // copy the index data to the buffer

	// set the vertex attributes pointers
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// unbind VBO, VAO for safety
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// light VAO
	// ---------
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// create texture
	// --------------
	// diffuse map
	unsigned int diffuseMap = createTexture("container2.png");

	// specular map
	unsigned int specularMap = createTexture("container2_specular.png");

	// emission map
	unsigned int emissionMap = createTexture("matrix.jpg");

	// normal object
	Shader myShader = Shader("myVertexShader.vert", "PhongFragmentShader.frag");

	// light source
	Shader lightShader = Shader("lightSourceVertexShader.vert", "lightSourceFragmentShader.frag");

	// model
	Model backpackModel((char*)"backpack/backpack.obj");

	lightShader.use();
	lightShader.setVec3fv("lightColor", lightColor);

	myShader.use();
	myShader.setFloat("material.shininess", 0.3);
	myShader.setInt("material.diffuse", 0);
	myShader.setInt("material.specular", 1);
	myShader.setInt("material.emission", 2);

	myShader.setVec3fv("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	myShader.setVec3fv("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	myShader.setVec3fv("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	myShader.setVec3fv("light.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
	myShader.setFloat("light.constant", 1.0f);
	myShader.setFloat("light.linear", 0.05f);
	myShader.setFloat("light.quadratic", 0.032f);

	myShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
	myShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));

	// ImGui Settings
	bool controlWindowOpened = true;
	int redValue = 25;
	int greenValue = 25;
	int blueValue = 25;

	PresetScenesCollapse presetScenesCollapse = PresetScenesCollapse("Preset Scenes");
	LightCollapse lightCollapse = LightCollapse("Light");
	ObjectCollapse objectCollapse = ObjectCollapse("Add Object");

	Pyramid myObject1 = Pyramid(myShader, "container2.png", "container2_specular.png", "matrix.jpg");

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window)) {
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//ImGui::ShowDemoWindow(); // Show demo window! :)

		ImVec2 controlWindowPos = ImVec2(WIDTH, 0);
		ImVec2 controlWindowSize = ImVec2(200, HEIGHT);
		ImVec2 viewportSize = ImGui::GetMainViewport()->Size;
		 
		ImGui::SetNextWindowPos(
			ImVec2(viewportSize.x - controlWindowSize.x, 0),
			ImGuiCond_Always
		);
		ImGui::SetNextWindowSize(controlWindowSize);		
		ImGui::Begin("Control Window", &controlWindowOpened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		presetScenesCollapse.show();
		lightCollapse.show();
		objectCollapse.show();

		ImGui::End();

		// calculate delta time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		keyboardInputControl(window);

		// render background
		// ------
		glClearColor(0.75f, 0.52f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 horizontalRotate = glm::mat4(1.0f);
		horizontalRotate = glm::rotate(horizontalRotate, glm::radians(horizontalRotateRate), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 verticalRotate = glm::mat4(1.0f);
		verticalRotate = glm::rotate(verticalRotate, glm::radians(verticalRotateRate), glm::vec3(1.0f, 0.0f, 0.0f));

		// light source
		// ------------
		lightShader.use();
		lightShader.setMat4fv("view", view);
		lightShader.setMat4fv("projection", projection);

		// draw
		glBindVertexArray(lightVAO);

		glm::mat4 lightModel = glm::mat4(1.0);
		lightModel = glm::rotate(lightModel, (float)sin(glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
		lightModel = glm::translate(lightModel, lightPos);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));
		lightShader.setMat4fv("model", lightModel);
		lightShader.setVec3fv("lightColor", glm::vec3(1.0, 1.0, 1.0));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		for (int i = 0; i < 4; i++) {
			glm::mat4 lightModel = glm::mat4(1.0);
			lightModel = glm::translate(lightModel, pointLightPositions[i]);
			lightModel = glm::scale(lightModel, glm::vec3(0.2f));
			lightShader.setMat4fv("model", lightModel);
			lightShader.setVec3fv("lightColor", pointLightColors[i]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// normal object
		// -------------
		myShader.use();

		// extract world-space light position and upload to myShader
		glm::vec3 viewPos = camera.Position;
		/*myShader.setVec3fv("viewPos", viewPos);
		myShader.setMat4fv("projection", projection);
		myShader.setMat4fv("view", view);*/
		myShader.setMat4fv("horizontalRotate", horizontalRotate);
		myShader.setMat4fv("verticalRotate", verticalRotate);

		// Directional light 1
		glUniform3f(glGetUniformLocation(myShader.getID(), "dirLight[0].direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(myShader.getID(), "dirLight[0].ambient"), 0.3f, 0.24f, 0.14f);
		glUniform3f(glGetUniformLocation(myShader.getID(), "dirLight[0].diffuse"), 0.7f, 0.42f, 0.26f);
		glUniform3f(glGetUniformLocation(myShader.getID(), "dirLight[0].specular"), 0.5f, 0.5f, 0.5f);
		// Point light 1
		glUniform3f(glGetUniformLocation(myShader.getID(), "pointLight[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(myShader.getID(), "pointLight[0].ambient"), pointLightColors[0].x * 0.1, pointLightColors[0].y * 0.1, pointLightColors[0].z * 0.1);
		glUniform3f(glGetUniformLocation(myShader.getID(), "pointLight[0].diffuse"), pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
		glUniform3f(glGetUniformLocation(myShader.getID(), "pointLight[0].specular"), pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
		glUniform1f(glGetUniformLocation(myShader.getID(), "pointLight[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(myShader.getID(), "pointLight[0].linear"), 0.09);
		glUniform1f(glGetUniformLocation(myShader.getID(), "pointLight[0].quadratic"), 0.032);
		//// Point light 2
		//glUniform3f(glGetUniformLocation(myShader.getID(), "pointLight[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		//glUniform3f(glGetUniformLocation(myShader.getID(), "pointLight[1].ambient"), pointLightColors[1].x * 0.1, pointLightColors[1].y * 0.1, pointLightColors[1].z * 0.1);
		//glUniform3f(glGetUniformLocation(myShader.getID(), "pointLight[1].diffuse"), pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
		//glUniform3f(glGetUniformLocation(myShader.getID(), "pointLight[1].specular"), pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
		//glUniform1f(glGetUniformLocation(myShader.getID(), "pointLight[1].constant"), 1.0f);
		//glUniform1f(glGetUniformLocation(myShader.getID(), "pointLight[1].linear"), 0.09);
		//glUniform1f(glGetUniformLocation(myShader.getID(), "pointLight[1].quadratic"), 0.032);
		//// Point light 3
		//glUniform3f(glGetUniformLocation(myShader.getID(), "pointLight[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		//glUniform3f(glGetUniformLocation(myShader.getID(), "pointLight[2].ambient"), pointLightColors[2].x * 0.1, pointLightColors[2].y * 0.1, pointLightColors[2].z * 0.1);
		//glUniform3f(glGetUniformLocation(myShader.getID(), "pointLight[2].diffuse"), pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
		//glUniform3f(glGetUniformLocation(myShader.getID(), "pointLight[2].specular"), pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
		//glUniform1f(glGetUniformLocation(myShader.getID(), "pointLight[2].constant"), 1.0f);
		//glUniform1f(glGetUniformLocation(myShader.getID(), "pointLight[2].linear"), 0.09);
		//glUniform1f(glGetUniformLocation(myShader.getID(), "pointLight[2].quadratic"), 0.032);
		//// Point light 4
		//glUniform3f(glGetUniformLocation(myShader.getID(), "pointLight[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		//glUniform3f(glGetUniformLocation(myShader.getID(), "pointLight[3].ambient"), pointLightColors[3].x * 0.1, pointLightColors[3].y * 0.1, pointLightColors[3].z * 0.1);
		//glUniform3f(glGetUniformLocation(myShader.getID(), "pointLight[3].diffuse"), pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
		//glUniform3f(glGetUniformLocation(myShader.getID(), "pointLight[3].specular"), pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
		//glUniform1f(glGetUniformLocation(myShader.getID(), "pointLight[3].constant"), 1.0f);
		//glUniform1f(glGetUniformLocation(myShader.getID(), "pointLight[3].linear"), 0.09);
		//glUniform1f(glGetUniformLocation(myShader.getID(), "pointLight[3].quadratic"), 0.032);
		//// Spot light 1
		//glUniform3f(glGetUniformLocation(myShader.getID(), "spotLight[0].position"), camera.Position.x, camera.Position.y, camera.Position.z);
		//glUniform3f(glGetUniformLocation(myShader.getID(), "spotLight[0].direction"), camera.Front.x, camera.Front.y, camera.Front.z);
		//glUniform3f(glGetUniformLocation(myShader.getID(), "spotLight[0].ambient"), 0.5f, 0.5f, 0.5f);
		//glUniform3f(glGetUniformLocation(myShader.getID(), "spotLight[0].diffuse"), 0.8f, 0.8f, 0.0f);
		//glUniform3f(glGetUniformLocation(myShader.getID(), "spotLight[0].specular"), 0.8f, 0.8f, 0.0f);
		//glUniform1f(glGetUniformLocation(myShader.getID(), "spotLight[0].constant"), 1.0f);
		//glUniform1f(glGetUniformLocation(myShader.getID(), "spotLight[0].linear"), 0.09);
		//glUniform1f(glGetUniformLocation(myShader.getID(), "spotLight[0].quadratic"), 0.032);
		//glUniform1f(glGetUniformLocation(myShader.getID(), "spotLight[0].innerCutOff"), glm::cos(glm::radians(12.5f)));
		//glUniform1f(glGetUniformLocation(myShader.getID(), "spotLight[0].outerCutOff"), glm::cos(glm::radians(17.0f)));

		//// draw
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, diffuseMap);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, specularMap);
		//glActiveTexture(GL_TEXTURE2);
		//glBindTexture(GL_TEXTURE_2D, emissionMap);
		//glBindVertexArray(cubeVAO);
		////glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//for (unsigned int i = 0; i < 10; i++) {
		//	glm::mat4 model = glm::mat4(1.0f);
		//	model = glm::translate(model, cubePositions[i]);
		//	float angle = 20.0f * i;
		//	model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		//	myShader.setMat4fv("model", model);

		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//}

		// draw custom objects
		myObject1.setPosition(glm::vec3(-2.0f, 1.0f, -3.0f));
		myObject1.setScale(glm::vec3(0.5f, 0.5f, 0.5f));
		myObject1.setRotation(glm::vec3(10.0f, 10.0f, 10.0f));
		myObject1.draw(view, projection, viewPos, horizontalRotate, verticalRotate);

		// loaded model
		glm::mat4 model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0, -1.0, 0.0));
		model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
		myShader.setMat4fv("model", model);
		//backpackModel.Draw(myShader);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();

	// ImGui terminate
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	return 0;
}

void keyboardInputControl(GLFWwindow* window) {
	const float rotateSpeed = 200.0f * deltaTime;
	// terminate program
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS or glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		verticalRotateRate += rotateSpeed;
		if (verticalRotateRate >= 360)
			verticalRotateRate = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		verticalRotateRate -= rotateSpeed;
		if (verticalRotateRate <= 0)
			verticalRotateRate = 360;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		horizontalRotateRate -= rotateSpeed;
		if (horizontalRotateRate <= 0)
			horizontalRotateRate = 360;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		horizontalRotateRate += rotateSpeed;
		if (horizontalRotateRate >= 360)
			horizontalRotateRate = 0;
	}

	// camera movement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
}

void mouseControl(GLFWwindow* window, double xpos, double ypos) {
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	if (state == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_UNAVAILABLE);
		if (firstMouse) {
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		camera.ProcessMouseMovement(xoffset, yoffset, GL_TRUE);
	}
	else {
		firstMouse = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void mouseScrollControl(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}