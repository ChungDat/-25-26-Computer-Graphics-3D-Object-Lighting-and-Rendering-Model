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
#include "custom_class/Shader.h"
#include "custom_class/Camera.h"
#include "custom_class/Model.h"
#include "custom_class/ControlCollapse.h"
#include "utils.h"
#include "custom_class/Object.h"
#include "custom_class/Light.h"
#include "custom_class/Axis.h"
#include "custom_class/Plane.h"
#include "custom_class/Container.h"
#include "constants.h"

// standard
#include <string>
#include <iostream>
#include <vector>

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

int numDirLights = 0, numPointLights = 0, numSpotLights = 0;
int numObjects = 0;

// create camera
Camera camera = Camera(cameraPos, yaw, pitch);

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
	// set up context
	// --------------
	GLFWwindow* window = initWindow(WIDTH, HEIGHT);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		return -1;
	}
	glfwMakeContextCurrent(window);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouseControl);
	glfwSetScrollCallback(window, mouseScrollControl);

	// optimization
	// ------------
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);

	stbi_set_flip_vertically_on_load(true);

	// check OpenGL version
	// --------------------
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	
	// setup Dear ImGui context
	// ------------------------
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableKeyboard;     // Disable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// setup Platform/Renderer backends
	// --------------------------------
	ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();

	// initial position for objects
	// ----------------------------
	glm::vec3 initPositions[] = {
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

	glm::vec3 lightPositions[] = {
		glm::vec3(0.0f, 0.0f, 2.0f),
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	glm::vec3 lightColors[] = {
		glm::vec3(1.0f),
		glm::vec3(1.0f, 0.6f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0, 0.0),
		glm::vec3(0.2f, 0.2f, 1.0f)
	};

	// normal object
	Shader PhongShader = Shader("custom_shader/PhongVertexShader.vert", "custom_shader/PhongFragmentShader.frag");
	Shader BlinnPhongShader = Shader("custom_shader/PhongVertexShader.vert", "custom_shader/BlinnPhongFragmentShader.frag");
	Shader GouraudShader = Shader("custom_shader/GouraudVertexShader.vert", "custom_shader/GouraudFragmentShader.frag");
	Shader CookTorranceShader = Shader("custom_shader/PhongVertexShader.vert", "custom_shader/CookTorranceFragmentShader.frag");

	std::vector<std::string> shaderModel = { "Phong", "Gouraud", "Blinn Phong", "Cook Torrance" };
	std::vector<Shader*> shaderList = { &PhongShader, &BlinnPhongShader, &GouraudShader, &CookTorranceShader };

	PhongShader.use();
	PhongShader.setFloat("material.shininess", 32.0f); // Phong uses a lower shininess

	BlinnPhongShader.use();
	BlinnPhongShader.setFloat("material.shininess", 128.0f); // Blinn-Phong needs a higher value

	GouraudShader.use();
	GouraudShader.setFloat("material.shininess", 32.0f); // Gouraud uses similar shininess to Phong

	Shader* objectShader = &PhongShader;

	// light source
	Shader lightShader = Shader("custom_shader/lightSourceVertexShader.vert", "custom_shader/lightSourceFragmentShader.frag");

	// axis shader
	Shader axisShader = Shader("custom_shader/axis.vert", "custom_shader/axis.frag");

	// create objects
	std::vector<Object*> objectList = {
		new Cube(objectShader),
		new Cube(objectShader),
		new Cube(objectShader),
		new Cube(objectShader),
		new Cube(objectShader),
		new Cube(objectShader),
		new Cube(objectShader),
		new Cube(objectShader),
		new Cube(objectShader),
		new Cube(objectShader)
	};

	// assimp model
	Backpack* backpackModel = new Backpack(objectShader);
	objectList.push_back(backpackModel);

	// create lights
	SpotLight* flashLight = new SpotLight(lightShader);

	std::vector<Light*> lightList = {
		new DirectionalLight(lightShader),// 2: Main directional light
		new PointLight(lightShader),      // 1: White orbiting light
		new PointLight(lightShader),      // 3: Orange point light
		new PointLight(lightShader),      // 4: Red point light
		new PointLight(lightShader),      // 5: Yellow point light
		new PointLight(lightShader),      // 6: Blue point light
	};

	// create axis
	Axis axis = Axis(axisShader);
	Plane plane = Plane(axisShader);

	// create box room
	Cube boxRoom = Cube(objectShader);

	// set object positions
	// --------------------
	for (unsigned int i = 0; i < objectList.size(); i++) {
		//float angle = 20.f * i;
		if (i < 10)
			objectList[i]->setPosition(initPositions[i]);
	}
	glm::vec3 scale(0.5, 0.5, 0.5);
	backpackModel->setScale(scale);

	boxRoom.setPosition(glm::vec3(0.0f));
	boxRoom.setScale(glm::vec3(25.0f));
	boxRoom.setDiffuse(glm::vec3(0.5f));
	boxRoom.setSpecular(glm::vec3(0.5f));
	boxRoom.setAlbedo(glm::vec3(1.0f));

	// set light color and positions
	// ---------

	// main directional light
	lightList[0]->setDirection(0.0f, 270.0f);
	lightList[0]->setColor(glm::vec3(1.0f, 1.0f, 1.0f));

	// orbital point light
	lightList[1]->setOrbital(true);

	// point light
	for (int i = 1; i < lightList.size(); i++) {
		lightList[i]->setColor(lightColors[i - 1]);
		lightList[i]->setPosition(lightPositions[i - 1]);
	}

	// ImGui Settings
	int currentRasterizationMode = 0;

	//PresetScenesCollapse presetScenesCollapse = PresetScenesCollapse("Preset Scenes");
	LightCollapse lightCollapse = LightCollapse("Add Light", lightList, numDirLights, numPointLights, numSpotLights, lightShader);
	ObjectCollapse objectCollapse = ObjectCollapse("Add Object", objectList, numObjects, objectShader);
	ObjectProperties objectProperties = ObjectProperties("Object Properties", objectList, numObjects);
	LightProperties lightProperties = LightProperties("Light Properties", lightList, numDirLights, numPointLights, numSpotLights, objectShader);
	Settings settings = Settings("Settings", flashLight, axis, boxRoom, objectShader, shaderModel, shaderList, currentRasterizationMode);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window)) {
		// Start the Dear ImGui frame
		// --------------------------
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// ============================================
		// Start Dear ImGui control
		ImVec2 controlWindowSize = ImVec2(WIDTH / 5, HEIGHT);
		ImVec2 viewportSize = ImGui::GetMainViewport()->Size;
		 
		ImGui::SetNextWindowPos(
			ImVec2(viewportSize.x - controlWindowSize.x, 0),
			ImGuiCond_Always
		);
		ImGui::SetNextWindowSize(controlWindowSize);		
		ImGui::Begin("Control Window", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		
		//presetScenesCollapse.show();
		lightCollapse.show();
		objectCollapse.show();
		objectProperties.show();
		lightProperties.show();
		settings.show();

		ImGui::End();

		// End Dear ImGui control
		// ============================================

		switch (currentRasterizationMode) {
		case 0:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case 1:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case 2:
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			glPointSize(2.0f);
			break;
		}

		// calculate delta time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		keyboardInputControl(window);

		// render background
		// -----
		//glClearColor(0.75f, 0.52f, 0.3f, 1.0f);
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::vec3 viewPos = camera.Position;

		objectShader->use();
		objectShader->setMat4fv("view", view);
		objectShader->setMat4fv("projection", projection);
		objectShader->setVec3fv("viewPos", viewPos);

		lightShader.use();
		lightShader.setMat4fv("view", view);
		lightShader.setMat4fv("projection", projection);

		// light source
		// ------------

		// draw light object
		// -----------------
		for (int i = 0; i < lightList.size(); i++) {
			lightList[i]->draw(deltaTime);
		}

		// extract world-space light position and upload to objectShader
		// -------------------------------------------------------------
		flashLight->setPosition(camera.Position);
		flashLight->setDirection(camera.Front);

		lightProperties.updateLightUniforms();
		flashLight->updateObjectShader(*objectShader, numSpotLights++);

		objectShader->setInt("numDirLights", numDirLights);
		objectShader->setInt("numPointLights", numPointLights);
		objectShader->setInt("numSpotLights", numSpotLights);

		// draw simple object;
		// -------------------
		for (unsigned int i = 0; i < objectList.size(); i++) {
			//float angle = 20.f * i;
			//cubeList[i].setRotation(glm::vec3(angle * 0.2f, angle * 0.5f, angle * 0.8f));
			objectList[i]->draw();
		}
		boxRoom.draw();

		if (axis.isEnabled()) {
			axis.draw(view, projection);
			plane.draw(view, projection);
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();

	// ImGui terminate
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	delete flashLight;
	for (int i = 0; i < lightList.size(); i++) {
		delete lightList[i];
	}
	for (int i = 0; i < objectList.size(); i++) {
		delete objectList[i];
	}

	return 0;
}

void keyboardInputControl(GLFWwindow* window) {
	const float rotateSpeed = 200.0f * deltaTime;
	// terminate program
	if ( glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

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