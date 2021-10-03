#include "Viewer2DPanel.hpp"

#include "Ceres/ShapeGenerator.hpp"
#include "Ceres/noise/NoiseLayer.hpp"
#include "editor/Parameter.hpp"
#include "editor/Editor.hpp"


namespace PlanetLab
{

Viewer2DPanel::Viewer2DPanel(std::shared_ptr<Ceres::Planet> planet)
	: _planet(planet)
{
	generateTexture();
}

Viewer2DPanel::~Viewer2DPanel()
{
	glDeleteTextures(1, &_textureId);
}

void Viewer2DPanel::draw()
{
	if (ImGui::Begin("Viewer 2D"))
	{
		drawParameter("Texture Size", [this]()
		{
			static int textureSizeId = 0;
			if (ImGui::Combo("##Texture Size", &(int&)textureSizeId, "128\0 256\0\0"))
			{
				switch (textureSizeId)
				{
				case 0:
					_textureSize = ImVec2(128, 128);
					break;
				case 1:
					_textureSize = ImVec2(256, 256);
					break;
				}
				updateTexture();
			}
		}, "Change Texture resolution");

		drawUpdateModeItem();

		const unsigned int& layerId = Editor::Get().getSelectedLayerId();
		if (Editor::Get().hasNoiseLayer())
		{
			const std::string& name = Editor::Get().getLayerSelectedName();
			ImGui::Text(name.c_str());
		}
		else
		{
			ImGui::Text("No Layer Selected");
		}

		ImGui::Text("Zoom x%f", _zoom);

		ImGui::Separator();

		// Image Texture
		ImGuiWindowFlags child_flags = ImGuiWindowFlags_HorizontalScrollbar;
		if (ImGui::BeginChild("Img", ImVec2(0, 0), false, child_flags))
		{
			_viewer2DHovered = ImGui::IsWindowHovered();

			ImGui::SetCursorPosX((ImGui::GetWindowWidth() - (_textureSize.x * _zoom)) * 0.5f);
			ImGui::SetCursorPosY((ImGui::GetWindowHeight() - (_textureSize.y * _zoom)) * 0.5f);
			ImGui::Image(
				Editor::Get().hasNoiseLayer() ? (ImTextureID)_textureId : (ImTextureID)layerId, 
				ImVec2(_textureSize.x * _zoom, _textureSize.y * _zoom), 
				ImVec2(0, 1), 
				ImVec2(1, 0)
			);
		}
		ImGui::EndChild();
	}
	ImGui::End();
}

void Viewer2DPanel::generateTexture()
{
	PLANETLAB_INFO("Creating Viewer2D texture.");

	glGenTextures(1, &_textureId);
	glBindTexture(GL_TEXTURE_2D, _textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _textureSize.x, _textureSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);

	PLANETLAB_INFO("Creating Viewer2D texture: ({}, {})", _textureSize.x, _textureSize.y);
}

void Viewer2DPanel::updateTexture()
{
	if (_updateMode == UpdateMode::OnGenerate && !_readyToGenerate)
		return;
	else
		_readyToGenerate = false;

	if (!_planet->getShapeGenerator()->getNoiseLayer(0))
		return;


	localBuffer.clear();
	auto& noiseLayer = _planet->getShapeGenerator()->getNoiseLayer(Editor::Get().getSelectedLayerId());
	float value = 0;
	for (size_t i = 0; i < _textureSize.x; i++)
	{
		for (size_t j = 0; j < _textureSize.y; j++)
		{
			value = noiseLayer->evaluate(glm::vec3(i * 0.01, j * 0.01, 0));
			value *= 10;
			localBuffer.push_back((unsigned char) (std::clamp(value, 0.0f, 1.0f) * 255));
		}
	}

	PLANETLAB_INFO("Updating Viewer2D texture: ({}, {})", _textureSize.x, _textureSize.y);

	glBindTexture(GL_TEXTURE_2D, _textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _textureSize.x, _textureSize.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, localBuffer.data());
	glBindTexture(GL_TEXTURE_2D, 0);
}


void Viewer2DPanel::drawUpdateModeItem()
{
	drawParameter("Update Mode", [&updateMode = _updateMode]()
	{
		ImGui::Combo("##Update Mode", &(int&)updateMode, "Auto\0On Release\0On Generate\0\0");
	}, "When do viewer2D updates have to be executed:\n * Auto: directly after user changes\n * On Release: after user release click on any parameter\n * On Generate: after click on generate button.");

	if (_updateMode == UpdateMode::OnGenerate)
	{
		drawParameter("", [this]()
			{
				if (ImGui::Button("Generate"))
				{
					_readyToGenerate = true;
					updateTexture();
				}
			});
	}
}


}