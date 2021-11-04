#pragma once
#include "Model.h"

class QuestMarker : public Model
{
private:

public:
	QuestMarker()
		:Model("QuestMarkerStart", "QM")
	{}

	void Update()
	{
		static float rotation = 0;
		rotation += Time::GetDelta();
		if (rotation >= PI * 2)
			rotation = 0;

		SetRotation(0, rotation, 0);

		Model::Update();
	}
};