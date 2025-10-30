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
			for (int n = 0; n < (int)scene.size(); n++) {
				bool is_selected = (currentSelect == n);
				if (ImGui::Selectable(scene[n], is_selected)) {
					currentSelect = n;
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

ObjectCollapse::ObjectCollapse(const char* _label, std::vector<Object*>& _objectList, Shader& _shader) 
	: ControlCollapse(_label), objectList(_objectList), shader(_shader) {}

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
		objectList.push_back(new Cube(shader, "container2.png", "container2_specular.png", "matrix.jpg"));
	else if (objectType == "Pyramid")
		objectList.push_back(new Pyramid(shader, "container2.png", "container2_specular.png", "matrix.jpg"));
	else if (objectType == "Sphere")
		objectList.push_back(new Sphere(shader, "container2.png", "container2_specular.png", "matrix.jpg"));
	else if (objectType == "Cylinder")
		objectList.push_back(new Cylinder(shader, "container2.png", "container2_specular.png", "matrix.jpg"));
}

ObjectProperties::ObjectProperties(const char* _label) : ControlCollapse(_label) {}

void ObjectProperties::show(unsigned int objectID) {

	if (ImGui::CollapsingHeader(label ? label : "Object Properties")) {
		ImGui::SeparatorText("Coordinate");
		//ImGui::SliderFloat("X", )
	}
}