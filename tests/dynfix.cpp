// Copyright (c) [2022-2026] Erdem Kose

#include "tests.hpp"
#include <cmath>
#include "../src/cpp/ffte/subfuncs.hpp"

// ============================================================
// Construction
// ============================================================

TEST(dynfix_test, default_construction) {
    DynFix<double, int32_t> a;
    EXPECT_EQ(a.raw(), 0);
    EXPECT_DOUBLE_EQ(a.toFloat(), 0.0);
}

TEST(dynfix_test, float_construction_positive) {
    DynFix<double, int32_t> a(1.5, 8, 16);
    EXPECT_NEAR(a.toFloat(), 1.5, 1e-4);
}

TEST(dynfix_test, float_construction_negative) {
    DynFix<double, int32_t> a(-2.75, 8, 16);
    EXPECT_NEAR(a.toFloat(), -2.75, 1e-4);
}

TEST(dynfix_test, raw_construction) {
    // Q(8.16): 1.0 is represented as 2^16 = 65536
    DynFix<double, int32_t> a((int32_t)65536, 8, 16);
    EXPECT_NEAR(a.toFloat(), 1.0, 1e-5);
    EXPECT_EQ(a.raw(), 65536);
}

TEST(dynfix_test, format_metadata) {
    DynFix<double, int32_t> a(1.0, 8, 16);
    EXPECT_EQ(a.getM(), 8);
    EXPECT_EQ(a.getN(), 16);
}

// ============================================================
// set()
// ============================================================

TEST(dynfix_test, set_method) {
    DynFix<double, int32_t> a(1.0, 8, 16);
    a.set(3.14);
    EXPECT_NEAR(a.toFloat(), 3.14, 1e-4);
}

TEST(dynfix_test, set_negative) {
    DynFix<double, int32_t> a(1.0, 8, 16);
    a.set(-0.5);
    EXPECT_NEAR(a.toFloat(), -0.5, 1e-5);
}

// ============================================================
// Addition and Subtraction
// ============================================================

TEST(dynfix_test, addition) {
    DynFix<double, int32_t> a(1.5, 8, 16);
    DynFix<double, int32_t> b(2.5, 8, 16);
    EXPECT_NEAR((a + b).toFloat(), 4.0, 1e-4);
}

TEST(dynfix_test, subtraction) {
    DynFix<double, int32_t> a(3.75, 8, 16);
    DynFix<double, int32_t> b(1.25, 8, 16);
    EXPECT_NEAR((a - b).toFloat(), 2.5, 1e-4);
}

TEST(dynfix_test, mixed_sign_addition) {
    DynFix<double, int32_t> a(-1.5, 8, 16);
    DynFix<double, int32_t> b( 2.5, 8, 16);
    EXPECT_NEAR((a + b).toFloat(), 1.0, 1e-4);
}

TEST(dynfix_test, subtraction_to_negative) {
    DynFix<double, int32_t> a(1.0, 8, 16);
    DynFix<double, int32_t> b(3.0, 8, 16);
    EXPECT_NEAR((a - b).toFloat(), -2.0, 1e-4);
}

// ============================================================
// Multiplication
// ============================================================

TEST(dynfix_test, multiply_integers) {
    DynFix<double, int32_t> a(2.0, 8, 16);
    DynFix<double, int32_t> b(3.0, 8, 16);
    EXPECT_NEAR((a * b).toFloat(), 6.0, 1e-3);
}

TEST(dynfix_test, multiply_fractions) {
    DynFix<double, int32_t> a(0.5, 8, 16);
    DynFix<double, int32_t> b(0.5, 8, 16);
    EXPECT_NEAR((a * b).toFloat(), 0.25, 1e-4);
}

TEST(dynfix_test, multiply_mixed_sign) {
    DynFix<double, int32_t> a(-2.0, 8, 16);
    DynFix<double, int32_t> b( 3.0, 8, 16);
    EXPECT_NEAR((a * b).toFloat(), -6.0, 1e-3);
}

