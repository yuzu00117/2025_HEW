#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <unordered_map>
#include <functional>
#include <memory>
#include"game.h"

// �V�[�����N���X
class Scene {
public:
    virtual ~Scene() = default;
    // ����������
    virtual void Initialize() {}

    // �X�V����
    virtual void Update() = 0;

    //�`�揈��
    virtual void Draw() = 0;

    // �I������
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

// �V�[���Ǘ��N���X
class SceneManager {
private:
    std::unordered_map<int, std::function<std::unique_ptr<Scene>()>> sceneRegistry;
    std::unique_ptr<Scene> currentScene;

public:
    // �V�[���̓o�^
    void RegisterScene(int id, std::function<std::unique_ptr<Scene>()> factory) {
        sceneRegistry[id] = factory;
    }

    // �V�[���̐؂�ւ�
    void ChangeScene(int id) {
        // ���݂̃V�[���̏I�����������s
        if (currentScene) {
            currentScene->Finalize();
        }

        // �V�����V�[���𐶐�
        auto it = sceneRegistry.find(id);
        if (it != sceneRegistry.end()) {
            currentScene = it->second();
            currentScene->Initialize();
        }
    }

    // �X�V����
    void Update() {
        if (currentScene) {
            currentScene->Update();
        }
    }

    //�`�揈��
    void Draw()
    {
        if (currentScene)
        {
            currentScene->Draw();
        }
    }
};

#endif // !SCENE_H

