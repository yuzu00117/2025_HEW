#include"scene.h"
#include"video.h"

Video video;

void	OP_Scene::Initialize()
{

}

void	OP_Scene::Finalize()
{

}

void	OP_Scene::Update()
{
	wchar_t URL[] = L"asset\\video\\simaenaga.mp4";
	video.Update(URL);//������ǂ񂾂瓮�悪�I���܂ł�����video.cpp��Update�ł��邮����

	//����I�������̏���
	SceneManager& sceneManager = SceneManager::GetInstance();
	sceneManager.ChangeScene(SCENE_STAGE_SELECT);
}

void	OP_Scene::Draw()
{

}