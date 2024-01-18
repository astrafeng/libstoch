# LibStoch: C++ Library for Stochastic Optimization in Algo Trading and Market Making

LibStoch provides tools for stochastic optimization in market making and algo trading. It implements DP based on Monte Carlo with regressions, semi Lagrangian for HJB equations and stochastic dual DP to deal with optimal liquidation, LO placement and order management problems. With parallelization from OpenMP, it is also able to solve high dimensional problems on clusters.

# Dependent Libraries
- Boost
- Eigen
- OpenMP