//-----------------------------------------------------------------------------------------------------
// #name game.h
// #description �Q�[��
// #make 2024/11/04
// #update 2024/11/03
// #comment �ǉ��E�C���\��
//          �EGame�Ȃ��̊Ǘ����Ă���@��{�I�ɂ��ׂĂ̊֐��������ɂ��ǂ蒅�����Ƃ�
//           
//----------------------------------------------------------------------------------------------------
#ifndef GAME_H
#define GAME_H

class Game
{
public:
	Game();
	~Game();


	static Game& GetInstance() {
		static Game instance;
		return instance;
	}

	//������
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Update();
	void Draw();
	void Uninit();

private:
	
};





#endif // !GAME_H

