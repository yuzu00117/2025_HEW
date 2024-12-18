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
	video.Update(URL);//‚±‚êˆê‰ñ“Ç‚ñ‚¾‚ç“®‰æ‚ªI‚í‚é‚Ü‚Å‚¸‚Á‚Ævideo.cpp‚ÌUpdate‚Å‚®‚é‚®‚é‰ñ‚é

	//“®‰æI‚í‚Á‚½Œã‚Ìˆ—
	SceneManager& sceneManager = SceneManager::GetInstance();
	sceneManager.ChangeScene(SCENE_STAGE_SELECT);
}

void	OP_Scene::Draw()
{

}