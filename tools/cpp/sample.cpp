#include <iostream>

int sum(int a, int b) {
    return a+b;
}

#ifdef UT
#include <gtest/gtest.h>
#include <gmock/gmock.h>
TEST(Sum, ShouldAdd) {
    EXPECT_THAT(sum(2,3), 5);
}
TEST(Sum, ShouldAddButThereIsABug) {
    EXPECT_THAT(sum(3,3), 4);
}
#endif

int main(int argc, char** argv) {
#ifdef UT
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
#endif
    int a, b;
    std::cin >> a >> b;
    std::cout << sum(a, b) << std::endl;
    return 0;
}
