#include "ControlCollapse.h"
#include <iostream>

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

LightCollapse::LightCollapse(const char* _label, std::vector<Light*>& _lightList, int& _numDirLights, int& _numPointLights, int& _numSpotLights, unsigned int& _depthMapFBO)
	: ControlCollapse(_label), lightList(_lightList), numDirLights(_numDirLights), numPointLights(_numPointLights), numSpotLights(_numSpotLights), depthMapFBO(_depthMapFBO){}

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
		lightList.push_back(new DirectionalLight(depthMapFBO));
		numDirLights++;
	}
	else if (lightType == "Point Light" && numPointLights < NR_POINT_LIGHTS) {
		lightList.push_back(new PointLight(depthMapFBO));
		numPointLights++;
	}
	else if (lightType == "Spot Light" && numSpotLights < NR_SPOT_LIGHTS) {
		lightList.push_back(new SpotLight(depthMapFBO));
		numSpotLights++;
	}
}

ObjectCollapse::ObjectCollapse(const char* _label, std::vector<Object*>& _objectList, int& _numObjects)
	: ControlCollapse(_label), objectList(_objectList), numObjects(_numObjects) {}

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
		objectList.push_back(new Cube());
	else if (objectType == "Pyramid")
		objectList.push_back(new Pyramid());
	else if (objectType == "Sphere")
		objectList.push_back(new Sphere());
	else if (objectType == "Cylinder")
		objectList.push_back(new Cylinder());
	else if (objectType == "Backpack")
		objectList.push_back(new Backpack());
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
			Object* object = objectList[currentSelect];

			ImGui::SeparatorText("Position");

			float posX = object->getX();
			float posY = object->getY();
			float posZ = object->getZ();

			bool positionChanged = false;
			positionChanged = ImGui::SliderFloat("X##o", &posX, -10.0f, 10.0f, "%.1f") || positionChanged;
			positionChanged = ImGui::SliderFloat("Y##o", &posY, -10.0f, 10.0f, "%.1f") || positionChanged;
			positionChanged = ImGui::SliderFloat("Z##o", &posZ, -10.0f, 10.0f, "%.1f") || positionChanged;
			if (positionChanged) {
				object->setPosition(glm::vec3(posX, posY, posZ));
			}

			ImGui::SeparatorText("Rotation");

			float yaw = object->getYaw();
			float pitch = object->getPitch();
			float roll = object->getRoll();

			bool rotationChanged = false;
			rotationChanged = ImGui::SliderFloat("Yaw##o", &yaw, 0.0f, 360.0f, "%.f") || rotationChanged;
			rotationChanged = ImGui::SliderFloat("Pitch##o", &pitch, -89.0f, 89.0f, "%.f") || rotationChanged;
			rotationChanged = ImGui::SliderFloat("Roll##o", &roll, 0.0f, 360.0f, "%.f") || rotationChanged;
			if (rotationChanged) {
				object->setRotation(pitch, yaw, roll);
			}

			ImGui::SeparatorText("Physical Attribute");
			
			float roughness = object->getRoughness();
			float metallic = object->getMetallic();

			if (ImGui::SliderFloat("Roughness##o", &roughness, 0.1f, 1.0f, "%.2f"))
			{
				object->setRoughness(roughness);
			}
			if (ImGui::SliderFloat("Metallic##o", &metallic, 0.0f, 1.0f, "%.2f"))
			{
				object->setMetallic(metallic);
			}

			ImGui::SeparatorText("Texture");

			if (true); // NOT IMPLEMENT

			ImGui::SeparatorText("Visibility");

			if (object->isEnabled()) {
				if (ImGui::Button("Hide##o")) {
					object->disable();
				}
			}
			else {
				if (ImGui::Button("Show##o")) {
					object->enable();
				}
			}

			if (ImGui::Button("Remove##o")) {
				delete object;
				objectList.erase(objectList.begin() + currentSelect);
			}
		}
	}
}

LightProperties::LightProperties(const char* _label, std::vector<Light*>& _lightList, int& _numDirLights, int& _numPointLights, int& _numSpotLights)
	: ControlCollapse(_label), lightList(_lightList), numDirLights(_numDirLights), numPointLights(_numPointLights), numSpotLights(_numSpotLights) {}

