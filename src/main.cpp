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
    std::filesystem::path f{"guidebolt.config"};
    if (std::filesystem::exists(f)) {
      std::ifstream i("guidebolt.config", std::ios::in);
      nlohmann::json j;
      i >> j;
      actuators = j.get<std::map<uint32_t, Actuator>>();
      i.close();
    }
  }

  ~ActuatorListLayer() {
    std::ofstream o("guidebolt.config", std::ios::out);
    nlohmann::json j = actuators;
    o << j << std::endl;
    o.close();
  }

  void OnUpdate() override {
    ImGui::ShowDemoWindow();

    ImGui::Begin("Nodes");

    if (ImGui::Button("Add Node")) ImGui::OpenPopup("Add Node");

    ErrorPopup();
    AddNodePopup();
    DrawListBox();

    ImGui::End();
  }

  void AddNodePopup() {
    if (ImGui::BeginPopupModal("Add Node", nullptr,
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
      int index = 0;
      for (auto [k, v] : actuators) {
        const bool isSelected = (currentlySelected == index);
        if (ImGui::Selectable(v.ToString().c_str(), isSelected))
          currentlySelected = index;

        if (isSelected) ImGui::SetItemDefaultFocus();

        index++;
      }

      ImGui::EndListBox();
    }
  }

 private:
  int32_t newNodeID = 0;

  int currentlySelected = 0;
  std::map<uint32_t, Actuator> actuators;
};

int main() {
  app.PushLayer(new ActuatorListLayer());
  app.MainLoop();
}
