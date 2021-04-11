#include "ProceduralEditor.hpp"


ProceduralEditor::ProceduralEditor()
{

}

bool ProceduralEditor::IsWireframeVisible() const
{
    return _showWireframe;
}

void ProceduralEditor::SetWireframeVisible(bool mode)
{
    _showWireframe = mode;
}

bool& ProceduralEditor::GetWireframePtr()
{
    return _showWireframe;
}

bool ProceduralEditor::IsAxisVisible() const
{
    return _showAxis;
}

void ProceduralEditor::SetAxisVisible(bool mode)
{
    _showAxis = mode;
}

bool& ProceduralEditor::GetAxisPtr()
{
    return _showAxis;
}
