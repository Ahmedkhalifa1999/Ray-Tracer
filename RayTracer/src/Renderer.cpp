#include "Renderer.h"

#include "Walnut/Random.h"

void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if (m_FinalImage)
	{
		//No resize necessary
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height) 
			return;

		m_FinalImage->Resize(width, height);
	}
	else 
	{
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}

	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height];
}

void Renderer::Render()
{
	//Render every pixel
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++) {

			glm::vec2 coord = { x / (float)m_FinalImage->GetWidth(), y / (float)m_FinalImage->GetHeight() };
			coord = (coord * 2.0f) - 1.0f;
			m_ImageData[y * m_FinalImage->GetWidth() + x] = PerPixel(coord);
			
		}
	}

	m_FinalImage->SetData(m_ImageData);
}

uint32_t Renderer::PerPixel(glm::vec2 coord)
{
	//The Cherno Video Version from Video 3
	/*
	glm::vec3 rayOrigin(0.0f, 0.0f, 2.0f);
	glm::vec3 rayDirection(coord.x, coord.y, -1.0f);
	float radius = 0.5f;
	// rayDirection = glm::normalize(rayDirection);


	float a = glm::dot(rayDirection, rayDirection);
	float b = 2.0f * glm::dot(rayOrigin, rayDirection);
	float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;
	float disc = b * b - 4.0f * a * c;

	return (disc >= 0.0f) ? 0xffff00ff : 0xff000000;
	*/

	//My version from Video 2 refactored
	float y_shift = (coord.x * m_FinalImage->GetWidth()) - ((float)m_FinalImage->GetWidth() / 2);
	float z_shift = (coord.y * m_FinalImage->GetHeight()) - ((float)m_FinalImage->GetHeight() / 2);
	float a = (camera_dir.x * camera_dir.x) + (camera_dir.y * camera_dir.y) + (camera_dir.z * camera_dir.z);
	float b = 2 * ((camera_pos.x * camera_dir.x) - (m_Sphere.x * camera_dir.x)
		+ ((camera_pos.y + y_shift) * camera_dir.y) - (m_Sphere.y * camera_dir.y)
		+ ((camera_pos.z + z_shift) * camera_dir.z) - (m_Sphere.z * camera_dir.z));
	float c = (camera_pos.x * camera_pos.x) - (2 * camera_pos.x * m_Sphere.x) + (m_Sphere.x * m_Sphere.x)
		+ ((camera_pos.y + y_shift) * (camera_pos.y + y_shift)) - (2 * (camera_pos.y + y_shift) * m_Sphere.y) + (m_Sphere.y * m_Sphere.y)
		+ ((camera_pos.z + z_shift) * (camera_pos.z + z_shift)) - (2 * (camera_pos.z + z_shift) * m_Sphere.z) + (m_Sphere.z * m_Sphere.z)
		- (m_Sphere.r * m_Sphere.r);
	float disc = (b * b) - (4.0f * a * c);
	if (disc >= 0.0f) {
		return 0xffff00ff;
	}
	else {
		return 0xff000000;
	}
}
