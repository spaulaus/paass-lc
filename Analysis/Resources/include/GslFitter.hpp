/// @file GslFitter.hpp
/// @brief Implementation of the GSL fitting routine for GSL v2+
/// @author S. V. Paulauskas
/// @date August 8, 2016
#ifndef PAASS_LC_GSLFITTER_HPP
#define PAASS_LC_GSLFITTER_HPP

#include "TimingDriver.hpp"

#include <gsl/gsl_blas.h>
#include <gsl/gsl_fit.h>
#include <gsl/gsl_multifit_nlin.h>

class TimingConfiguration;

class GslFitter : public TimingDriver {
public:
    ///Default Constructor
    GslFitter();

    ///Default Destructor
    ~GslFitter();

    ///The ever important phase calculation
    /// @param[in] data The baseline subtracted data for the fitting
    /// @param[in] pars The parameters for the fit
    /// @param[in] max : Information about the maximum position and value
    /// @param[in] baseline : The average and standard deviation of the baseline
    double CalculatePhase(const std::vector<double> &data, const TimingConfiguration &cfg,
                          const std::pair<unsigned int, double> &max, const std::pair<double, double> baseline);

    /// @brief Structure that holds information required by the GSL fitting routines to calculate the value of the
    /// function being fit. It's required by GSL so that the signature of the function, jacobian, and derivative
    /// methods are as expected.
    struct FitConfiguration {
        size_t n;//!< The number of data points that we are fitting.
        double *y;//!< The actual data that we are fitting.
        double *weight;//!< The weights for the fit
        double beta; //!< The beta parameter for the fit. We do not use this parameter for Gaussian fits.
        double gamma; //!< The gamma parameter for the fit. This is the only parameter used for Gaussian fits.
        double qdc;//!< The QDC for the fit
    };

private:
    ///Defines the GSL fitting function for standard PMTs
    ///@param [in] x : the vector of gsl starting parameters
    ///@param [in] FitConfiguration : The data to use for the fit
    ///@param [in] f : pointer to the function
    ///@return an integer that GSL does something magical with
    static int PmtFunction(const gsl_vector *x, void *FitConfiguration, gsl_vector *f);

    ///Defines the GSL fitting function for standard PMTs
    ///@param [in] x : the vector of gsl starting parameters
    ///@param [in] FitConfiguration : The data to use for the fit
    ///@param [in] J : pointer to the Jacobian of the function
    ///@return an integer that GSL does something magical with
    static int PmtJacobian(const gsl_vector *x, void *FitConfiguration, gsl_matrix *J);

    ///Defines the GSL fitting function for the analysis
    ///@param [in] x : the vector of gsl starting parameters
    ///@param [in] FitConfiguration : The data to use for the fit
    ///@param [in] f : pointer to the function
    ///@param [in] J : pointer to the Jacobian of the function
    ///@return an integer that GSL does something magical with
    static int PmtDerivative(const gsl_vector *x, void *FitConfiguration, gsl_vector *f, gsl_matrix *J);

    ///Defines the GSL fitting function for the fast output of SiPMTs
    ///@param [in] x : the vector of gsl starting parameters
    ///@param [in] FitConfiguration : The data to use for the fit
    ///@param [in] f : pointer to the function
    ///@return an integer that GSL does something magical with
    static int GaussianFunction(const gsl_vector *x, void *FitConfiguration, gsl_vector *f);

    /// Defines the GSL fitting function for the fast output SiPMTs
    ///@param [in] x : the vector of gsl starting parameters
    ///@param [in] FitConfiguration : The data to use for the fit
    ///@param [in] J : pointer to the Jacobian of the function
    ///@return an integer that GSL does something magical with
    static int GaussianJacobian(const gsl_vector *x, void *FitConfiguration, gsl_matrix *J);

    ///Defines the GSL fitting function for the fast output of SiPMTs
    ///@param [in] x : the vector of gsl starting parameters
    ///@param [in] FitConfiguration : The data to use for the fit
    ///@param [in] f : pointer to the function
    ///@param [in] J : pointer to the Jacobian of the function
    ///@return an integer that GSL does something magical with
    static int GaussianDerivative(const gsl_vector *x, void *FitConfiguration, gsl_vector *f, gsl_matrix *J);

    ///Method that initializes the PmtFunction for fitting
    ///@param[in] numParameters : The number of parameters that are used in the fit. This function always has 2 :
    /// beta and gamma.
    ///@param[in] initialParameterValues : The starting values for the fitted parameters.
    ///@param[in] fitFunction : The fitting function that we're going to use.
    void InitializePmtFunction(size_t &numParameters, double *initialParameterValues,
                               gsl_multifit_function_fdf &fitFunction);

    ///Method that initializes a Gaussian for fitting
    ///@param[in] numParameters : The number of parameters that are used in the fit. This function always has 1 : gamma.
    ///@param[in] initialParameterValues : The starting values for the fitted parameters.
    ///@param[in] fitFunction : The fitting function that we're going to use.
    ///@param[in] numDataPoints : We try to center the Gaussian in the waveform, so we use the number of data points
    /// to approximate where the Max of the Gaussian falls in the waveform
    void InitializeGaussian(size_t &numParameters, double *initialParameterValues,
                            gsl_multifit_function_fdf &fitFunction, const size_t &numDataPoints);

    double amp_; //!< The amplitude calculated by the fit
    double chi_; //!< The chi calculated from the fit
    double dof_; //!< The degrees of freedom in the fit.
};
#endif //PAASS_LC_GSLFITTER_HPP