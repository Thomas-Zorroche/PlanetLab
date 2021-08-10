#include "EditorSettings.hpp"
#include "imgui/imgui.h"

namespace PlanetLab
{


void EditorSettings::setDarkThemeMode() const
{
    // NOTE: change colors with ImGui demo windows

    // Setup main colors
    const ImVec4 lightGreen = ImVec4{ 0, 0.8, 0.478, 1.0f };
    const ImVec4 green = ImVec4{ 0.149, 0.509, 0.415, 1.0f };

    const ImVec4 darkPurple = ImVec4{ 0.117, 0.109, 0.137, 1.0f };
    const ImVec4 purple = ImVec4{ 0.196, 0.172, 0.227, 1.0f };
    const ImVec4 lightPurple = ImVec4{ 0.305, 0.262, 0.356, 1.0f };

    auto& colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = darkPurple;

    // Headers
    colors[ImGuiCol_Header] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Buttons
    colors[ImGuiCol_Button] = green;
    colors[ImGuiCol_ButtonHovered] = lightGreen;
    colors[ImGuiCol_ButtonActive] = lightGreen;

    // Frame Components BG
    colors[ImGuiCol_FrameBg] = purple;
    colors[ImGuiCol_FrameBgHovered] = lightPurple;
    colors[ImGuiCol_FrameBgActive] = lightPurple;

    // Tabs
    colors[ImGuiCol_Tab] = green;
    colors[ImGuiCol_TabHovered] = lightGreen;
    colors[ImGuiCol_TabActive] = lightGreen;
    colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Slider
    colors[ImGuiCol_SliderGrab] = green;
    colors[ImGuiCol_SliderGrabActive] = lightGreen;

    // Checkbox
    colors[ImGuiCol_CheckMark] = green;

    // Separator (on resize window)
    colors[ImGuiCol_SeparatorHovered] = green;
    colors[ImGuiCol_SeparatorActive] = lightGreen;
}
}

