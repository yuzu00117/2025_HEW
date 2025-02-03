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
#include"sound.h"
#include"sprite.h"
#include"include/box2d/box2d.h"

//シーンの種類
enum SCENE_NAME
{
   SCENE_TITLE,
   SCENE_OP,
   SCENE_STAGE_SELECT,
   SCENE_GAME,
   SCENE_RESULT,
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
};


class ResulttScene :public Scene {

public:
    void Initialize() override;

    void Update() override;

    void Draw()override;

    void Finalize() override;

private:
    int total_score_points = 0;
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

    // コンストラクタを private にして外部からのインスタンス化を防ぐ
    SceneManager() = default;

public:
    // シングルトンインスタンスを取得する関数
    static SceneManager& GetInstance() {
        static SceneManager instance;
        return instance;
    }

    // コピーコンストラクタと代入演算子を無効化
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

    // シーンの登録
    void RegisterScene(SCENE_NAME scene_name, std::function<std::unique_ptr<Scene>()> factory) {
        sceneRegistry[scene_name] = factory;
    }

    // シーンの切り替え
    void ChangeScene(SCENE_NAME scene_name) {
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

    // 更新処理
    void Update() {
        if (currentScene) {
            currentScene->Update();
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

