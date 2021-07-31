#include <imgui.h>

#include <Application.hpp>
#include <Layer.hpp>
#include <map>
#include <sstream>
#include <vector>

static Application app;

enum NodeType { NONE = 0, ACTUATOR };

struct Node {
  uint32_t nodeID;
  NodeType type;

  Node() {}
  Node(uint32_t _nodeID, NodeType type)
      : nodeID(_nodeID), type(NodeType::NONE) {}
  Node(uint32_t _nodeID) : Node(_nodeID, NodeType::NONE) {}

  virtual ~Node() = default;

  virtual std::string ToString() {
    std::stringstream ss;
    ss << "NodeID: " << std::to_string(nodeID);

    return ss.str();
  }
};

struct Actuator : public Node {};

class NodeLayer : public Layer {
 public:
  NodeLayer() {
    Node a(0);
    nodes[0] = a;
  };

  void OnUpdate() override {
    ImGui::ShowDemoWindow();

    ImGui::Begin("Nodes");

    if (ImGui::Button("Add Node")) ImGui::OpenPopup("Add Node");

    // ========= Add Node Modal ====== //
    if (ImGui::BeginPopupModal("Add Node", nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize)) {
      int32_t id = 0;
      ImGui::InputInt("NodeID", &id);

      const char *items = {"ACTUATOR \0"};
      int currentItem = 0;
      ImGui::Combo("Node Type", &currentItem, "ACTUATOR\0");

      if (ImGui::Button("Add", ImVec2(120, 0))) {
        // Verify then add the node
        if (nodes.find(id) == nodes.end()) {
          Node n(id, NodeType::ACTUATOR);
          nodes[id] = n;
          ImGui::CloseCurrentPopup();
        } else {
          ImGui::OpenPopup("Error");

          if (ImGui::BeginPopup("Error")) {
            ImGui::Text("Failed to Add Node");
            ImGui::EndPopup();
          }
        }
      }

      ImGui::SetItemDefaultFocus();
      ImGui::SameLine();
      if (ImGui::Button("Cancel", ImVec2(120, 0))) ImGui::CloseCurrentPopup();

      ImGui::EndPopup();
    }

    // ========= Node List box ======= //
    if (ImGui::BeginListBox(
            "", ImVec2(-FLT_MIN,
                       nodes.size() * ImGui::GetTextLineHeightWithSpacing()))) {
      for (int i = 0; i < nodes.size(); i++) {
        const bool isSelected = (currentIndex == i);
        ImGui::Selectable(nodes[i].ToString().c_str(), isSelected);

        if (isSelected) ImGui::SetItemDefaultFocus();
      }

      ImGui::EndListBox();
    }

    ImGui::End();
  }

 private:
  int currentIndex = 0;
  std::map<uint32_t, Node> nodes;
};

int main() {
  app.PushLayer(new NodeLayer());
  app.MainLoop();
}
