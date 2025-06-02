#include "tests.hpp"

int main(int argc, char **argv) {
    system("CLS");

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}