#include"create_filter.h"
#include"include/box2d/box2d.h"

//フィルター自体ビットフラグで付けるものなんだけど、文字列で管理したいので　変換ようね
//フィルターを付けるための
std::unordered_map<std::string, uint16_t> filter_type = {
    {"NULL_filter",             0x0000}, // 無効なカテゴリ
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


//これは指定した物を除外する　（ぶつかれない）
b2Filter createFilterExclude(const std::string& category, const std::vector<std::string>& excludeMasks) {
    b2Filter filter;

    // カテゴリを設定
    filter.categoryBits = filter_type[category];

    // マスクを初期化（すべてのカテゴリと衝突可能にする）
    filter.maskBits = 0xFFFF;

    // 除外するカテゴリを削除
    for (const std::string& excludeMask : excludeMasks) {
        filter.maskBits &= ~filter_type[excludeMask];
    }

    return filter;
}

//これは指定したものしかぶつかれない
b2Filter createFilterInclude(const std::string& category, const std::vector<std::string>& includeMasks) {
    b2Filter filter;

    // カテゴリを設定
    filter.categoryBits = filter_type[category];

    // マスクを初期化（何とも衝突しないようにする）
    filter.maskBits = 0;

    // 指定されたカテゴリと衝突可能にする
    for (const std::string& includeMask : includeMasks) {
        filter.maskBits |= filter_type[includeMask];
    }

    return filter;
}

uint16_t GetFilter(const std::string& category)
{
    return filter_type[category];
}
