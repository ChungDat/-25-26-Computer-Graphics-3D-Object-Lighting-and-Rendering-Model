#include "ControlCollapse.h"
#include <iostream>

const std::map<std::string, std::string> TEXTURE = {
	{"wood crate", "custom_texture/container2.png"},
};

const std::map < std::string, glm::vec3> MATERIAL = {
	{"gold", glm::vec3(1.0f, 0.71f, 0.29f)},
	{"copper", glm::vec3(0.95, 0.64, 0.54)},
};

ControlCollapse::ControlCollapse()
	: label(nullptr)
{}

ControlCollapse::ControlCollapse(const char* _label)
	: label(_label)
{}

PresetScenesCollapse::PresetScenesCollapse(const char* _label)
	: ControlCollapse(_label)
{}

void PresetScenesCollapse::show() {
	if (ImGui::CollapsingHeader(label ? label : "Preset Scenes")) {
		const char* preview_scene = (currentSelect >= 0) ? scene[currentSelect] : "";
		if (ImGui::BeginCombo("Scenes", preview_scene)) {
			for (int i = 0; i < scene.size(); i++) {
				bool is_selected = (currentSelect == i);
				if (ImGui::Selectable(scene[i], is_selected)) {
					currentSelect = i;
				}

				if (is_selected) ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
	}
}

LightCollapse::LightCollapse(const char* _label, std::vector<Light*>& _lightList, int& _numDirLights, int& _numPointLights, int& _numSpotLights, Shader& _lightShader) 
	: ControlCollapse(_label), lightList(_lightList), numDirLights(_numDirLights), numPointLights(_numPointLights), numSpotLights(_numSpotLights), lightShader(_lightShader) {}

void LightCollapse::show() {
	if (ImGui::CollapsingHeader(label ? label : "Light")) {
		for (int i = 0; i < lightType.size(); i++) {
			if (ImGui::Button(lightType[i])) {
				addLight(lightType[i]);
				printf("Add <%s> \n", lightType[i]);
			}
		}
	}
}

void LightCollapse::addLight(const char* lightType) {
	if (lightType == "Directional Light" && numDirLights < NR_DIR_LIGHTS) {
		lightList.push_back(new DirectionalLight(lightShader));
		numDirLights++;
	}
	else if (lightType == "Point Light" && numPointLights < NR_POINT_LIGHTS) {
		lightList.push_back(new PointLight(lightShader));
		numPointLights++;
	}
	else if (lightType == "Spot Light" && numSpotLights < NR_SPOT_LIGHTS) {
		lightList.push_back(new SpotLight(lightShader));
		numSpotLights++;
	}
}

ObjectCollapse::ObjectCollapse(const char* _label, std::vector<Object*>& _objectList, int& _numObjects, Shader* _shader)
	: ControlCollapse(_label), objectList(_objectList), numObjects(_numObjects), shader(_shader) {}

void ObjectCollapse::show() {
	if (ImGui::CollapsingHeader(label ? label : "Add object")) {
		for (int i = 0; i < object.size(); i++) {
			if (ImGui::Button(object[i])) {
				addObject(object[i]);
				printf("Add <%s>\n", object[i]);
			}
		}
	}
}

void ObjectCollapse::addObject(const char* objectType) {
	if (objectType == "Cube")
		objectList.push_back(new Cube(shader));
	else if (objectType == "Pyramid")
		objectList.push_back(new Pyramid(shader));
	else if (objectType == "Sphere")
		objectList.push_back(new Sphere(shader));
	else if (objectType == "Cylinder")
		objectList.push_back(new Cylinder(shader));
}

ObjectProperties::ObjectProperties(const char* _label, std::vector<Object*>& _objectList, int& _numPbjects) 
	: ControlCollapse(_label), objectList(_objectList), numObjects(_numPbjects) {}

void ObjectProperties::show() {
	if (ImGui::CollapsingHeader(label ? label : "Object Properties")) {
		for (int i = 0; i < objectList.size(); i++) {
			std::string objectTypeStr = objectList[i]->getType() + "##";
			const char* type = objectTypeStr.c_str();

			bool is_selected = (currentSelect == i);
			ImGui::PushID(i);
			if (ImGui::Selectable(type, is_selected)) {
				currentSelect = i;
				printf("Select %s\n", type);
			}
			ImGui::PopID();
		}

		if (currentSelect < objectList.size()) {
			ImGui::SeparatorText("Position");

			float posX = objectList[currentSelect]->getX();
			float posY = objectList[currentSelect]->getY();
			float posZ = objectList[currentSelect]->getZ();

			bool positionChanged = false;
			positionChanged = ImGui::SliderFloat("X##o", &posX, -10.0f, 10.0f, "%.1f") || positionChanged;
			positionChanged = ImGui::SliderFloat("Y##o", &posY, -10.0f, 10.0f, "%.1f") || positionChanged;
			positionChanged = ImGui::SliderFloat("Z##o", &posZ, -10.0f, 10.0f, "%.1f") || positionChanged;
			if (positionChanged) {
				objectList[currentSelect]->setPosition(glm::vec3(posX, posY, posZ));
			}
			
			ImGui::SeparatorText("Physical Attribute");
			
			float roughness = objectList[currentSelect]->getRoughness();
			float metallic = objectList[currentSelect]->getMetallic();

			if (ImGui::SliderFloat("Roughness##o", &roughness, 0.1f, 1.0f, "%.2f"))
			{
				objectList[currentSelect]->setRoughness(roughness);
			}
			if (ImGui::SliderFloat("Metallic##o", &metallic, 0.0f, 1.0f, "%.2f"))
			{
				objectList[currentSelect]->setMetallic(metallic);
			}

			ImGui::SeparatorText("Texture");

			if (true);

			ImGui::SeparatorText("Visibility");

			if (objectList[currentSelect]->isEnabled()) {
				if (ImGui::Button("Hide##o")) {
					objectList[currentSelect]->disable();
				}
			}
			else {
				if (ImGui::Button("Show##o")) {
					objectList[currentSelect]->enable();
				}
			}

			if (ImGui::Button("Remove##o")) {
				delete objectList[currentSelect];
				objectList.erase(objectList.begin() + currentSelect);
			}
		}
	}
}

LightProperties::LightProperties(const char* _label, std::vector<Light*>& _lightList, int& _numDirLights, int& _numPointLights, int& _numSpotLights, Shader*& _objectShader)
	: ControlCollapse(_label), lightList(_lightList), numDirLights(_numDirLights), numPointLights(_numPointLights), numSpotLights(_numSpotLights), objectShader(_objectShader) {}

void LightProperties::updateLightUniforms() {
	objectShader->use();

	// extract world-space light position, attributes and upload to objectShader
	// -------------------------------------------------------------
	int n_DirLight = 0, n_SpotLight = 0, n_PointLight = 0;
	for (int i = 0; i < lightList.size(); i++) {
		if (DirectionalLight* s = dynamic_cast<DirectionalLight*>(lightList[i])) {
			s->updateObjectShader(*objectShader, n_DirLight++);
		}
		// SpotLight is derived from PointLight -> CHECK THIS FIRST
		else if (SpotLight* s = dynamic_cast<SpotLight*>(lightList[i])) {
			s->updateObjectShader(*objectShader, n_SpotLight++);
		}
		else if (PointLight* s = dynamic_cast<PointLight*>(lightList[i])) {
			s->updateObjectShader(*objectShader, n_PointLight++);
		}
	}
	
	numDirLights = n_DirLight;
	numPointLights = n_PointLight;
	numSpotLights = n_SpotLight;
}

void LightProperties::show() {
	if (ImGui::CollapsingHeader(label ? label : "Light Properties")) {
		for (int i = 0; i < lightList.size(); i++) {
			std::string lightTypeStr = lightList[i]->getType() + "Light##";
			const char* type = lightTypeStr.c_str();

			bool is_selected = (currentSelect == i);
			ImGui::PushID(i);
			if (ImGui::Selectable(type, is_selected)) {
				currentSelect = i;
				printf("Select %s\n", type);
				std::cout << "Clicked" << std::endl;
			}
			ImGui::PopID();
		}
		if (currentSelect < lightList.size()) {
			if (lightList[currentSelect]->getType() != "Directional") {
				ImGui::SeparatorText("Position");
				float posX = lightList[currentSelect]->getX();
				float posY = lightList[currentSelect]->getY();
				float posZ = lightList[currentSelect]->getZ();

				bool positionChanged = false;
				positionChanged = ImGui::SliderFloat("X##l", &posX, -10.0f, 10.0f, "%.1f") || positionChanged;
				positionChanged = ImGui::SliderFloat("Y##l", &posY, -10.0f, 10.0f, "%.1f") || positionChanged;
				positionChanged = ImGui::SliderFloat("Z##l", &posZ, -10.0f, 10.0f, "%.1f") || positionChanged;
				if (positionChanged) {
					lightList[currentSelect]->setPosition(glm::vec3(posX, posY, posZ));
				}
			}
			if (lightList[currentSelect]->getType() != "Point") {
				ImGui::SeparatorText("Direction");
				glm::vec3 dir = lightList[currentSelect]->getDirection();
				float dirX = dir.x;
				float dirY = dir.y;
				float dirZ = dir.z;

				bool directionChanged = false;
				directionChanged = ImGui::SliderFloat("X", &dirX, -10.0f, 10.0f, "%.1f") || directionChanged;
				directionChanged = ImGui::SliderFloat("Y", &dirY, -10.0f, 10.0f, "%.1f") || directionChanged;
				directionChanged = ImGui::SliderFloat("Z", &dirZ, -10.0f, 10.0f, "%.1f") || directionChanged;
				if (directionChanged) {
					lightList[currentSelect]->setDirection(glm::vec3(dirX, dirY, dirZ));
				}
			}

			if (lightList[currentSelect]->getType() != "Directional") {
				ImGui::SeparatorText("Orbit");
				float radius = lightList[currentSelect]->getRadius();
				bool directionChanged = false;
				if (ImGui::SliderFloat("Radius", &radius, 0.0, 10.0f, "%.1f"))
					lightList[currentSelect]->setRadius(radius);
			}

			if (lightList[currentSelect]->getType() == "Spot") {
				ImGui::SeparatorText("Cut Off");
				float innerCutOff = lightList[currentSelect]->getInnerCutOff();
				float outerCutOff = lightList[currentSelect]->getOuterCutOff();
				bool cutOffChanged = false;
				cutOffChanged = ImGui::SliderFloat("Inner", &innerCutOff, 0.0f, 15.0f, "%.1f") || cutOffChanged;
				cutOffChanged = ImGui::SliderFloat("Outer", &outerCutOff, innerCutOff, innerCutOff + 10.0f, "%.1f") || cutOffChanged;
				if (cutOffChanged && outerCutOff >= innerCutOff) {
					lightList[currentSelect]->setInnerCutOff(innerCutOff);
					lightList[currentSelect]->setOuterCutOff(outerCutOff);
				}
			}

			ImGui::SeparatorText("Color");
			glm::vec3 _color = lightList[currentSelect]->getColor();
			float colorR = _color.x;
			float colorG = _color.y;
			float colorB = _color.z;
			float col[3] = {colorR, colorG, colorB};

			glm::vec3& lightColorRef = lightList[currentSelect]->getColor_Ref();
			if (lightList[currentSelect]->isEnabled()) {
				if (ImGui::ColorEdit3("##color", col))
				{
					lightList[currentSelect]->setColor(glm::vec3(col[0], col[1], col[2]));
				}
			}

			if (lightList[currentSelect]->isEnabled()) {
				if (ImGui::Button("Hide##l")) {
					lightList[currentSelect]->disable();
				}
			}
			else {
				if (ImGui::Button("Show##l")) {
					lightList[currentSelect]->enable();
				}
			}

			if (ImGui::Button("Remove##l")) {
				//lightList[currentSelect]->setColor(glm::vec3(0.0f));

				if (DirectionalLight* s = dynamic_cast<DirectionalLight*>(lightList[currentSelect])) {
					numDirLights--;
				}
				// SpotLight is derived from PointLight -> CHECK THIS FIRST
				else if (SpotLight* s = dynamic_cast<SpotLight*>(lightList[currentSelect])) {
					numSpotLights--;
				}
				else if (PointLight* s = dynamic_cast<PointLight*>(lightList[currentSelect])) {
					numPointLights--;
				}

				delete lightList[currentSelect];
				lightList.erase(lightList.begin() + currentSelect);

				currentSelect = 0;
			}
		}
	}
}

Settings::Settings(const char* _label, Light* _flashLight, Axis& _axis, Cube& _boxRoom, Shader*& _shader, const std::vector<std::string>& _shaderModel, std::vector<Shader*>& _shaderList, int& rasterizationMode) 
	: ControlCollapse(_label), flashLight(_flashLight), axis(_axis), boxRoom(_boxRoom), objectShader(_shader), shaderModel(_shaderModel), shaderList(_shaderList), currentRasterizationMode(rasterizationMode), currentLighting(0) {}

void Settings::show() {
	if (ImGui::CollapsingHeader(label ? label : "Settings")) {
		// flash light
		if (flashLight->isEnabled()) {
			if (ImGui::Button("Hide Flash Light")) {
				flashLight->disable();
			}
		}
		else {
			if (ImGui::Button("Show Flash Light")) {
				flashLight->enable();
			}
		}

		// axis
		if (axis.isEnabled()) {
			if (ImGui::Button("Hide Axis"))
				axis.disable();
		}
		else {
			if (ImGui::Button("Show Axis"))
				axis.enable();
		}

		// box room
		if (boxRoom.isEnabled()) {
			if (ImGui::Button("Hide Box Room"))
				boxRoom.disable();
		}
		else {
			if (ImGui::Button("Show Box Room"))
				boxRoom.enable();
		}

		// rasterization mode
		ImGui::RadioButton("Fill", &currentRasterizationMode, 0); ImGui::SameLine();
		ImGui::RadioButton("Wireframe", &currentRasterizationMode, 1); ImGui::SameLine();
		ImGui::RadioButton("Point", &currentRasterizationMode, 2);

		// shader mode
		for (int i = 0; i < shaderModel.size(); i++) {
			const char* type = shaderModel[i].c_str();

			bool is_selected = (currentLighting == i);
			ImGui::PushID(i);

			if (ImGui::Selectable(type, is_selected)) {
				if (currentLighting != i) {
					currentLighting = i;
					objectShader = shaderList[currentLighting];
					printf("Select %s\n", type);
				}
			}
			ImGui::PopID();
		}
	}
}