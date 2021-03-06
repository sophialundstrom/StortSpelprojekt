#pragma once
#include "Model.h"

class QuestMarker : public Model
{
public:
	QuestMarker()
		:Model("ExclamationMark", "QM")
	{
		position = { 0, -1000, 0 };
	}

	void SetAsGive()	 { ApplyMesh("ExclamationMark"); }
	void SetAsHelp()	 { ApplyMesh("QuestionMarkGray"); }
	void SetAsDone()	 { ApplyMesh("QuestionMarkYellow"); }
	void SetAsComplete() { ApplyMesh("ChatBubble"); }
	void SetAsObjective(){ ApplyMesh("ChatBubbleYellow"); }

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