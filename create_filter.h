//-----------------------------------------------------------------------------------------------------
// #name create_filter.h
// #description �t�B���^�[��t����
// #make 2024/12/04�@�i��`��
// #update 2024/12/04
// #comment �ǉ��E�C���\��
//        
//----------------------------------------------------------------------------------------------------

#ifndef CREATE_FILTER_H
#define CREATE_FILTER_H



#include"include/box2d/box2d.h"
#include <unordered_map>
#include <string>


// �t�B���^�[�̐錾
extern std::unordered_map<std::string, uint16_t> filter_type;


/**
 * @brief �@�t�B���^�[��ǉ�����
 * @param category ���ꎩ�̂̃t�B���^�[
 * @param excludeMasks �@�ݒ肵���t�B���^�[�Ə��O�������t�B���^�[��ݒ�
 * @return �@b2filter�^�ŁA�������g�̃t�B���^�[�Ə��O��񂪓����Ă�
 */
b2Filter createFilterExclude(const std::string& category, const std::vector<std::string>& excludeMasks);

uint16_t GetFilter(const std::string& category);


#endif // !CREATE_FILTER_H