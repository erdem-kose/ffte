// Copyright (c) [2022-2026] Erdem Kose

#include "tests.hpp"
#include <cstdio>

class ResultPrinter : public ::testing::EmptyTestEventListener {
    void OnTestEnd(const ::testing::TestInfo& info) override {
        printf("  --> %s.%s: %s\n",
            info.test_suite_name(),
            info.name(),
            info.result()->Passed() ? "PASSED" : "FAILED");
    }
};

int main(int argc, char **argv) {
    system("CLS");

    ::testing::InitGoogleTest(&argc, argv);

    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();
    listeners.Append(new ResultPrinter());

    return RUN_ALL_TESTS();
}