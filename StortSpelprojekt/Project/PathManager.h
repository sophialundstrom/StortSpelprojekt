#include "Pathfinding.h"
#include <queue>

struct PathRequest
{
	Vector3 pathStart;
	Vector3 pathEnd;
	Action<Vector3[], bool> callback;

	public PathRequest(Vector3 _start, Vector3 _end, Action<Vector3[], bool> _callback) {
		pathStart = _start;
		pathEnd = _end;
		callback = _callback;
	}
};

class PathManager
{
public:
	PathManager();
	~PathManager();

private:

};

PathManager::PathManager()
{
}

PathManager::~PathManager()
{
}