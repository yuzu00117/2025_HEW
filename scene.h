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
#include"OP.h"
#include"sound.h"
#include"sprite.h"
#include"include/box2d/box2d.h"
#include"world_box2d.h"

#include"clock.h"

//�V�[���̎��
enum SCENE_NAME
{
   SCENE_TITLE,
   SCENE_OP,
   SCENE_STAGE_SELECT,
   SCENE_GAME,
   SCENE_RESULT,
};


//�X�e�[�W�Z���N�g
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
    // �V�[���̓o�^�}�b�v
    std::unordered_map<int, std::function<std::unique_ptr<Scene>()>> sceneRegistry;

    // ���݂̃V�[��
    std::unique_ptr<Scene> currentScene;
    

    STAGE_NAME Stage_name;


    std::unique_ptr<Scene> Change_Request_Scene;

    bool Change_flag=false;

    // �R���X�g���N�^�� private �ɂ��ĊO������̃C���X�^���X����h��
    SceneManager() = default;

    Clock clock;
    float deltaTime = 0;

public:
    // �V���O���g���C���X�^���X���擾����֐�
    static SceneManager& GetInstance() {
        static SceneManager instance;
        return instance;
    }

    // �R�s�[�R���X�g���N�^�Ƒ�����Z�q�𖳌���
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

    float GetDeltaTime() { return deltaTime; }
    void    SetDeltaTime(float time) { deltaTime = time; }

    // �V�[���̓o�^
    void RegisterScene(SCENE_NAME scene_name, std::function<std::unique_ptr<Scene>()> factory) {
        sceneRegistry[scene_name] = factory;
    }

    // �V�[���̐؂�ւ�
    void ChangeScene(SCENE_NAME scene_name) {
        
        Box2dWorld& world = Box2dWorld::GetInstance();
        world.SetWorldCallStep(false);
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



    // �X�V����
    void Update() {
        if (currentScene) {
            currentScene->Update();

            deltaTime = clock.elapsed();
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

