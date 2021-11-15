#pragma once
#include "ApplicationState.h"
#include "AnimatedModelRenderer.h"
#include "ModelRenderer.h"
#include "ParticleRenderer.h"
#include "ShadowRenderer.h"
#include "DeferredRenderer.h"
#include "ColliderRenderer.h"
#include "TerrainRenderer.h"
#include "SkeletonRenderer.h"
#include "WaterRenderer.h"
#include "Building.h"
#include "Item.h"
#include "QuestLog.h"
#include "SaveStation.h"
#include "Canvas.h"
#include "Time.h"
#include "Grid.h"
#include "Arrow.h"
#include "Pathfinding.h"
#include "NPCFriendly.h"
#include "Audio.h"
#include "NPCHostile.h"
#include "Loot.h"

enum class GameState { ACTIVE, PAUSED, DIALOGUE };

// The state subclass for the level/game
class Game : public ApplicationState
{
private:
    GameState state = GameState::ACTIVE;

    bool hovering = false;
    bool done = false;
    //bool paused = false;
    const std::string file = "Default"; //"Test"

    //-----TEMP-----//
    Pathfinding pathing;

    std::unique_ptr<QuestLog> questLog;

    AnimatedModelRenderer animatedModelRenderer;
    ParticleRenderer particleRenderer;
    ModelRenderer modelRenderer;
   // ShadowRenderer shadowRenderer;
    TerrainRenderer terrainRenderer;
    ColliderRenderer colliderRenderer;
    SkeletonRenderer skeletonRenderer;
    WaterRenderer waterRenderer;

    SaveStation saveStations[2];

    Terrain terrain;
    Water water;

    std::shared_ptr<Canvas> currentCanvas;
    std::map<std::string, std::shared_ptr<Canvas>> canvases;

    std::shared_ptr<Player> player;
    std::vector<std::shared_ptr<Arrow>> arrows;
    std::vector<std::shared_ptr<Arrow>> hostileArrows;

    std::vector<std::shared_ptr<Item>> items;

    std::shared_ptr<Building> building;

    std::vector<std::shared_ptr<FriendlyNPC>> friendlyNPCs;
    
    std::vector<std::shared_ptr<Collider>> colliders;

    std::vector<std::shared_ptr<HostileNPC>> hostiles;
    UINT hostileID = 0;

    std::vector<std::shared_ptr<Loot>> loot;
    UINT lootID = 0;

    void Update();
    void Render();

    // UI FUNC
    void Pause();
    void Resume();
    void Options();
    void HowToPlay();
    void BacktoPause();
    void MainMenu();

    // LEAVES
    void HoveringResume();
    void HoveringOptions();
    void HoveringHowToPlay();
    void HoveringQuit();
    void HoveringBackHowToPlay();
    void HoveringBackQuit();
    void HoveringBackOptions();
    void HoveringYes();
    void HoveringNo();

  
    bool mainMenu = false;

    void RemoveItem(const std::string name);
    void AddItem(RESOURCE resource, Vector3 position);

    std::shared_ptr<FriendlyNPC> AddFriendlyNPC(const std::string fileName, Vector3 position);

    void AddArrow(const std::string fileName);

    void AddHostileNPC(const std::string& filename, Vector3 position, CombatStyle combatStyle);
    void AddLoot(LOOTTYPE type, const Vector3& position);

    void UpdateAndHandleLoot();
    void CheckNearbyCollision();
    void CheckSaveStationCollision();
    void CheckItemCollision();
    void CheckQuestInteraction();
    void CheckNearbyEnemies();

    void UnbindBuildingEffect(std::unique_ptr<BuildingEffect> effect);
    void UpdateInventoryUI();

    void Initialize();
public:
    Game() = delete;
    Game(UINT clientWidth, UINT clientHeight, HWND window);
    ~Game();

    // Inherited via GameState
    virtual APPSTATE Run() override;
};