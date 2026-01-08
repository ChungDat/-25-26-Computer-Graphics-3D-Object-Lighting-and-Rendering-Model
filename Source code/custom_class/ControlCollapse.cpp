#include "ControlCollapse.h"
#include <iostream>

// Base constructors (definitions required by the linker)
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

LightCollapse::LightCollapse(const char* _label, std::vector<Light*>& _lightList, Shader& _shader) 
	: ControlCollapse(_label), lightList(_lightList), shader(_shader) {}

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
	if (lightType == "Directional Light")
		lightList.push_back(new DirectionalLight(shader));
	else if (lightType == "Point Light")
		lightList.push_back(new PointLight(shader));
	else if (lightType == "Spot Light")
		lightList.push_back(new SpotLight(shader));
}

ObjectCollapse::ObjectCollapse(const char* _label, std::vector<Object*>& _objectList) 
	: ControlCollapse(_label), objectList(_objectList) {}

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
		objectList.push_back(new Cube("container2.png", "container2_specular.png", "matrix.jpg"));
	else if (objectType == "Pyramid")
		objectList.push_back(new Pyramid("container2.png", "container2_specular.png", "matrix.jpg"));
	else if (objectType == "Sphere")
		objectList.push_back(new Sphere("container2.png", "container2_specular.png", "matrix.jpg"));
	else if (objectType == "Cylinder")
		objectList.push_back(new Cylinder("container2.png", "container2_specular.png", "matrix.jpg"));
}

ObjectProperties::ObjectProperties(const char* _label, std::vector<Object*>& _objectList) 
	: ControlCollapse(_label), objectList(_objectList) {}

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

		//if (objectList.size() > 0 && currentSelect < objectList.size()) {
		if (currentSelect < objectList.size()) {
			ImGui::SeparatorText("Position");

			float posX = objectList[currentSelect]->getX();
			float posY = objectList[currentSelect]->getY();
			float posZ = objectList[currentSelect]->getZ();

			if (ImGui::SliderFloat("X##o", &posX, -10.0f, 10.0f, "%.1f") ||
				ImGui::SliderFloat("Y##o", &posY, -10.0f, 10.0f, "%.1f") ||
				ImGui::SliderFloat("Z##o", &posZ, -10.0f, 10.0f, "%.1f"))
			{
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

LightProperties::LightProperties(const char* _label, std::vector<Light*>& _lightList)
	: ControlCollapse(_label), lightList(_lightList) {}

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
		if (lightList.size() > 0 && currentSelect < lightList.size()) {
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
			else {
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
				if (lightList.size() > 0) {
					delete lightList[currentSelect];
					lightList.erase(lightList.begin() + currentSelect);
					currentSelect = 0;
				}
			}
		}
	}

}

Settings::Settings(Light& _flashLight, Axis& _axis, int& rasterizationMode) : flashLight(_flashLight), axis(_axis), currentRasterizationMode(rasterizationMode) {}

void Settings::show() {
	// flash light
	if (flashLight.isEnabled()) {
		if (ImGui::Button("Hide Flash Light")) {
			flashLight.disable();
		}
	}
	else {
		if (ImGui::Button("Show Flash Light")) {
			flashLight.enable();
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

	// rasterization mode
	ImGui::RadioButton("Fill", &currentRasterizationMode, 0); ImGui::SameLine();
	ImGui::RadioButton("Wireframe", &currentRasterizationMode, 1); ImGui::SameLine();
	ImGui::RadioButton("Point", &currentRasterizationMode, 2);

}