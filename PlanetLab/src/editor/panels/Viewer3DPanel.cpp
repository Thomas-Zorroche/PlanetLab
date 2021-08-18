#include "Viewer3DPanel.hpp"

#include "engine/Renderer.hpp"
#include "editor/Editor.hpp"

namespace PlanetLab
{

using namespace Ceres;
using namespace PlanetLab;

Viewer3DPanel::Viewer3DPanel(std::shared_ptr<Ceres::Planet> planet)
    : _planet(planet)
{
}

void Viewer3DPanel::draw()
{
    if (ImGui::Begin("Renderer"))
    {
        _viewer3DHovered = ImGui::IsWindowHovered();
        displayRenderer();
        displayStatistics();
        displayLoadingWheel();
    }
    ImGui::End();
}

void Viewer3DPanel::displayRenderer()
{
    ImVec2 wsize = ImGui::GetContentRegionAvail();
    if (_viewportWidth != wsize.x || _viewportHeight != wsize.y)
    {
        _viewportWidth = wsize.x;
        _viewportHeight = wsize.y;
        _fbo.resize(_viewportWidth, _viewportHeight);
    }
    Renderer::Get().ComputeProjectionMatrix();

    ImGui::Image((ImTextureID)_fbo.getTextureId(), wsize, ImVec2(0, 1), ImVec2(1, 0));
}

void Viewer3DPanel::displayStatistics()
{
    if (Editor::Get().getEditorSettings()->IsStatsVisible() && _planet->isVisible())
    {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground
            | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
        ImGui::SetNextWindowPos(ImVec2(40, 80));
        ImGui::SetNextWindowSize(ImVec2(150, 100));
        if (ImGui::Begin("Statistics", NULL, window_flags))
        {
            ImGui::Text("Vertices    %s", _statistics.verticesCount.c_str());
            ImGui::Text("Faces        %s", _statistics.facesCount.c_str());
            ImGui::Text("Triangles  %s", _statistics.trianglesCount.c_str());
            ImGui::Text("Fps            %.1f", ImGui::GetIO().Framerate);
        }
        ImGui::End();
    }
}

void Viewer3DPanel::displayLoadingWheel()
{
    if (Application::Get().GetLoading())
    {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground
            | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
        ImGui::SetNextWindowPos(ImVec2(25, _viewportHeight - 50));
        ImGui::SetNextWindowSize(ImVec2(50, 50));
        if (ImGui::Begin("Loading", NULL, window_flags))
        {
            float frame = Application::Get().GetLastFrameDuration();
            ImGui::Image((ImTextureID)_loadingWheel.GetId(), ImVec2(40, 40),
                ImVec2(_loadingWheel.GetUV1().x, _loadingWheel.GetUV1().y),
                ImVec2(_loadingWheel.GetUV2().x, _loadingWheel.GetUV2().y)
            );
            _loadingWheel.NextSprite();
        }
        ImGui::End();
    }
}

void Viewer3DPanel::updateStatistics()
{
    prettyPrintNumber(_planet->getVerticesCount(), _statistics.verticesCount);
    prettyPrintNumber(_planet->getFacesCount(), _statistics.facesCount);
    prettyPrintNumber(_planet->getFacesCount() * 2, _statistics.trianglesCount);
}

// Turn 1451862 into 1 451 862
void prettyPrintNumber(int number, std::string& str)
{
    str = std::to_string(number);
    str.reserve(str.length() + str.length() / 3);

    for (int i = 0, j = 3 - str.length() % 3; i < str.length(); ++i, ++j)
        if (i != 0 && j % 3 == 0)
            str.insert(i++, 1, ' ');
}

} // ns PlanetLab