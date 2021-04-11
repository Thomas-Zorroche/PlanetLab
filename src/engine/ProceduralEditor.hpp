#pragma once

class ProceduralEditor
{
public:
	ProceduralEditor();
	~ProceduralEditor() = default;

	// Wireframe
	bool IsWireframeVisible() const;
	void SetWireframeVisible(bool mode);
	bool& GetWireframePtr();

	// Axis
	bool IsAxisVisible() const;
	void SetAxisVisible(bool mode);
	bool& GetAxisPtr();

private:
	bool _showWireframe = false;
	bool _showAxis = false;
};