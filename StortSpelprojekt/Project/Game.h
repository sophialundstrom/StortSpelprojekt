#pragma once
#include "ApplicationState.h"
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
#include "Renderers.h"
#include "DialogueOverlay.h"
#include "InGameOverlay.h"
#include "PauseOverlay.h"
#include "QuadTree.h"
#include "AudioSource.h"

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

    //CameraSettnings
    bool freeCamera = false;
    bool cullFromPlayer = false;
    bool showUI = true;

    //DayNightCycleConfig
    float worldClockTime = 0;
    float dayLength = 300;
    float nightLength = 300;
    float timeSliderVal = 0;
    float fadeTimeMultiplier = 0.25;
    Vector4 lightColor;
    Vector4 DayLightColor = { 1, 1, 1, 1 };
    Vector4 NightLightColor = { 0.5, 0.5, 1, 1 };

    bool hovering = false;
    bool done = false;
    //bool paused = false;
    const std::string file = "Default"; //"Test"

    float lastStateChange = 0.0f;

    //-----TEMP-----//
    Pathfinding pathing;

    SaveStation saveStations[2];

    Terrain terrain;
    Water water;

    Overlay* overlay;

    InGameOverlay* ingameOverlay;
    DialogueOverlay* dialogueOverlay;
    PauseOverlay* pauseOverlay;

    std::shared_ptr<Canvas> currentCanvas;
    std::map<std::string, std::shared_ptr<Canvas>> canvases;

    std::shared_ptr<Player> player;
    std::vector<std::shared_ptr<Arrow>> arrows;
    std::vector<std::shared_ptr<Arrow>> hostileArrows;

    std::vector<std::shared_ptr<Item>> items;

    std::shared_ptr<Building> buildings[3];

    std::map<BarbarianCamp::Location, BarbarianCamp*> camps;

    std::vector<std::shared_ptr<Target>> targets;

    std::vector<std::shared_ptr<FriendlyNPC>> friendlyNPCs;
    
    std::vector<std::shared_ptr<Collider>> colliders;

    std::vector<std::shared_ptr<HostileNPC>> hostiles;
    UINT hostileID = 0;

    std::vector<std::shared_ptr<Loot>> loot;
    UINT lootID = 0;

    std::vector<std::shared_ptr<Biome>> biomes;

    void Update();
    void Render();

    // UI FUNC
    void Pause();
    void Options();
    void HowToPlay();
    void BacktoPause();
    void MainMenu();
    void QuitCanvas();

    std::vector<AudioSource>audioSources;
  
    bool mainMenu = false;

    void RemoveItem(std::shared_ptr<Item> removedItem);
    void AddItem(Item::Type type, Vector3 position, const Vector3& rotation = { 0.0f, 0.0f, 0.0f });
    void GenerateRandomItems(const Vector3& center, UINT amount, float radius);

    std::shared_ptr<FriendlyNPC> AddFriendlyNPC(const std::string& name, const std::string& fileName, Vector3 position);

    void AddFriendlyNPCs();
    void AddLoot(LOOTTYPE type, const Vector3& position);
    void AddTarget(const std::string& file, const Vector3& position, const Vector3& rotation);
    void AddBarbarianCamps();

    void SpawnInvasion();

    void HandleCamera();
    void UpdateAndHandleLoot();
    void CheckNearbyCollision();
    void CheckSaveStationCollision();
    void CheckItemCollision();
    void CheckTargetCollision();
    void CheckQuestInteraction();
    void CheckNearbyEnemies();
    void HandleBiomes();
    void HandleAudioSources();
    void HandleHouseUpgrades();
    bool CheckBuildRequirements(std::shared_ptr<Building> building);
    void HandleCamps();
    void HandleDayNightCycle();

    void SetupAudio();
    void UpdateQuadTree();
    void UpdateInventoryUI();

    void Initialize();
public:
    Game() = delete;
    Game(UINT clientWidth, UINT clientHeight, HWND window);
    ~Game();

    // Inherited via GameState
    virtual APPSTATE Run() override;
};