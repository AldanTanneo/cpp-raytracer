#ifndef LAMBDA_VALUES_HPP
#define LAMBDA_VALUES_HPP

// Get compile time computed lambda values at n, i
//
// Only available for n = 25, 20, 15, 16, 12, and 9 (all possible patch sizes)
const double lambdas(const int n, const int i) noexcept;

#endif
