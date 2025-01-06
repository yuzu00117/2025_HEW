//-----------------------------------------------------------------------------------------------------
// #name tool.h
// #description Float�̌v�Z�����񂽂�ɂ��邽��
// #make 2024/11/02�@�@�i��`��
// #update 2024/11/02
// #comment �ǉ��E�C���\��
//          �Efloat���ȒP�� 
// �@�@�@�@ �E���g�͗������Ă�@�ł�����Ȃނ������Ƃ��ĂȂ��@�g���₷�����Ă邾��
//          
//----------------------------------------------------------------------------------------------------

#include"main.h"
#include"include/box2d/box2d.h"
#include"tool.h"




// ==================================================
// Float2�ɕϊ� [��]
// ==================================================
XMFLOAT2 toFloat2(const XMFLOAT3& f) {
    return XMFLOAT2(f.x, f.y);
}

XMFLOAT2 toFloat2(const XMFLOAT4& f) {
    return XMFLOAT2(f.x, f.y);
}

XMFLOAT2 toFloat2(const XMINT2& f) {
    return XMFLOAT2(static_cast<float>(f.x), static_cast<float>(f.y));
}

XMFLOAT2 toFloat2(const b2Vec2& f) {
    return XMFLOAT2(f.x, f.y);
}

// ==================================================
// Float3�ɕϊ� [��]
// ==================================================
XMFLOAT3 toFloat3(const XMFLOAT2& f) {
    return XMFLOAT3(f.x, f.y, 0.0f);
}

XMFLOAT3 toFloat3(const XMFLOAT4& f) {
    return XMFLOAT3(f.x, f.y, f.z);
}

XMFLOAT3 toFloat3(const XMINT2& f) {
    return XMFLOAT3(static_cast<float>(f.x), static_cast<float>(f.y), 0.0f);
}

XMFLOAT3 toFloat3(const b2Vec2& f) {
    return XMFLOAT3(f.x, f.y, 0.0f);
}

// ==================================================
// Float4�ɕϊ� [��]
// ==================================================
XMFLOAT4 toFloat4(const XMFLOAT2& f) {
    return XMFLOAT4(f.x, f.y, 0.0f, 1.0f);
}

XMFLOAT4 toFloat4(const XMFLOAT3& f) {
    return XMFLOAT4(f.x, f.y, f.z, 1.0f);
}

XMFLOAT4 toFloat4(const XMINT2& f) {
    return XMFLOAT4(static_cast<float>(f.x), static_cast<float>(f.y), 0.0f, 1.0f);
}

XMFLOAT4 toFloat4(const b2Vec2& f) {
    return XMFLOAT4(f.x, f.y, 0.0f, 1.0f);
}


/**
 * @brief ��Βl��Ԃ��֐�
 * @param value ���ꂽ���l
 * @return ��Βl
 */
float ReturnAbsoluteValue(float value)
{
    if (0 < value)//�O���傫�����ߎ󂯎�����l�����̂܂ܕԂ�
    {
        return value;
    }
    else 
    {
        return -value;
    }
}




// �����_���Ȑ����𐶐�����֐�
int GetRandomInt(int min, int max) {
    static std::random_device rd;  // �����_���f�o�C�X�i�V�[�h�����j
    static std::mt19937 gen(rd()); // �����Z���k�E�c�C�X�^����������
    std::uniform_int_distribution<> dist(min, max); // �͈͎w��

    return dist(gen); // �����_���Ȑ����𐶐����ĕԂ�
}

// �x�������W�A���ɕϊ�����֐�
double DegreesToRadians(double degrees) {
    return degrees * M_PI / 180.0;
}


vector<string>
Split(const string& str, const char& word)
{
    istringstream stream(str);

    string field;
    vector<string> res;
    res.reserve(1024);


    while (getline(stream, field, word))
        res.push_back(field);

    return res;
}