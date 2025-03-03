//-----------------------------------------------------------------------------------------------------
// #name scene.h
// #description シーンのヘッダー
// #make 2024/12/17　　永野義也
// #update 2024/12/17
// #comment 追加・修正予定
//          ・シーンのヘッダー　
//          ・シーンを追加するたびにSCENE_NAMEに足して
//           
//----------------------------------------------------------------------------------------------------

#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <unordered_map>
#include <functional>
#include <memory>
#include"game.h"
#include"OP.h"
#include"sound.h"
#include"sprite.h"
#include"include/box2d/box2d.h"
#include"world_box2d.h"

#include"clock.h"

//シーンの種類
enum SCENE_NAME
{
   SCENE_TITLE,
   SCENE_OP,
   SCENE_STAGE_SELECT,
   SCENE_GAME,
   SCENE_RESULT,
};


//ステージセレクト
enum STAGE_NAME
{
    STAGE_NULL,
    STAGE_TUTORIAL,
    STAGE_1_1,
    STAGE_ISEKI,
    STAGE_BOSS,
    STAGE_SELECT,

    STAGE_TEST,
};



// シーン基底クラス
class Scene {
public:

   

    virtual ~Scene() = default;
    // 初期化処理
    virtual void Initialize() {}

    // 更新処理
    virtual void Update() = 0;

    //描画処理
    virtual void Draw() = 0;

    // 終了処理
    virtual void Finalize() {}
   
};

class TitleScene : public Scene {
public:
    void Initialize() override;

    void Update() override;

    void Draw()override;

    void Finalize() override;


private:
    float text_sheet_cnt = 0;

    float text_sheet_end_cnt = 0;
 
    float bottom_sheet_cnt = 0;

    bool scene_change_flag=false;
    int scene_change_cnt = 0;

    float black_fade_cnt = 0.0f;

};

class VideoScene : public Scene {
public :
    OP& op = OP::GetInstance();
    void    Initialize() override {
        std::cout << "OP Scene Initialized" << std::endl;
        op.Initialize();
    }

    void    Update() override
    {
        std::cout << "OP Scene Updating" << std::endl;
        op.Update();
    }

    void Draw()override
    {
        std::cout << "OP Scene Drawing" << std::endl;
        op.Draw();
    }

    void Finalize() override {
        std::cout << "OP Scene Finalized" << std::endl;
        op.Finalize();
    }

};

class GameScene : public Scene {
public:

    Game& game = Game::GetInstance();
    void Initialize() override {
        std::cout << "Game Scene Initialized" << std::endl;

        game.Initialize();
    }

    void Update() override {
        std::cout << "Game Scene Updating" << std::endl;
        game.Update();
    }

    void Draw()override
    {
        std::cout << "Game Scene Drawing" << std::endl;
        game.Draw();
    }

    void Finalize() override {
        std::cout << "Game Scene Finalized" << std::endl;
        game.Finalize();
    }
};
class SceneManager {
private:
    // シーンの登録マップ
    std::unordered_map<int, std::function<std::unique_ptr<Scene>()>> sceneRegistry;

    // 現在のシーン
    std::unique_ptr<Scene> currentScene;
    

    STAGE_NAME Stage_name;


    std::unique_ptr<Scene> Change_Request_Scene;

    bool Change_flag=false;

    // コンストラクタを private にして外部からのインスタンス化を防ぐ
    SceneManager() = default;

    Clock clock;
    float deltaTime = 0;

public:
    // シングルトンインスタンスを取得する関数
    static SceneManager& GetInstance() {
        static SceneManager instance;
        return instance;
    }

    // コピーコンストラクタと代入演算子を無効化
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

    float GetDeltaTime() { return deltaTime; }
    void    SetDeltaTime(float time) { deltaTime = time; }

    // シーンの登録
    void RegisterScene(SCENE_NAME scene_name, std::function<std::unique_ptr<Scene>()> factory) {
        sceneRegistry[scene_name] = factory;
    }

    // シーンの切り替え
    void ChangeScene(SCENE_NAME scene_name) {
        
        Box2dWorld& world = Box2dWorld::GetInstance();
        world.SetWorldCallStep(false);
        if (currentScene) {
            currentScene->Finalize(); // 現在のシーンの終了処理
        }

        auto it = sceneRegistry.find(scene_name);
        if (it != sceneRegistry.end()) {
            currentScene = it->second(); // 新しいシーンの生成
            currentScene->Initialize();  // 初期化処理
        }
        else {
            currentScene = nullptr;
           
        }
    }

    void SetStageName(STAGE_NAME name)
    {
        Stage_name = name;
    }

    STAGE_NAME GetStageName(void)
    {
        return Stage_name;
    }


    void Set_Chenge_Scene_flag(bool flag)
    {
        Change_flag = flag;
    }

    bool Get_Chenge_Scene_flag()
    {
        return Change_flag;
    }



    // 更新処理
    void Update() {
        if (currentScene) {
            currentScene->Update();

            deltaTime = clock.elapsed();
        }
    }

    // 描画処理
    void Draw() {
        if (currentScene) {
            currentScene->Draw();
        }
    }
};

#endif // !SCENE_H

