//
// Created by Marco Paggiaro on 02/01/2021.
//

#ifndef EX1_EUROPEANOPTION_H
#define EX1_EUROPEANOPTION_H

#include <string>
#include <utility>
#include <vector>
#include <complex>

class EuropeanOption {
private:
    void copy(const EuropeanOption& o2); // need to check if I can do this with inheritance, see book.

public:
// Public member data for convenience only
    static double tau_bar;
    double q, kappa{}, theta{}, sigma{}, rho{}, v0{}, S0;
    unsigned nParameters;
    double K = 0; // Strike price
    double T = 0; // Expiry date
    double r;
    std::string optType; // Option name (call, put)

    // additional parameter: displacement (phi_t)
    std::vector<double> times, deltaTimes, phiT;

    double N = 200;
    std::string cfType;
//    static std::vector<double> xGauss, wGauss;

public:
    // index needed for computation of integralPhi:
    int indexT = 0;
    // Constructors:
    EuropeanOption(double K, double T, double r, double kappa, double theta,
                   double sigma, double rho, double v0, double S0, std::string  optType = "C",
                   unsigned nParameters = 5, double q = 0,
                   std::string cfType = "Cui", double N = 200);

    EuropeanOption(double K, double T, double r, double S0, std::string  optType = "C",
                   unsigned nParameters = 5, double q = 0,
                   std::string cfType = "Cui", double N = 200);

// Destructor
    virtual ~EuropeanOption();
// Assignment operator
    EuropeanOption& operator = (const EuropeanOption& option2);
// Functions that calculate option price and (some) sensitivities
    virtual double Price() const = 0;
    virtual std::vector<double> Jacobian() const = 0;
    void SetIndexT();
    void setParameters(const double *parameters);
    void setDisplacement (const double *parameters, const std::vector<double> & finalTimes,
                          const std::vector<double> & deltaT);

private:
    virtual std::complex<double> CharFunc(std::complex<double> u) const = 0;
    virtual std::vector<std::complex<double>> JacobianCF(std::complex<double> u) const = 0;
    virtual double IntegralPhi() const = 0;
};

#endif //EX1_EUROPEANOPTION_H
