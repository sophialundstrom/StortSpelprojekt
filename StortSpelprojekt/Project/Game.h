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
#include "MainMenu.h"
#include "QuadTree.h"

enum class GameState { ACTIVE, PAUSED, DIALOGUE };

// The state subclass for the level/game
class Game : public ApplicationState
{
private:
    GameState state = GameState::ACTIVE;

    //QuadTreeStuff stuff
    QuadTree* quadTree;
    FrustrumCollider frustrumCollider;
    OrthographicCollider orthographicCollider;
    std::map<std::string, std::shared_ptr<Drawable>> drawablesToBeRendered;
    std::map<std::string, std::shared_ptr<Drawable>> noCullingDrawables;
    /*
    The following 3 variables: useQuadTreeCulling, updateFrustrum and cullingProfile
    are intended only be used for debugging. Relecvant code that they are used for are currently uncommented
    in a attempt to get rid of the gamespeed x2 glitch.
    */
    bool useQuadTreeCulling = true;
    bool updateFrustrum = true;
    int cullingProfile = 0;

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
    ModelRenderer staticMeshModelRender;
    ShadowRenderer shadowRenderer;

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
    void QuitCanvas();

    // LEAVES
    void HoveringResume();
    void HoveringOptions();
    void HoveringHowToPlay();
    void HoveringMainMenu();
    void HoveringBackHowToPlay();
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
    void UpdateQuadTree();

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