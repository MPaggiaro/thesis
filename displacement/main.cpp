#include <iostream>
#include <vector>
#include "EuropeanOption.h"
#include "Calibration.h"

template < class T >
inline std::ostream& operator << (std::ostream& os, const std::vector<T>& v)
{
    os << "[";
    for (auto ii = v.begin(); ii != v.end(); ++ii)
    {
        os << " " << *ii;
    }
    os << " ]";
    return os;
}



int main() 
{
    double r = 0.02, theta = 0.1, kappa = 3.0, sigma = 0.25, rho = -0.8, v0 = 0.08, S0 = 1.0;
    double marketParameters[] = {v0, theta, rho, kappa, sigma};
    // int nParameters = 5;
    std::vector<double> SPX_strikes = {
            0.9371, 0.8603, 0.8112, 0.7760, 0.7470, 0.7216, 0.6699,
            0.6137, 0.9956, 0.9868, 0.9728, 0.9588, 0.9464, 0.9358,
            0.9175, 0.9025, 1.0427, 1.0463, 1.0499, 1.0530, 1.0562,
            1.0593, 1.0663, 1.0766, 1.2287, 1.2399, 1.2485, 1.2659,
            1.2646, 1.2715, 1.2859, 1.3046, 1.3939, 1.4102, 1.4291,
            1.4456, 1.4603, 1.4736, 1.5005, 1.5328};

    std::vector<double> SPX_maturities = {
            0.08219178, 0.16438356, 0.24657534, 0.32876712, 0.4109589,  0.49315068,
            0.73972603, 0.98630137, 0.08219178, 0.16438356, 0.24657534, 0.32876712,
            0.4109589,  0.49315068, 0.73972603, 0.98630137, 0.08219178, 0.16438356,
            0.24657534, 0.32876712, 0.4109589,  0.49315068, 0.73972603, 0.98630137,
            0.08219178, 0.16438356, 0.24657534, 0.32876712, 0.4109589,  0.49315068,
            0.73972603, 0.98630137, 0.08219178, 0.16438356, 0.24657534, 0.32876712,
            0.4109589,  0.49315068, 0.73972603, 0.98630137};

    std::vector<double> VIX_strikes = {
            0.07660333466800909, 0.07823036262505452, 0.08007866879151475,
            0.08197064388101638, 0.08371167160297435, 0.07528231789011931,
            0.07755350918528185, 0.08015741494276396, 0.08284874840358378,
            0.0853482112450969,  0.07430112345712658, 0.077055734551228,
            0.08023623852982013, 0.08354801898792094, 0.08664544590808211,
            0.0732872469748735,  0.07654634796281677, 0.08033769704753065,
            0.08431683207193616, 0.08806642128491488, 0.07084911309063555,
            0.07534553012964565, 0.08067681958626018, 0.08638533974549638,
            0.0918677586016848,  0.06910110193769552, 0.07450988995253548,
            0.08101737363349071, 0.08809320258894406, 0.0949885695945464};

    std::vector<double> VIX_maturities = {
            0.01917808, 0.01917808, 0.01917808, 0.01917808, 0.01917808, 0.03835616,
            0.03835616, 0.03835616, 0.03835616, 0.03835616, 0.05753425, 0.05753425,
            0.05753425, 0.05753425, 0.05753425, 0.08219178, 0.08219178, 0.08219178,
            0.08219178, 0.08219178, 0.16438356, 0.16438356, 0.16438356, 0.16438356,
            0.16438356, 0.24657534, 0.24657534, 0.24657534, 0.24657534, 0.24657534};

    //Calibration calibration(SPX_strikes, SPX_maturities, VIX_strikes, VIX_maturities,
      //                      marketParameters, r, S0);

    // try new displacement initialization:
    std::vector<double> displacement_vector(14,5e-4);
//    Calibration displaced_calibration(SPX_strikes, SPX_maturities, VIX_strikes, VIX_maturities,
//                                      displacement_vector,marketParameters, r, S0);
//    auto prices = displaced_calibration.VIX_Prices();

    // Let's build the calibration in an alternative (more correct way):
    // Longer parameters vector:
    double parametersWithDisplacement[5 + displacement_vector.size()];
    for (int i = 0; i < 5; ++i)
    {
        parametersWithDisplacement[i] = marketParameters[i];
    }
    for (int i = 0; i < displacement_vector.size(); ++i)
    {
        parametersWithDisplacement[i + 5] = displacement_vector[i];
    }

    Calibration calibration (SPX_strikes, SPX_maturities, VIX_strikes,
                             VIX_maturities, parametersWithDisplacement,
                             r, S0, true);

    // calibrate(calibration);
    double initialGuess[EuropeanOption::nParameters];
    initialGuess[0] = 0.2;
    initialGuess[1] = 0.2;
    initialGuess[2] = -0.6;
    initialGuess[3] = 1.2;
    initialGuess[4] = 0.3;
    if (EuropeanOption::nParameters > 5)
        for (int i = 5; i < EuropeanOption::nParameters; ++i)
            initialGuess[i] = 2e-4;

    // Let's debug this:
//    auto gradients = calibration.Gradients();
//    double gradientP[calibration.size() * EuropeanOption::nParameters];
//    computeGradients(initialGuess, gradientP, 1,1, (void * ) &calibration);

    calibrate(calibration, initialGuess);
    // calibrate(calibration);
    std::cout << "End of program" << std::endl;
    return 0;
}