TEST(dynfix_test, multiply_by_zero) {
    DynFix<double, int32_t> a(5.0, 8, 16);
    DynFix<double, int32_t> z(0.0, 8, 16);
    EXPECT_NEAR((a * z).toFloat(), 0.0, 1e-5);
}

// ============================================================
// Division
// ============================================================

TEST(dynfix_test, divide_exact) {
    DynFix<double, int32_t> a(6.0, 8, 16);
    DynFix<double, int32_t> b(2.0, 8, 16);
    EXPECT_NEAR((a / b).toFloat(), 3.0, 1e-4);
}

TEST(dynfix_test, divide_fractional_result) {
    DynFix<double, int32_t> a(1.0, 8, 16);
    DynFix<double, int32_t> b(4.0, 8, 16);
    EXPECT_NEAR((a / b).toFloat(), 0.25, 1e-4);
}

TEST(dynfix_test, divide_negative) {
    DynFix<double, int32_t> a(-6.0, 8, 16);
    DynFix<double, int32_t> b( 2.0, 8, 16);
    EXPECT_NEAR((a / b).toFloat(), -3.0, 1e-4);
}

// ============================================================
// Q15 format: int16_t, 1 sign bit + 15 fractional bits
// Typical for DSP: range [-1, 1), resolution ~3e-5
// ============================================================

TEST(dynfix_test, q15_construction) {
    DynFix<float, int16_t> a(0.5f, 1, 15);
    EXPECT_NEAR(a.toFloat(), 0.5f, 1e-4f);
    EXPECT_EQ(a.raw(), 16384);  // 0.5 * 2^15
}

TEST(dynfix_test, q15_add) {
    DynFix<float, int16_t> a(0.5f,  1, 15);
    DynFix<float, int16_t> b(0.25f, 1, 15);
    EXPECT_NEAR((a + b).toFloat(), 0.75f, 1e-4f);
}

TEST(dynfix_test, q15_subtract) {
    DynFix<float, int16_t> a(0.75f, 1, 15);
    DynFix<float, int16_t> b(0.25f, 1, 15);
    EXPECT_NEAR((a - b).toFloat(), 0.5f, 1e-4f);
}

TEST(dynfix_test, q15_multiply) {
    // 0.5 * 0.5 = 0.25
    DynFix<float, int16_t> a(0.5f, 1, 15);
    DynFix<float, int16_t> b(0.5f, 1, 15);
    EXPECT_NEAR((a * b).toFloat(), 0.25f, 2e-4f);
}

TEST(dynfix_test, q15_negative_multiply) {
    DynFix<float, int16_t> a(-0.5f, 1, 15);
    DynFix<float, int16_t> b( 0.5f, 1, 15);
    EXPECT_NEAR((a * b).toFloat(), -0.25f, 2e-4f);
}

// ============================================================
// Q(1.31) format: int32_t, high-precision fractional
// Range [-1, 1), resolution ~4.7e-10
// ============================================================

TEST(dynfix_test, q31_multiply_precision) {
    DynFix<double, int32_t> a(0.5,  1, 31);
    DynFix<double, int32_t> b(0.25, 1, 31);
    EXPECT_NEAR((a * b).toFloat(), 0.125, 1e-7);
}

TEST(dynfix_test, q31_add) {
    DynFix<double, int32_t> a(0.1, 1, 31);
    DynFix<double, int32_t> b(0.2, 1, 31);
    EXPECT_NEAR((a + b).toFloat(), 0.3, 1e-7);
}

// ============================================================
// Precision scales with fractional bit depth
// ============================================================

TEST(dynfix_test, precision_increases_with_n) {
    double val = 0.123456789;
    DynFix<double, int32_t> lo(val,  8,  8);   // ~4e-3 LSB
    DynFix<double, int32_t> hi(val,  8, 16);   // ~1.5e-5 LSB

    double err_lo = std::abs(lo.toFloat() - val);
    double err_hi = std::abs(hi.toFloat() - val);
    EXPECT_GT(err_lo, err_hi);
}
