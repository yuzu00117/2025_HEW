#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <unordered_map>
#include <functional>
#include <memory>
#include"game.h"

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

class MenuScene : public Scene {
public:
    void Initialize() override {
        std::cout << "Menu Scene Initialized" << std::endl;
    }

    void Update() override {
        std::cout << "Menu Scene Updating" << std::endl;
    }

    void Draw()override
    {
        std::cout << "Menu Scene Drawing" << std::endl;
    }

    void Finalize() override {
        std::cout << "Menu Scene Finalized" << std::endl;
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

// シーン管理クラス
class SceneManager {
private:
    std::unordered_map<int, std::function<std::unique_ptr<Scene>()>> sceneRegistry;
    std::unique_ptr<Scene> currentScene;

public:
    // シーンの登録
    void RegisterScene(int id, std::function<std::unique_ptr<Scene>()> factory) {
        sceneRegistry[id] = factory;
    }

    // シーンの切り替え
    void ChangeScene(int id) {
        // 現在のシーンの終了処理を実行
        if (currentScene) {
            currentScene->Finalize();
        }

        // 新しいシーンを生成
        auto it = sceneRegistry.find(id);
        if (it != sceneRegistry.end()) {
            currentScene = it->second();
            currentScene->Initialize();
        }
    }

    // 更新処理
    void Update() {
        if (currentScene) {
            currentScene->Update();
        }
    }

    //描画処理
    void Draw()
    {
        if (currentScene)
        {
            currentScene->Draw();
        }
    }
};

#endif // !SCENE_H

