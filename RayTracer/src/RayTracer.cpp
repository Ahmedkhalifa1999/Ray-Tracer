#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last Render: %.3fms", m_LastRenderTime);
		if (ImGui::Button("Render")) 
		{
			render();
		}
		ImGui::End();


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("View Port");

		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;

		if (m_Image)
			ImGui::Image(m_Image->GetDescriptorSet(), { (float)m_Image->GetWidth(), (float)m_Image->GetHeight() });

		ImGui::End();
		ImGui::PopStyleVar();

		//render();
	}

	void render() 
	{
		Walnut::Timer timer;
		if (!m_Image || m_ViewportWidth != m_Image->GetWidth() || m_ViewportHeight != m_Image->GetHeight()) 
		{
			m_Image = std::make_shared<Walnut::Image>(m_ViewportWidth, m_ViewportHeight, Walnut::ImageFormat::RGBA);
			delete[] m_ImageData;
			m_ImageData = new uint32_t[m_ViewportHeight * m_ViewportWidth];
		}

		for (uint32_t i = 0; i < m_ViewportHeight; i++) 
		{
			for (uint32_t j = 0; j < m_ViewportWidth; j++) {
				float y_shift = j - ((float)m_ViewportWidth / 2);
				float z_shift = i - ((float)m_ViewportHeight / 2);
				float a = (camera_dir.x * camera_dir.x) + (camera_dir.y * camera_dir.y) + (camera_dir.z * camera_dir.z);
				float b = 2 * ((camera_pos.x * camera_dir.x) - (m_Sphere.x * camera_dir.x)
							 + ((camera_pos.y + y_shift) * camera_dir.y) - (m_Sphere.y * camera_dir.y)
							 + ((camera_pos.z + z_shift) * camera_dir.z) - (m_Sphere.z * camera_dir.z));
				float c = (camera_pos.x * camera_pos.x) - (2 * camera_pos.x * m_Sphere.x) + (m_Sphere.x * m_Sphere.x)
						+ ((camera_pos.y + y_shift) * (camera_pos.y + y_shift)) - (2 * (camera_pos.y + y_shift) * m_Sphere.y) + (m_Sphere.y * m_Sphere.y)
						+ ((camera_pos.z + z_shift) * (camera_pos.z + z_shift)) - (2 * (camera_pos.z + z_shift) * m_Sphere.z) + (m_Sphere.z * m_Sphere.z)
						- (m_Sphere.r * m_Sphere.r);
				float disc = (b * b) - (4 * a * c);
				if (disc >= 0) {
					m_ImageData[i * m_ViewportWidth + j] = Walnut::Random::UInt() | 0xff000000;
				}
				else {
					m_ImageData[i * m_ViewportWidth + j] = 0xff000000;
				}
			}
		}

		m_Image->SetData(m_ImageData);

		m_LastRenderTime = timer.ElapsedMillis();
	}
private:
	std::shared_ptr<Walnut::Image> m_Image;
	uint32_t* m_ImageData = nullptr;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	float m_LastRenderTime = 0;

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

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Ray Tracer";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}