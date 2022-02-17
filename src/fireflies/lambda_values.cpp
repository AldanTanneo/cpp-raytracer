#include <vector>

// From src/include
#include <extern/gcem.hpp>
#include <extern/stats.hpp>
#include <fireflies/lambda_values.hpp>
#include <utils.hpp>

// Compute a lambda value at compile time
consteval const double
    ct_lambda(const int n, const int i, const double alpha) noexcept {
    double p = 1.0 - alpha / (2.0 * (n - i + 1.0));
    double t = stats::qt(p, n - i - 1.0);
    return (n - i) * t / gcem::sqrt((n - i - 1.0 + t * t) * (n - i + 1.0));
}

// Helper class for computing lambda values at compile time
//
// https://www.itl.nist.gov/div898/handbook/eda/section3/eda35h3.htm
class LambdaValues {
private:
    double nine[7];
    double twelve[10];
    double fifteen[13];
    double sixteen[14];
    double twenty[18];
    double twentyfive[23];

public:
    consteval LambdaValues(const double alpha) noexcept {
        for (int i = 0; i < 7; ++i)
            nine[i] = ct_lambda(9, i + 1, alpha);
        for (int i = 0; i < 10; ++i)
            twelve[i] = ct_lambda(12, i + 1, alpha);
        for (int i = 0; i < 13; ++i)
            fifteen[i] = ct_lambda(15, i + 1, alpha);
        for (int i = 0; i < 14; ++i)
            sixteen[i] = ct_lambda(16, i + 1, alpha);
        for (int i = 0; i < 18; ++i)
            twenty[i] = ct_lambda(20, i + 1, alpha);
        for (int i = 0; i < 23; ++i)
            twentyfive[i] = ct_lambda(25, i + 1, alpha);
    }

    constexpr const double operator()(const uint32_t n,
                                      const uint32_t i) const noexcept {
        if (i == 0 || i >= n - 1)
            return 0.0;
        switch (n) {
            case 9: return nine[i - 1];
            case 12: return twelve[i - 1];
            case 15: return fifteen[i - 1];
            case 16: return sixteen[i - 1];
            case 20: return twenty[i - 1];
            case 25: return twentyfive[i - 1];
            default: return 0.0;
        }
    }
};

static constexpr double ALPHA = 0.1;
static const constinit LambdaValues _lambdas(ALPHA);

// Return a compile time computed lambda value
const double lambdas(const int n, const int i) noexcept {
    return _lambdas(n, i);
}
