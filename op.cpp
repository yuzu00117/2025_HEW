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
	video.Update(URL);//これ一回読んだら動画が終わるまでずっとvideo.cppのUpdateでぐるぐる回る

	//動画終わった後の処理
	SceneManager& sceneManager = SceneManager::GetInstance();
	sceneManager.ChangeScene(SCENE_STAGE_SELECT);
}

void	OP_Scene::Draw()
{

}