#include <imgui.h>
#include <unistd.h>

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
#include <CANSocket.hpp>
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

  int32_t ConvertRange(float num) {
    float nAngle = num / 180;
    return nAngle * INT32_MAX;
  }

  void OnUpdate() override {
    // ImGui::ShowDemoWindow();

    ImGui::Begin("Actuators");

    if (ImGui::Button("Add Actuator")) ImGui::OpenPopup("Add Actuator");

    ErrorPopup();
    AddActuator();
    DrawListBox();

    ImGui::End();

    ImGui::Begin("Editor");

    auto actuator = actuators[currentlySelected];

    ImGui::Text("Selected Node:  %s",
                actuators[currentlySelected].ToString().c_str());

    ImGui::SliderFloat("Position", &actuators[currentlySelected].position, -180,
                       180);

    // Update position
    if (actuators[currentlySelected].position !=
        actuators[currentlySelected].lastPosition) {
      // actuators[currentlySelected].SendNewPosition();

      std::vector<uint8_t> data(sizeof(int32_t));
      auto val = ConvertRange(actuators[currentlySelected].position);
      std::memcpy(data.data(), &val, sizeof(int32_t));
      data.emplace(data.begin(), currentlySelected);
      data.emplace(data.begin(), CAN::Intents::MOVE);

      socket.Send(0x1F4, data);
      usleep(10000);

      actuators[currentlySelected].lastPosition =
          actuators[currentlySelected].position;
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
  int32_t newNodeID = 0;

  int currentlySelected = 0;
  std::map<uint32_t, Actuator> actuators;

  CANSocket socket = CANSocket("can0");
};

int main() {
  app.PushLayer(new ActuatorListLayer());
  app.MainLoop();
}
