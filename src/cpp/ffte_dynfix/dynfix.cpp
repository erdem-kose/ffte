// Copyright (c) [2022-2026] Erdem Kose

#include "subfuncs.hpp"
#include <cstdint>

// ---------------------------------------------------------------------------
// Internal helpers (file-scope)
// ---------------------------------------------------------------------------

// Minimum number of signed bits (including the sign bit) needed to represent q
// in two's complement.  Equivalently: 64 - (leading redundant sign bits).
//   q =  0 or -1  →  1
//   q =  1        →  2
//   q =  127      →  8
//   q = -128      →  8
static int signed_bits_needed(int64_t q) {
    uint64_t u = (q >= 0) ? (uint64_t)q : (uint64_t)(~q);
    int bits = 1;  // sign bit
    while (u) { u >>= 1; ++bits; }
    return bits;
}

// Align q from n_from fractional bits to n_to fractional bits.
// Right-shifts are always safe.  Left-shifts are capped so the result stays
// within int64_t (i.e. does not set bit 63 unexpectedly).
static int64_t align_n(int64_t q, int n_from, int n_to) {
    int shift = n_to - n_from;
    if (shift == 0) return q;
    if (shift < 0)  return q >> (-shift);
    // Left-shift: limit so total significant bits stay ≤ 62 (leaves sign bit free)
    int max_shift = 62 - (signed_bits_needed(q) - 1);
    if (max_shift <= 0) return q;
    return q << (shift < max_shift ? shift : max_shift);
}

// Pack q64 (currently scaled by 2^n_in) into Ti, choosing n to maximise
// fractional precision while fitting within the available bit width.
// This is the core of the "dynamic" behaviour:
//   - value grew  → radix point moves right  (n decreases)
//   - value shrank → radix point moves left   (n increases)
template <typename Tf, typename Ti>
static DynFix<Tf,Ti> renormalize(int64_t q64, int n_in) {
    constexpr int B = (int)(sizeof(Ti) * 8);

    if (q64 == 0) {
        // Zero: allocate all non-sign bits to the fractional part.
        return DynFix<Tf,Ti>(Ti(0), uint8_t(1), uint8_t(B - 2));
    }

    int nbits = signed_bits_needed(q64);

    // shift > 0 → right-shift q64 to fit in B bits (radix moves right)
    // shift < 0 → left-shift  q64 to fill B bits  (radix moves left)
    int shift = nbits - B;
    int new_n = n_in - shift;

    // Clamp new_n to [0, B-2]: at least 0 fractional bits,
    // at least 1 integer bit above the sign bit.
    if (new_n < 0)   { shift -= new_n;           new_n = 0;     }
    if (new_n > B-2) { shift += (new_n - (B-2)); new_n = B - 2; }

    int64_t new_q;
    if      (shift > 0) new_q = q64 >> shift;
    else if (shift < 0) new_q = q64 << (-shift);
    else                new_q = q64;

    const uint8_t nn = (uint8_t)new_n;
    const uint8_t nm = (uint8_t)(B - 1 - nn);
    return DynFix<Tf,Ti>(static_cast<Ti>(new_q), nm, nn);
}

// ---------------------------------------------------------------------------
// Constructors
// ---------------------------------------------------------------------------

template <typename Tf, typename Ti>
DynFix<Tf,Ti>::DynFix() : Q(0), m(0), n(0) {}

// From raw integer (e.g. already-scaled value from hardware or prior conversion)
template <typename Tf, typename Ti>
DynFix<Tf,Ti>::DynFix(Ti raw, uint8_t m, uint8_t n) : Q(raw), m(m), n(n) {}

// From float: convert val to Q(m.n) and store in the caller-specified format.
// The initial format is respected; dynamic adjustment happens only in operators.
template <typename Tf, typename Ti>
DynFix<Tf,Ti>::DynFix(Tf val, uint8_t m, uint8_t n)
    : Q(static_cast<Ti>(val * static_cast<Tf>(int64_t(1) << n))), m(m), n(n) {}

// ---------------------------------------------------------------------------
// Accessors
// ---------------------------------------------------------------------------

template <typename Tf, typename Ti>
void DynFix<Tf,Ti>::set(Tf val) {
    Q = static_cast<Ti>(val * static_cast<Tf>(int64_t(1) << n));
}

template <typename Tf, typename Ti>
Tf DynFix<Tf,Ti>::toFloat() const {
    return static_cast<Tf>(Q) / static_cast<Tf>(int64_t(1) << n);
}

template <typename Tf, typename Ti>
Ti DynFix<Tf,Ti>::raw() const { return Q; }

template <typename Tf, typename Ti>
uint8_t DynFix<Tf,Ti>::getM() const { return m; }

template <typename Tf, typename Ti>
uint8_t DynFix<Tf,Ti>::getN() const { return n; }

// ---------------------------------------------------------------------------
// Arithmetic operators — all auto-renormalize results
// Operands may have different Q(m.n) formats (different n values after prior ops).
// ---------------------------------------------------------------------------

// Addition: align both operands to the higher-precision format, then renormalize.
template <typename Tf, typename Ti>
DynFix<Tf,Ti> DynFix<Tf,Ti>::operator+(const DynFix<Tf,Ti>& r) const {
    int n_out = ((int)n > (int)r.n) ? (int)n : (int)r.n;
    int64_t q1 = align_n((int64_t)Q,   (int)n,   n_out);
    int64_t q2 = align_n((int64_t)r.Q, (int)r.n, n_out);
    return renormalize<Tf,Ti>(q1 + q2, n_out);
}

// Subtraction: same alignment strategy as addition.
template <typename Tf, typename Ti>
DynFix<Tf,Ti> DynFix<Tf,Ti>::operator-(const DynFix<Tf,Ti>& r) const {
    int n_out = ((int)n > (int)r.n) ? (int)n : (int)r.n;
    int64_t q1 = align_n((int64_t)Q,   (int)n,   n_out);
    int64_t q2 = align_n((int64_t)r.Q, (int)r.n, n_out);
    return renormalize<Tf,Ti>(q1 - q2, n_out);
}

// Multiplication: full integer product is scaled by 2^(n+r.n); renormalize
// from that combined scale.  No manual >> n needed — renormalize handles it.
template <typename Tf, typename Ti>
DynFix<Tf,Ti> DynFix<Tf,Ti>::operator*(const DynFix<Tf,Ti>& r) const {
    int64_t product = (int64_t)Q * (int64_t)r.Q;
    return renormalize<Tf,Ti>(product, (int)n + (int)r.n);
}

// Division: left-shift numerator by as many bits as int64_t safely allows to
// maximise quotient precision, then renormalize from the resulting scale.
template <typename Tf, typename Ti>
DynFix<Tf,Ti> DynFix<Tf,Ti>::operator/(const DynFix<Tf,Ti>& r) const {
    if (r.Q == 0)
        return DynFix<Tf,Ti>(Ti(0), m, n);
    // Boost numerator: shift left until int64_t is nearly full.
    int n_boost = 62 - (signed_bits_needed((int64_t)Q) - 1);
    if (n_boost < 0) n_boost = 0;
    int64_t shifted = (int64_t)Q << n_boost;
    int64_t result  = shifted / (int64_t)r.Q;
    return renormalize<Tf,Ti>(result, (int)n + n_boost - (int)r.n);
}

// ---------------------------------------------------------------------------
// Explicit instantiations
// ---------------------------------------------------------------------------
template class DynFix<double, int64_t>;
template class DynFix<double, int32_t>;
template class DynFix<float,  int32_t>;
template class DynFix<float,  int16_t>;
