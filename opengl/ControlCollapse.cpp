#include "ControlCollapse.h"
#include <iostream>

// Base constructors (definitions required by the linker)
ControlCollapse::ControlCollapse()
	: label(nullptr)
{
}

ControlCollapse::ControlCollapse(const char* _label)
	: label(_label)
{
}

PresetScenesCollapse::PresetScenesCollapse(const char* _label)
	: ControlCollapse(_label)
{
}

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

LightCollapse::LightCollapse(const char* _label) : ControlCollapse(_label) {}

void LightCollapse::show() {
	if (ImGui::CollapsingHeader(label ? label : "Light")) {
		for (int i = 0; i < lightType.size(); i++) {
			if (ImGui::Button(lightType[i])) {
				// addLight(lightType[i]);
				printf("Add %s light\n", lightType[i]);
			}
		}
	}
}

ObjectCollapse::ObjectCollapse(const char* _label) : ControlCollapse(_label) {}

void ObjectCollapse::show() {
	if (ImGui::CollapsingHeader(label ? label : "Add object")) {
		for (int i = 0; i < object.size(); i++) {
			if (ImGui::Button(object[i])) {
				// addObject(object[i]);
				printf("Add %s object\n", object[i]);
			}
		}
	}
}

ObjectProperties::ObjectProperties(const char* _label) : ControlCollapse(_label) {}

void ObjectProperties::show(unsigned int objectID) {

	if (ImGui::CollapsingHeader(label ? label : "Object Properties")) {
		ImGui::SeparatorText("Coordinate");
		//ImGui::SliderFloat("X", )
	}
}