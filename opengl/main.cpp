// ============================
// TINH LAI ORBITAL MOTION GUI
// TODO:	THEM CHUC NANG XOA
//			THEM PRESET SCENE
//			THEM CHUC NANG WIREFRAME
//			THEM BLINN PHONG
//			THEM GOURAUD
//			THEM 3D OBJJECT LIGHTING
// ============================
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
#include "Light.h"
#include "Axis.h"

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

// white light cube
glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 2.0f);
glm::vec3 lightColor = glm::vec3(1.0f);

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
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableKeyboard;     // Disable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();

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

	// axis shader
	Shader axisShader = Shader("axis.vert", "axis.frag");

	// model
	Model backpackModel((char*)"backpack/backpack.obj");

	myShader.use();
	myShader.setFloat("material.shininess", 32);
	myShader.setInt("material.diffuse", 0);
	myShader.setInt("material.specular", 1);
	myShader.setInt("material.emission", 2);


	// create objects
	std::vector<Object*> objectList = {
		new Cube(myShader, "container2.png", "container2_specular.png", "matrix.jpg"),
		new Cube(myShader, "container2.png", "container2_specular.png", "matrix.jpg"),
		new Cube(myShader, "container2.png", "container2_specular.png", "matrix.jpg"),
		new Cube(myShader, "container2.png", "container2_specular.png", "matrix.jpg"),
		new Cube(myShader, "container2.png", "container2_specular.png", "matrix.jpg"),
		new Cube(myShader, "container2.png", "container2_specular.png", "matrix.jpg"),
		new Cube(myShader, "container2.png", "container2_specular.png", "matrix.jpg"),
		new Cube(myShader, "container2.png", "container2_specular.png", "matrix.jpg"),
		new Cube(myShader, "container2.png", "container2_specular.png", "matrix.jpg"),
		new Cube(myShader, "container2.png", "container2_specular.png", "matrix.jpg"),
	};

	//create lights
	std::vector<Light*> lightList = {
		new SpotLight(lightShader),       // 0: Flashlight
		new PointLight(lightShader),      // 1: White orbiting light
		new DirectionalLight(lightShader),// 2: Main directional light
		new PointLight(lightShader),      // 3: Orange point light
		new PointLight(lightShader),      // 4: Red point light
		new PointLight(lightShader),      // 5: Yellow point light
		new PointLight(lightShader),      // 6: Blue point light
	};

	// create axis
	Axis axis = Axis(axisShader);

	// set object
	for (unsigned int i = 0; i < objectList.size(); i++) {
		//float angle = 20.f * i;
		if (i < 10)
			objectList[i]->setPosition(cubePositions[i]);
	}

	// set light
	lightList[0]->setColor(glm::vec3(1.0f));

	lightList[1]->setCircularMotion(true);
	lightList[1]->setPosition(lightPos);
	lightList[1]->setCircularMotion(true);

	lightList[2]->setDirection(glm::vec3(-0.2f, -1.0f, -0.3f));
	if (lightList[2]->isEnabled())
		lightList[2]->setColor(glm::vec3(1.0f, 1.0f, 1.0f));

	for (int i = 3; i < lightList.size(); i++) {
		if (i < 7) {
			if (lightList[i]->isEnabled())
				lightList[i]->setColor(pointLightColors[i - 3]);
			lightList[i]->setPosition(pointLightPositions[i - 3]);
		}
	}


	// ImGui Settings
	bool controlWindowOpened = true;
	int redValue = 25;
	int greenValue = 25;
	int blueValue = 25;

	PresetScenesCollapse presetScenesCollapse = PresetScenesCollapse("Preset Scenes");
	LightCollapse lightCollapse = LightCollapse("Add Light", lightList, lightShader);
	ObjectCollapse objectCollapse = ObjectCollapse("Add Object", objectList, myShader);
	ObjectProperties objectProperties = ObjectProperties("Object Properties", objectList);
	LightProperties lightProperties = LightProperties("Light Properties", lightList);
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window)) {
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//ImGui::ShowDemoWindow(); // Show demo window! :)

		// ============================================
		// Start Dear ImGui control
		ImVec2 controlWindowPos = ImVec2(WIDTH, 0);
		ImVec2 controlWindowSize = ImVec2(200, HEIGHT);
		ImVec2 viewportSize = ImGui::GetMainViewport()->Size;
		 
		ImGui::SetNextWindowPos(
			ImVec2(viewportSize.x - controlWindowSize.x, 0),
			ImGuiCond_Always
		);
		ImGui::SetNextWindowSize(controlWindowSize);		
		ImGui::Begin("Control Window", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		presetScenesCollapse.show();
		lightCollapse.show();
		objectCollapse.show();
		objectProperties.show();
		lightProperties.show();

		ImGui::Separator();

		if (lightList[0]->isEnabled()) {
			if (ImGui::Button("Hide Flash Light"))
				lightList[0]->disable();
		}
		else {
			if (ImGui::Button("Show Flash Light"))
				lightList[0]->enable();
		}

		if (axis.isEnabled()) {
			if (ImGui::Button("Hide Axis"))
				axis.disable();
		}
		else {
			if (ImGui::Button("Show Axis"))
				axis.enable();
		}

		ImGui::End();

		// End Dear ImGui control
		// ============================================

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

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 horizontalRotate = glm::mat4(1.0f);
		horizontalRotate = glm::rotate(horizontalRotate, glm::radians(horizontalRotateRate), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 verticalRotate = glm::mat4(1.0f);
		verticalRotate = glm::rotate(verticalRotate, glm::radians(verticalRotateRate), glm::vec3(1.0f, 0.0f, 0.0f));

		// light source
		// ------------

		// ===================================================================
		// PER-FRAME LIGHT UPDATES (Only for animated lights)
		// ===================================================================
		lightList[1]->update(currentFrame); // Keep updating the orbiting one

		// draw

		for (int i = 0; i < lightList.size(); i++) {
			if (i == 0) continue;
			if (lightList[i]->isEnabled())
				lightList[i]->draw(view, projection);
		}

		// normal object
		// -------------

		// extract world-space light position and upload to myShader
		int n_DirLight = 0, n_PointLight = 0, n_SpotLight = 0;
		for (int i = 0; i < lightList.size(); i++) {
			if (DirectionalLight* s = dynamic_cast<DirectionalLight*>(lightList[i])) {
				s->updateObjectShader(myShader, n_DirLight++);
			}
			// SpotLight is derived from PointLight -> CHECK THIS FIRST
			else if (SpotLight* s = dynamic_cast<SpotLight*>(lightList[i])) {
				if (i == 0) {
					s->setPosition(camera.Position);
					s->setDirection(camera.Front);
				}
				s->updateObjectShader(myShader, n_SpotLight++);
			}
			else if (PointLight* s = dynamic_cast<PointLight*>(lightList[i])) {
				s->updateObjectShader(myShader, n_PointLight++);
			}
		}

		// draw
		glm::vec3 viewPos = camera.Position;
		for (unsigned int i = 0; i < objectList.size(); i++) {
			//float angle = 20.f * i;
			//cubeList[i].setRotation(glm::vec3(angle * 0.2f, angle * 0.5f, angle * 0.8f));
			objectList[i]->draw(view, projection, viewPos, horizontalRotate, verticalRotate);
		}

		// loaded model
		glm::mat4 model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0, -1.0, 0.0));
		model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
		myShader.setMat4fv("model", model);
		//backpackModel.draw(myShader);

		if (axis.isEnabled())
			axis.draw(view, projection);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	//glDeleteVertexArrays(1, &cubeVAO);
	//glDeleteVertexArrays(1, &lightVAO);
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();

	// ImGui terminate
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

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