#include <beman/optional/optional.hpp>

#include <cassert>
#include <map>
#include <string>

#include <gtest/gtest.h>

using beman::optional::optional;

optional<int> readConfigValue(std::string name) {
    (void)name;
    return optional<int>{78};
}

int basic_use() {

    // 898237b6-5a21-439f-94be-b383a439e47e
    int size;
    if (optional<int> s = readConfigValue("Size")) {
        size = *s;
    } else {
        size = 0;
    }
    return size;
    // 898237b6-5a21-439f-94be-b383a439e47e end
}

TEST(SlideTest, BasicUse) { EXPECT_EQ(basic_use(), 78); }

int basic_use26() {

    // 848bce37-c7d0-48c4-ab5d-8f77cc436385
    int size = 0;
    for (int s : readConfigValue("Size")) {
        size = s;
    }
    return size;
    // 848bce37-c7d0-48c4-ab5d-8f77cc436385 end
}

TEST(SlideTest, BasicUse26) { EXPECT_EQ(basic_use26(), 78); }

// f42adfde-5cf2-4c68-8453-83f85ccece63
constexpr int optParam(int a, optional<int> b = {}) {
    if (b) {
        return a + *b;
    }
    return a;
}
// f42adfde-5cf2-4c68-8453-83f85ccece63 end

void test3() {

    // ()e1003632-659b-4d3b-a5a8-2045c1e7a67d
    const auto t1 = optParam(3);
    const auto t2 = optParam(3, 4);
    static_assert(t1 != t2);
    // e1003632-659b-4d3b-a5a8-2045c1e7a67d end
}

std::map<std::string, int> map = {{"one", 1}, {"two", 2}};

void test4() {

    // 31481e6d-8401-4859-92b2-908af1c09fd2
    auto i = map.find("one");
    auto j = map.find("two");
    auto k = map.find("three");
    assert(i->second == 1);
    assert(j->second == 2);
    assert(k == map.end());
    // 31481e6d-8401-4859-92b2-908af1c09fd2 end
}

optional<int&> findInMap(std::string key) {
    if (map.contains(key)) {
        return map[key];
    }
    return {};
}

TEST(SlideTest, FindInMap) {
    // a945e141-bc08-4cf0-89ba-060068ffca00
    optional<int&> i = findInMap("one");
    optional<int&> j = findInMap("two");
    optional<int&> k = findInMap("three");
    EXPECT_TRUE(*i == 1);
    EXPECT_TRUE(*j == 2);
    EXPECT_TRUE(!k);
    *(findInMap("one")) = 3;
    EXPECT_TRUE(*i == 3);
    // a945e141-bc08-4cf0-89ba-060068ffca00 end
}

struct Logger {
    void log(std::string_view log) {}
};


// c089386c-ee92-4d95-87bb-10cf878b9882
void doSomething(std::string const& data,
                 optional<Logger&> logger = {}) {
    for (auto l : logger) {
        l.log(data);
    }
    return;
}
// c089386c-ee92-4d95-87bb-10cf878b9882 end

TEST(SlideTest, LogSomething) {
    Logger logger;
    doSomething("");
    doSomething("log this", logger);
}
