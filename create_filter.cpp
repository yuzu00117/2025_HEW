#include"create_filter.h"
#include"include/box2d/box2d.h"

//�t�B���^�[���̃r�b�g�t���O�ŕt������̂Ȃ񂾂��ǁA������ŊǗ��������̂Ł@�ϊ��悤��
//�t�B���^�[��t���邽�߂�
std::unordered_map<std::string, uint16_t> filter_type = {
    {"NULL_filter",             0x0000}, // �����ȃJ�e�S��
    {"Player_filter",           0x0001}, // 1 << 0
    {"wall_filter",             0x0002}, // 1 << 1
    {"enemy_filter",            0x0004}, // 1 << 2
    {"one-way_platform_filter", 0x0008}, // 1 << 3
    {"object_filter",           0x0010}, // 1 << 4
};

b2Filter createFilterExclude(const std::string& category, const std::vector<std::string>& excludeMasks) {
    b2Filter filter;

    // �J�e�S����ݒ�
    filter.categoryBits = filter_type[category];

    // �}�X�N���������i���ׂẴJ�e�S���ƏՓˉ\�ɂ���j
    filter.maskBits = 0xFFFF;

    // ���O����J�e�S�����폜
    for (const std::string& excludeMask : excludeMasks) {
        filter.maskBits &= ~filter_type[excludeMask];
    }

    return filter;
}