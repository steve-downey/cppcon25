#include <beman/optional/optional.hpp>

#include <cassert>
#include <map>
#include <string>

#include <gtest/gtest.h>

using beman::optional::optional;

optional<int> readConfigValue(std::string name) {
    (void) name;
    return optional<int>{78};
}

int basic_use() {
        int size;
        if (optional<int> s = readConfigValue("Size")) {
                size = *s;
        } else {
                size = 0;
        }
        return size;
}

TEST(SlideTest, BasicUse) {
    EXPECT_EQ(basic_use(), 78);
}

int basic_use26() {
        int size = 0;
        for (int s : readConfigValue("Size")) {
                size = s;
        }
        return size;
}

TEST(SlideTest, BasicUse26) { EXPECT_EQ(basic_use26(), 78); }

constexpr int optParam(int a, optional<int> b = {}) {
        if (b) {
                return a + *b;
        }
        return a;
}

void test3() {
        const auto t1 = optParam(3);
        const auto t2 = optParam(3, 4);
        static_assert(t1 != t2);
}

std::map<std::string, int> map = {{"one", 1}, {"two", 2}};
void test4() {
        auto i = map.find("one");
        auto j = map.find("two");
        auto k = map.find("three");
        assert(i->second == 1);
        assert(j->second == 2);
        assert(k == map.end());
}

optional<int&> findInMap(std::string key) {
        if (map.contains(key)) {
                return map[key];
        }
        return {};
}

TEST(SlideTest, FindInMap) {
    auto i = findInMap("one");
    auto j = findInMap("two");
    auto k = findInMap("three");
    EXPECT_TRUE(*i == 1);
    EXPECT_TRUE(*j == 2);
    EXPECT_TRUE(!k);
    *(findInMap("one")) = 3;
    EXPECT_TRUE(*i == 3);
}
