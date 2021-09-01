#include "EditorWindow.h"

enum class EditorType { UNDEFINED = -1, TERRAIN, LEVEL, PARTICLE, MATERIAL };

class Editor : public EditorWindow
{
protected:
	EditorType type = EditorType::UNDEFINED;
public:
	Editor(std::string name)
		:EditorWindow(name) {}

	virtual void Update() = 0;
};