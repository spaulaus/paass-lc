/// @file GslFitter.cpp
/// @brief An class providing instructions for GSL fitting routines.
/// @author S. V. Paulauskas
/// @date April 28, 2018
/// @copyright Copyright (c) 2018 S. V. Paulauskas. 
/// @copyright All rights reserved. Released under the Creative Commons Attribution-ShareAlike 4.0 International License
#include "GslFitter.hpp"

#include "TimingConfiguration.hpp"

#include <stdexcept>

using namespace std;

GslFitter::GslFitter() : TimingDriver() {}

GslFitter::~GslFitter() = default;

int GslFitter::GaussianFunction(const gsl_vector *x, void *FitConfiguration, gsl_vector *f) {
    size_t n = ((struct GslFitter::FitConfiguration *) FitConfiguration)->n;
    double *y = ((struct GslFitter::FitConfiguration *) FitConfiguration)->y;
    double *weight = ((struct GslFitter::FitConfiguration *) FitConfiguration)->weight;
    double gamma = ((struct GslFitter::FitConfiguration *) FitConfiguration)->gamma;
    double qdc = ((struct GslFitter::FitConfiguration *) FitConfiguration)->qdc;

    double phi = gsl_vector_get(x, 0);

    for (size_t i = 0; i < n; i++) {
        double t = i;
        double diff = t - phi;
        double Yi = (qdc / (gamma * sqrt(2 * M_PI))) * exp(-diff * diff / (2 * gamma * gamma));
        gsl_vector_set(f, i, (Yi - y[i]) / weight[i]);
    }
    return (GSL_SUCCESS);
}

int GslFitter::GaussianJacobian(const gsl_vector *x, void *FitConfiguration, gsl_matrix *J) {
    size_t n = ((struct GslFitter::FitConfiguration *) FitConfiguration)->n;
    double *weight = ((struct GslFitter::FitConfiguration *) FitConfiguration)->weight;
    double gamma = ((struct GslFitter::FitConfiguration *) FitConfiguration)->gamma;
    double qdc = ((struct GslFitter::FitConfiguration *) FitConfiguration)->qdc;

    double phi = gsl_vector_get(x, 0);
    double dphi;

    for (size_t i = 0; i < n; i++) {
        double t = i;
        double diff = t - phi;
        double s = weight[i];

        dphi = (qdc * diff / (pow(gamma, 3) * sqrt(2 * M_PI))) * exp(-diff * diff / (2 * gamma * gamma));

        gsl_matrix_set(J, i, 0, dphi / s);
    }
    return (GSL_SUCCESS);
}

int GslFitter::GaussianDerivative(const gsl_vector *x, void *FitConfiguration, gsl_vector *f, gsl_matrix *J) {
    GaussianFunction(x, FitConfiguration, f);
    GaussianJacobian(x, FitConfiguration, J);
    return (GSL_SUCCESS);
}

int GslFitter::PmtFunction(const gsl_vector *x, void *FitConfiguration, gsl_vector *f) {
    size_t n = ((struct GslFitter::FitConfiguration *) FitConfiguration)->n;
    double *y = ((struct GslFitter::FitConfiguration *) FitConfiguration)->y;
    double *weight = ((struct GslFitter::FitConfiguration *) FitConfiguration)->weight;
    double beta = ((struct GslFitter::FitConfiguration *) FitConfiguration)->beta;
    double gamma = ((struct GslFitter::FitConfiguration *) FitConfiguration)->gamma;
    double qdc = ((struct GslFitter::FitConfiguration *) FitConfiguration)->qdc;

    double phi = gsl_vector_get(x, 0);
    double alpha = gsl_vector_get(x, 1);

    for (size_t i = 0; i < n; i++) {
        double t = i;
        double diff = t - phi;
        double Yi = 0;

        if (t < phi)
            Yi = 0;
        else
            Yi = qdc * alpha * exp(-beta * diff) * (1 - exp(-pow(gamma * diff, 4.)));

        gsl_vector_set(f, i, (Yi - y[i]) / weight[i]);
    }
    return (GSL_SUCCESS);
}

int GslFitter::PmtJacobian(const gsl_vector *x, void *FitConfiguration, gsl_matrix *J) {
    size_t n = ((struct GslFitter::FitConfiguration *) FitConfiguration)->n;
    double *weight = ((struct GslFitter::FitConfiguration *) FitConfiguration)->weight;
    double beta = ((struct GslFitter::FitConfiguration *) FitConfiguration)->beta;
    double gamma = ((struct GslFitter::FitConfiguration *) FitConfiguration)->gamma;
    double qdc = ((struct GslFitter::FitConfiguration *) FitConfiguration)->qdc;

    double phi = gsl_vector_get(x, 0);
    double alpha = gsl_vector_get(x, 1);

    double dphi, dalpha;

    for (size_t i = 0; i < n; i++) {
        double t = i;
        double diff = t - phi;
        double gaussSq = exp(-pow(gamma * diff, 4.));
        double s = weight[i];
        if (t < phi) {
            dphi = 0;
            dalpha = 0;
        } else {
            dphi = alpha * beta * qdc * exp(-beta * diff) * (1 - gaussSq) -
                   4 * alpha * qdc * pow(diff, 3.) * exp(-beta * diff) *
                   pow(gamma, 4.) * gaussSq;
            dalpha = qdc * exp(-beta * diff) * (1 - gaussSq);
        }
        gsl_matrix_set(J, i, 0, dphi / s);
        gsl_matrix_set(J, i, 1, dalpha / s);
    }
    return (GSL_SUCCESS);
}

