#pragma once

#include "Walnut/Image.h"

#include <memory>
#include <glm/glm.hpp>

class Renderer
{
public:
	Renderer() = default;

	void OnResize(uint32_t width, uint32_t height);
	void Render();

	std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }
private:
	uint32_t PerPixel(glm::vec2 coord);

private:
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;

	typedef struct {
		float x;
		float y;
		float z;
	}pos3D, vec3D;

	typedef struct {
		float x;
		float y;
		float z;
		float r;
	}sphere3D;

	pos3D camera_pos = { 0, 0, 0 };
	vec3D camera_dir = { 1, 0, 0 };

	sphere3D m_Sphere = { 5, 0, 0, 250 };
};