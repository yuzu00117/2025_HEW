//-----------------------------------------------------------------------------------------------------
// #name scene.h
// #description �V�[���̃w�b�_�[
// #make 2024/12/17�@�@�i��`��
// #update 2024/12/17
// #comment �ǉ��E�C���\��
//          �E�V�[���̃w�b�_�[�@
//          �E�V�[����ǉ����邽�т�SCENE_NAME�ɑ�����
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

//�V�[���̎��
enum SCENE_NAME
{
   SCENE_TITLE,
   SCENE_OP,
   SCENE_STAGE_SELECT,
   SCENE_GAME,
   SCENE_RESULT,
};



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
    // �V�[���̓o�^�}�b�v
    std::unordered_map<int, std::function<std::unique_ptr<Scene>()>> sceneRegistry;

    // ���݂̃V�[��
    std::unique_ptr<Scene> currentScene;

    // �R���X�g���N�^�� private �ɂ��ĊO������̃C���X�^���X����h��
    SceneManager() = default;

public:
    // �V���O���g���C���X�^���X���擾����֐�
    static SceneManager& GetInstance() {
        static SceneManager instance;
        return instance;
    }

    // �R�s�[�R���X�g���N�^�Ƒ�����Z�q�𖳌���
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

    // �V�[���̓o�^
    void RegisterScene(SCENE_NAME scene_name, std::function<std::unique_ptr<Scene>()> factory) {
        sceneRegistry[scene_name] = factory;
    }

    // �V�[���̐؂�ւ�
    void ChangeScene(SCENE_NAME scene_name) {
        if (currentScene) {
            currentScene->Finalize(); // ���݂̃V�[���̏I������
        }

        auto it = sceneRegistry.find(scene_name);
        if (it != sceneRegistry.end()) {
            currentScene = it->second(); // �V�����V�[���̐���
            currentScene->Initialize();  // ����������
        }
        else {
            currentScene = nullptr;
           
        }
    }

    // �X�V����
    void Update() {
        if (currentScene) {
            currentScene->Update();
        }
    }

    // �`�揈��
    void Draw() {
        if (currentScene) {
            currentScene->Draw();
        }
    }
};

#endif // !SCENE_H