void LightProperties::updateLightUniforms(Shader*& objectShader) {
	objectShader->use();

	// extract world-space light position, attributes and upload to objectShader
	// -------------------------------------------------------------
	int n_DirLight = 0, n_SpotLight = 0, n_PointLight = 0;
	for (int i = 0; i < lightList.size(); i++) {
		if (DirectionalLight* s = dynamic_cast<DirectionalLight*>(lightList[i])) {
			s->updateObjectShader(objectShader, n_DirLight++);
		}
		// SpotLight is derived from PointLight -> CHECK THIS FIRST
		else if (SpotLight* s = dynamic_cast<SpotLight*>(lightList[i])) {
			s->updateObjectShader(objectShader, n_SpotLight++);
		}
		else if (PointLight* s = dynamic_cast<PointLight*>(lightList[i])) {
			s->updateObjectShader(objectShader, n_PointLight++);
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
			Light* light = lightList[currentSelect];

			if (light->getType() != "Directional") {

				ImGui::SeparatorText("Position");
				float posX = light->getX();
				float posY = light->getY();
				float posZ = light->getZ();

				bool positionChanged = false;
				positionChanged = ImGui::SliderFloat("X##l", &posX, -10.0f, 10.0f, "%.1f") || positionChanged;
				positionChanged = ImGui::SliderFloat("Y##l", &posY, -10.0f, 10.0f, "%.1f") || positionChanged;
				positionChanged = ImGui::SliderFloat("Z##l", &posZ, -10.0f, 10.0f, "%.1f") || positionChanged;
				if (positionChanged && !light->isOrbital()) {
					light->setPosition(glm::vec3(posX, posY, posZ));
				}
			}
			if (light->getType() != "Point") {
				ImGui::SeparatorText("Direction");
				float pitch = light->getPitch();
				float yaw = light->getYaw();

				bool directionChanged = false;
				directionChanged = ImGui::SliderFloat("pitch", &pitch, -89.0f, 89.0f, "%.f") || directionChanged;
				directionChanged = ImGui::SliderFloat("yaw", &yaw, 0.0f, 360.0f, "%.f") || directionChanged;
				if (directionChanged && !light->isOrbital()) {
					light->setDirection(pitch, yaw);
				}
			}

			if (light->getType() != "Directional") {
				ImGui::SeparatorText("Orbit");
				float radius = light->getRadius();
				bool directionChanged = false;
				if (ImGui::SliderFloat("Radius", &radius, 0.0, 10.0f, "%.1f"))
					light->setRadius(radius);
			}

			if (light->getType() == "Spot") {
				ImGui::SeparatorText("Cut Off");

				SpotLight* s = dynamic_cast<SpotLight*>(light);
				float innerCutOff = s->getInnerCutOff();
				float outerCutOff = s->getOuterCutOff();
				bool cutOffChanged = false;
				cutOffChanged = ImGui::SliderFloat("Inner", &innerCutOff, 0.0f, 15.0f, "%.1f") || cutOffChanged;
				cutOffChanged = ImGui::SliderFloat("Outer", &outerCutOff, innerCutOff, innerCutOff + 10.0f, "%.1f") || cutOffChanged;
				if (cutOffChanged && outerCutOff >= innerCutOff) {
					s->setInnerCutOff(innerCutOff);
					s->setOuterCutOff(outerCutOff);
				}
			}

			ImGui::SeparatorText("Color");
			glm::vec3 _color = light->getColor();
			float colorR = _color.x;
			float colorG = _color.y;
			float colorB = _color.z;
			float col[3] = {colorR, colorG, colorB};

			//glm::vec3& lightColorRef = lightList[currentSelect]->getColor_Ref();
			if (light->isEnabled()) {
				if (ImGui::ColorEdit3("##color", col))
				{
					light->setColor(glm::vec3(col[0], col[1], col[2]));
				}
			}

			if (light->isEnabled()) {
				if (ImGui::Button("Hide##l")) {
					light->disable();
				}
			}
			else {
				if (ImGui::Button("Show##l")) {
					light->enable();
				}
			}

			if (light->isOrbital()) {
				if (ImGui::Button("Disable orbital motion")) {
					light->setOrbital(false);
				}
			}
			else {
				if (ImGui::Button("Enable orbital motion")) {
					light->setOrbital(true);
				}
			}

			if (ImGui::Button("Remove##l")) {
				//lightList[currentSelect]->setColor(glm::vec3(0.0f));

				if (DirectionalLight* s = dynamic_cast<DirectionalLight*>(light)) {
					numDirLights--;
				}
				// SpotLight is derived from PointLight -> CHECK THIS FIRST
				else if (SpotLight* s = dynamic_cast<SpotLight*>(light)) {
					numSpotLights--;
				}
				else if (PointLight* s = dynamic_cast<PointLight*>(light)) {
					numPointLights--;
				}

				delete light;
				lightList.erase(lightList.begin() + currentSelect);

				currentSelect = 0;
			}
		}
	}
}

Settings::Settings(const char* _label, Light* _flashLight, Axis& _axis, Cube& _boxRoom, Surface& _surface, Shader*& _shader, const std::vector<std::string>& _shaderModel, std::vector<Shader*>& _shaderList, int& rasterizationMode) 
	: ControlCollapse(_label), flashLight(_flashLight), axis(_axis), boxRoom(_boxRoom), surface(_surface), objectShader(_shader), shaderModel(_shaderModel), shaderList(_shaderList), currentRasterizationMode(rasterizationMode), currentLighting(0) {}

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

		// surface
		if (surface.isEnabled()) {
			if (ImGui::Button("Hide Surface"))
				surface.disable();
		}
		else {
			if (ImGui::Button("Show Surface"))
				surface.enable();
		}


		// rasterization mode
		ImGui::RadioButton("Fill", &currentRasterizationMode, 0); ImGui::SameLine();
		ImGui::RadioButton("Wireframe", &currentRasterizationMode, 1); ImGui::SameLine();
		ImGui::RadioButton("Point", &currentRasterizationMode, 2);

		// shadow bias
		if (ImGui::SliderFloat("Shadow Bias", &shadowBias, 0.005, 1, "%.3f")) {
			objectShader->setFloat("shadowBias", shadowBias);
		}

		// shader mode
		for (int i = 0; i < shaderModel.size(); i++) {
			const char* type = shaderModel[i].c_str();

			bool is_selected = (currentLighting == i);
			ImGui::PushID(i);

			if (ImGui::Selectable(type, is_selected)) {
				if (currentLighting != i) {
					currentLighting = i;
					objectShader = shaderList[currentLighting];
					objectShader->use();
					printf("Select %s\n", type);
				}
			}
			ImGui::PopID();
		}
	}
}