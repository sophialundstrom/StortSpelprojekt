#include "NPCFriendly.h"
#include "Target.h"

class QuestLog
{
private:
	static std::vector<Quest*> quests;
	static std::vector<Quest*> activeQuests;
	static void LoadQuest(Quest* quest);
public:
	static void Update(std::shared_ptr<Player> player, std::map<BarbarianCamp::Location, BarbarianCamp*> camps, std::vector<std::shared_ptr<Target>> targets);
	
	static void Save(const std::string& fileName);
	static void Load(const std::string& fileName);

	static std::vector<Quest*> GetActiveQuests() { return activeQuests; }

	static void CreateQuests();
	static void ShutDown();

	static void Activate(Quest* quest);
	static void Complete(Quest* quest);
	static Quest* Get(const std::string& name);
	static Objective* GetTalkObjective(const std::string& NPC);
};

inline std::vector<Quest*> QuestLog::quests;
inline std::vector<Quest*> QuestLog::activeQuests;