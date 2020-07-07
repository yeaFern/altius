#pragma once

#include "Scene.h"

class CreditsScene : public Scene
{
public:
	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float dt) override;
};
