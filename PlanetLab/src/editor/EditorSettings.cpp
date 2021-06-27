#include "EditorSettings.hpp"

namespace PlanetLab
{

EditorSettings::EditorSettings()
{

}

bool EditorSettings::IsWireframeVisible() const
{
    return _showWireframe;
}

void EditorSettings::SetWireframeVisible(bool mode)
{
    _showWireframe = mode;
}

bool& EditorSettings::GetWireframePtr()
{
    return _showWireframe;
}

bool EditorSettings::IsAxisVisible() const
{
    return _showAxis;
}

void EditorSettings::SetAxisVisible(bool mode)
{
    _showAxis = mode;
}

bool& EditorSettings::GetAxisPtr()
{
    return _showAxis;
}

}   // ns editor