int GslFitter::PmtDerivative(const gsl_vector *x, void *FitConfiguration, gsl_vector *f, gsl_matrix *J) {
    PmtFunction(x, FitConfiguration, f);
    PmtJacobian(x, FitConfiguration, J);
    return (GSL_SUCCESS);
}

void GslFitter::InitializePmtFunction(size_t &numParameters, double *initialFitValues,
                                      gsl_multifit_function_fdf &fitFunction) {
    numParameters = 2;
    initialFitValues[0] = 0.0;
    initialFitValues[1] = 2.5;

    fitFunction.f = &PmtFunction;
    fitFunction.df = &PmtJacobian;
#ifdef GSL_VERSION_ONE
    fitFunction.fdf = &PmtDerivative;
#endif
}

void GslFitter::InitializeGaussian(size_t &numParameters, double *initialFitValues,
                                   gsl_multifit_function_fdf &fitFunction, const size_t &numDataPoints) {
    numParameters = 1;
    initialFitValues[0] = (double) numDataPoints * 0.5;

    fitFunction.f = &GaussianFunction;
    fitFunction.df = &GaussianJacobian;
#ifdef GSL_VERSION_ONE
    fitFunction.fdf = &GaussianDerivative;
#endif
}

double GslFitter::CalculatePhase(const std::vector<double> &data, const TimingConfiguration &cfg,
                                 const std::pair<unsigned int, double> &max, const std::pair<double, double> baseline) {
    if (data.empty())
        throw range_error("GslFitter::CalculatePhase - The data vector had a zero size. No data to fit!!");

    static constexpr size_t maxIterations = 100;
    static constexpr double xtol = 1e-4;
    static constexpr double gtol = 1e-4;

    size_t numDataPoints = data.size();
    size_t numParameters = 0;
    int status;
    double initialFitValues[2];
    gsl_multifit_function_fdf fitFunction;

    if (!cfg.IsFastSiPm())
        InitializePmtFunction(numParameters, initialFitValues, fitFunction);
    else
        InitializeGaussian(numParameters, initialFitValues, fitFunction, numDataPoints);

    dof_ = numDataPoints - numParameters;

    static const gsl_multifit_fdfsolver_type *T_ = gsl_multifit_fdfsolver_lmsder;
    gsl_multifit_fdfsolver *solver = gsl_multifit_fdfsolver_alloc(T_, numDataPoints, numParameters);
    gsl_matrix *covarianceMatrix = gsl_matrix_alloc(numParameters, numParameters);

    double *y = new double[numDataPoints];
    double *weights = new double[numDataPoints];
    for (unsigned int i = 0; i < numDataPoints; i++) {
        y[i] = data[i];
        weights[i] = baseline.second;
    }

    struct FitConfiguration fitData = {numDataPoints, y, weights, cfg.GetBeta(), cfg.GetGamma(), cfg.GetQdc()};
    gsl_vector_view x = gsl_vector_view_array(initialFitValues, numParameters);

    fitFunction.n = numDataPoints;
    fitFunction.p = numParameters;
    fitFunction.params = &fitData;

#ifndef GSL_VERSION_ONE
    static constexpr double ftol = 0.0;
    gsl_matrix *jacobian = gsl_matrix_alloc(numDataPoints, numParameters);
    gsl_vector_view gslWeights = gsl_vector_view_array(weights, numDataPoints);

    gsl_multifit_fdfsolver_wset(solver, &fitFunction, &x.vector, &gslWeights.vector);
    gsl_multifit_fdfsolver_driver(solver, maxIterations, xtol, gtol, ftol, &status);
    gsl_multifit_fdfsolver_jac(solver, jacobian);
    gsl_multifit_covar(jacobian, 0.0, covarianceMatrix);

    chi_ = gsl_blas_dnrm2(gsl_multifit_fdfsolver_residual(solver));
#else
    gsl_multifit_fdfsolver_set(solver, &fitFunction, &x.vector);

    for (unsigned int iter = 0; iter < maxIterations; iter++) {
        status = gsl_multifit_fdfsolver_iterate(solver);
        if (status)
            break;
        status = gsl_multifit_test_delta(solver->dx, solver->x, xtol, gtol);
        if (status != GSL_CONTINUE)
            break;
    }
#endif

    double phase = 0.0;
    if (!cfg.IsFastSiPm()) {
        phase = gsl_vector_get(solver->x, 0);
        amp_ = gsl_vector_get(solver->x, 1);
    } else {
        phase = gsl_vector_get(solver->x, 0);
        amp_ = 0.0;
    }

    gsl_multifit_fdfsolver_free(solver);
    gsl_matrix_free(covarianceMatrix);
#ifndef GSL_VERSION_ONE
    gsl_matrix_free(jacobian);
#endif
    delete[] y;
    delete[] weights;

    return phase;
}