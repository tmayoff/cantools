#include <imgui.h>

// C++ system headers
#include <filesystem>
#include <fstream>
#include <map>
#include <ostream>
#include <sstream>
#include <vector>

// C++ libraries
#include <nlohmann/json.hpp>

//
#include <Application.hpp>
#include <Layer.hpp>
#include <Nodes/Actuator.hpp>

static Application app;

class ActuatorListLayer : public Layer {
 public:
  ActuatorListLayer() {
    // Get Actuator list from file
    std::filesystem::path f{"cantools.json"};
    if (std::filesystem::exists(f)) {
      std::ifstream i("cantools.json", std::ios::in);
      nlohmann::json j;
      i >> j;
      actuators = j.get<std::map<uint32_t, Actuator>>();
      i.close();
    }
  }

  ~ActuatorListLayer() {
    std::ofstream o("cantools.json", std::ios::out);
    nlohmann::json j = actuators;
    o << j << std::endl;
    o.close();
  }

  void OnUpdate() override {
    ImGui::Begin("Actuators");

    if (ImGui::Button("Add Actuator")) ImGui::OpenPopup("Add Actuator");

    ErrorPopup();
    AddActuator();
    DrawListBox();

    ImGui::End();

    ImGui::Begin("Editor");

    if (actuators.find(currentlySelected) != actuators.end()) {
      Actuator &actuator = actuators[currentlySelected];

      const float controlWidth = 350;

      ImGui::Text("Selected Node:  %s", actuator.ToString().c_str());

      ImGui::Checkbox("Clamp Position to Limits", &clamp);

      float min = clamp?actuator.lowerLimit : -180;
      float max = clamp?actuator.upperLimit : 180;

      ImGui::SetNextItemWidth(controlWidth);
      ImGui::SliderFloat("Position", &actuator.position, min, max);

      ImGui::SetNextItemWidth(controlWidth);
      ImGui::DragFloat("Lower Limit", &actuator.lowerLimit, 1.0f, -180, 0,
                       "%.3f");
      ImGui::SameLine();
      ImGui::SetNextItemWidth(controlWidth);
      ImGui::DragFloat("Upper Limit", &actuator.upperLimit, 1.0f, 0, 180,
                       "%.3f");

      actuator.Update();

      if (ImGui::Button("Set Limits")) actuator.SetLimits();
      ImGui::SameLine();
      if (ImGui::Button("Enable Torque")) actuator.SetTorque(true);
      ImGui::SameLine();
      if (ImGui::Button("Disable Torque")) actuator.SetTorque(false);

      if (ImGui::Button("Delete Actuator")) {
        actuators.erase(actuators.find(currentlySelected));
        currentlySelected = actuators.begin()->first;
      }
    }

    ImGui::End();
  }

  void AddActuator() {
    if (ImGui::BeginPopupModal("Add Actuator", nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize)) {
      ImGui::InputInt("NodeID", &newNodeID);

      if (ImGui::Button("Add", ImVec2(120, 0))) {
        // Verify then add the node
        if (actuators.find(newNodeID) == actuators.end()) {
          Actuator a(newNodeID);
          actuators[newNodeID] = a;
          ImGui::CloseCurrentPopup();
          newNodeID = 0;
        } else {
          ImGui::OpenPopup("AddActuatorError");
        }
      }

      ImGui::SetItemDefaultFocus();
      ImGui::SameLine();
      if (ImGui::Button("Cancel", ImVec2(120, 0))) ImGui::CloseCurrentPopup();

      ImGui::EndPopup();
    }
  }

  void ErrorPopup() {
    bool open = true;
    if (ImGui::BeginPopupModal("AddActuatorError", &open)) {
      ImGui::Text("Failed to Add Node");
      ImGui::EndPopup();
    }
  }

  void DrawListBox() {
    if (ImGui::BeginListBox(
            "", ImVec2(-FLT_MIN, actuators.size() *
                                         ImGui::GetTextLineHeightWithSpacing() +
                                     10))) {
      for (auto [k, v] : actuators) {
        const bool isSelected = (currentlySelected == k);
        if (ImGui::Selectable(v.ToString().c_str(), isSelected)) {
          currentlySelected = k;
        }

        if (isSelected) ImGui::SetItemDefaultFocus();
      }

      ImGui::EndListBox();
    }
  }

 private:
  bool clamp;

  int32_t newNodeID = 0;

  int currentlySelected = 0;
  std::map<uint32_t, Actuator> actuators;
};

int main() {
  app.PushLayer(new ActuatorListLayer());
  app.MainLoop();
}
