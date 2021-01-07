#include <Core/Window.h>
#include <Core/Input.h>
#include <Core/Time.h>
#include <Core/Camera.h>
#include <Core/Resources.h>

int main(int argc, char* argv[])
{
	if (!Window::GetInstance().Init())
	{
		return -1;
	}

	Camera::GetInstance().Init();

	Resources::GetInstance().ImportShaderFromFile("Resources/Shaders/Standard.json");
	Resources::GetInstance().ImportModelFromFile("Resources/Models/SampleModel.obj");
	Resources::GetInstance().ImportTextureFromFile("Resources/Textures/arm_dif.png");
	Resources::GetInstance().ImportTextureFromFile("Resources/Textures/body_dif.png");
	Resources::GetInstance().ImportTextureFromFile("Resources/Textures/glass_dif.png");
	Resources::GetInstance().ImportTextureFromFile("Resources/Textures/hand_dif.png");
	Resources::GetInstance().ImportTextureFromFile("Resources/Textures/helmet_diff.png");
	Resources::GetInstance().ImportTextureFromFile("Resources/Textures/leg_dif.png");

	std::shared_ptr<Material> testMaterial = std::make_shared<Material>();
	testMaterial->shader = Resources::GetInstance().shaderVector[0];
	testMaterial->name = "Standard";
	testMaterial->SetTexture("DiffuseMap", Resources::GetInstance().textureVector[2]);
	testMaterial->SetVec3("Color", glm::vec3(0.0f, 0.0f, 0.0f));
	Resources::GetInstance().materialVector.push_back(testMaterial);

	while (Window::GetInstance().IsActive())
	{
		Time::Update();
		Input::Update();

		Camera::GetInstance().Update();

		Window::GetInstance().Update();
	}

	return 0;
}
