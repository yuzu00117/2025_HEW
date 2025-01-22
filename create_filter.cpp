#include"create_filter.h"
#include"include/box2d/box2d.h"

//�t�B���^�[���̃r�b�g�t���O�ŕt������̂Ȃ񂾂��ǁA������ŊǗ��������̂Ł@�ϊ��悤��
//�t�B���^�[��t���邽�߂�
std::unordered_map<std::string, uint16_t> filter_type = {
    {"NULL_filter",             0x0000}, // �����ȃJ�e�S��
    {"Player_filter",           0x0001}, // 1 << 0
    {"ground_filter",           0x0002}, // 1 << 1
    {"enemy_filter",            0x0004}, // 1 << 2
    {"one-way_platform_filter", 0x0008}, // 1 << 3
    {"object_filter",           0x0010}, // 1 << 4
    {"item_filter",             0x0020}, // 1 << 5
    {"Boss_filter",             0x0040}, // 1 << 6
    {"MiniGolem_filter",        0x0080}, // 1 << 7
    {"Shockwave_filter",        0x0100}  // 1 << 8
};


//����͎w�肵���������O����@�i�Ԃ���Ȃ��j
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

//����͎w�肵�����̂����Ԃ���Ȃ�
b2Filter createFilterInclude(const std::string& category, const std::vector<std::string>& includeMasks) {
    b2Filter filter;

    // �J�e�S����ݒ�
    filter.categoryBits = filter_type[category];

    // �}�X�N���������i���Ƃ��Փ˂��Ȃ��悤�ɂ���j
    filter.maskBits = 0;

    // �w�肳�ꂽ�J�e�S���ƏՓˉ\�ɂ���
    for (const std::string& includeMask : includeMasks) {
        filter.maskBits |= filter_type[includeMask];
    }

    return filter;
}

uint16_t GetFilter(const std::string& category)
{
    return filter_type[category];
}
