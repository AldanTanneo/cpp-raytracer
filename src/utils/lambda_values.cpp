#include <vector>

// From src/include
#include <extern/gcem.hpp>
#include <extern/stats.hpp>
#include <utils.hpp>
#include <utils/lambda_values.hpp>

consteval double lambda(const int n, const int i, const double alpha) noexcept {
    double p = 1.0 - alpha / (2.0 * (n - i + 1.0));
    double t = stats::qt(p, n - i - 1.0);
    return (n - i) * t / gcem::sqrt((n - i - 1.0 + t * t) * (n - i + 1.0));
}

class LambdaValues {
private:
    double nine[8];
    double twelve[11];
    double fifteen[14];
    double sixteen[15];
    double twenty[19];
    double twentyfive[24];

public:
    consteval LambdaValues(const double alpha) noexcept {
        for (int i = 0; i < 8; ++i)
            nine[i] = lambda(9, i, alpha);
        for (int i = 0; i < 11; ++i)
            twelve[i] = lambda(12, i, alpha);
        for (int i = 0; i < 14; ++i)
            fifteen[i] = lambda(15, i, alpha);
        for (int i = 0; i < 15; ++i)
            sixteen[i] = lambda(16, i, alpha);
        for (int i = 0; i < 19; ++i)
            twenty[i] = lambda(20, i, alpha);
        for (int i = 0; i < 24; ++i)
            twentyfive[i] = lambda(25, i, alpha);
    }

    constexpr double operator()(const int n, const int i) const noexcept {
        if (i >= n - 1)
            return 0.0;
        switch (n) {
            case 9: return nine[i];
            case 12: return twelve[i];
            case 15: return fifteen[i];
            case 16: return sixteen[i];
            case 20: return twenty[i];
            case 25: return twentyfive[i];
            default: return 0.0;
        }
    }
};

static constexpr double _alpha = 0.05;
static const constinit LambdaValues _lambdas(_alpha);

double lambdas(const int n, const int i) noexcept { return _lambdas(n, i); }
