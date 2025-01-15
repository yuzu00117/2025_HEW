//-----------------------------------------------------------------------------------------------------
// #name boss_field_manager.h
// #description boss��Ŕ�������I�u�W�F�N�g�̃t�@�N�g���[
// #make 2025/01/15 �i��`��
// #update 2025/01/15
// #comment �ǉ��E�C���\��
//          �E�I�u�W�F�N�g����邲�Ƃɐ������銴��
//----------------------------------------------------------------------------------------------------


#ifndef BOSS_FIELD_MANAGER_H
#define BOSS_FIELD_MANAGER_H


enum BossFieldObjectType
{
    NULL,

};

class BossFieldManager
{
public:

	//�V���O���g���̃C���X�^���X���擾
	static BossFieldManager& GetInstance();

    // �S�ẴI�u�W�F�N�g��������
    void InitializeAll();

    // �S�ẴI�u�W�F�N�g���X�V
    void UpdateAll();

    // �S�ẴI�u�W�F�N�g��`��
    void DrawAll();

    // �S�ẴI�u�W�F�N�g��j��
    void FinalizeAll();

private:

};



#endif // !BOSS_FIELD_MANAGER_H

