#pragma once
#include "Model.h"
#include <fstream>

namespace SettingsLoader
{
	inline void ApplySettings(std::shared_ptr<Model> model, std::string file, std::map<std::string, std::shared_ptr<Drawable>>& drawables)
	{
		std::ifstream reader;

		reader.open("Models/" + file + "/" + "Settings.txt", std::ios::beg);
		if (!reader.is_open())
		{
			Print("NO SETTINGS FILE FOUND FOR: " + file);
			return;
		}

		UINT numCopies = 0;
		std::vector<std::shared_ptr<Model>> models;
		while (!reader.eof())
		{
			std::string str;
			reader >> str;

			if (str == "c") // NUM COPIES
			{
				reader >> numCopies;

				models.push_back(model);
				for (UINT i = 1; i < numCopies; ++i)
					models.push_back(std::make_shared<Model>(*model.get()));
			}

			if (str == "t") // APPEND TEXTURE
			{
				std::string file;
				reader >> file;

				for (UINT i = 0; i < numCopies; ++i)
					models[i]->AddTexture(file);
			}

			if (str == "d") // DISPLACEMENT TEXTURE
			{
				std::string file;
				reader >> file;

				for (UINT i = 0; i < numCopies; ++i)
					models[i]->AddDisplacementTexture(file);
			}

			if (str == "uva") //UV ANIMATION
			{
				bool value;
				reader >> value;

				for (UINT i = 0; i < numCopies; ++i)
					models[i]->HasUVAnimation(value);
			}

			if (numCopies == 1)
			{
				if (str == "p") // POSITION
				{
					Vector3 position;

					reader >> position.x;
					reader >> position.y;
					reader >> position.z;

					models[0]->SetPosition(position);
				}

				if (str == "r") // ROTATION
				{
					Vector3 rotation;

					reader >> rotation.x;
					reader >> rotation.y;
					reader >> rotation.z;

					models[0]->SetRotation(rotation);
				}

				if (str == "s") // SCALING
				{
					Vector3 scale;

					reader >> scale.x;
					reader >> scale.y;
					reader >> scale.z;

					models[0]->SetScale(scale);
				}
			}

			else
			{
				if (str == "p") //POSITION
				{
					int ID;
					reader >> ID;

					Vector3 position;

					reader >> position.x;
					reader >> position.y;
					reader >> position.z;

					models[ID]->SetPosition(position);
				}

				if (str == "r") // ROTATION
				{
					int ID;
					reader >> ID;

					Vector3 rotation;

					reader >> rotation.x;
					reader >> rotation.y;
					reader >> rotation.z;

					models[ID]->SetRotation(rotation);
				}

				if (str == "s") // SCALE
				{
					int ID;
					reader >> ID;

					Vector3 scale;

					reader >> scale.x;
					reader >> scale.y;
					reader >> scale.z;

					models[ID]->SetScale(scale);
				}
			}
		}

		for (UINT i = 0; i < numCopies; ++i)
		{
			models[i]->BindToRenderGraph();
			drawables.emplace(file + std::to_string(i), models[i]);
		}
	}
}