#include "bsplines.h"
#include <cmath>
#include "gauss.h"
#include <fstream>
#include <iostream>

bsplines::bsplines(std::string& filename) : data(filename)
{
    _compute_complex_knots();
}

std::complex<double> bsplines::ecs_x(double x)
{
    if (x < bspline_data["R0"].get<double>())
    {
        return std::complex<double>(x, 0.0);
    }
    else
    {
        return bspline_data["R0"].get<double>() +
               (x - bspline_data["R0"].get<double>()) *
               std::exp(std::complex<double>(0, M_PI * bspline_data["eta"].get<double>()));
    }
}

std::complex<double> bsplines::ecs_w(double x, double w)
{
    if (x < bspline_data["R0"].get<double>())
    {
        return std::complex<double>(w, 0.0);
    }
    else 
    {
        return w * std::exp(std::complex<double>(0, M_PI * bspline_data["eta"].get<double>()));
    }
}

void bsplines::_compute_complex_knots()
{   
    for (int i = 0; i < this->knots.size(); i++)  
    {
        complex_knots.push_back(this->ecs_x(this->knots[i])); // Use 'this->' to access inherited members
    }
}



std::complex<double> bsplines::B(int i, int degree, std::complex<double> x)
{
    if (degree == 0)
    {
        return (complex_knots[i].real() <= x.real() && x.real() < complex_knots[i+1].real() ? 1.0 : 0.0);
    }


    std::complex<double> denom1 = complex_knots[i + degree] - complex_knots[i];
    std::complex<double> denom2 = complex_knots[i + degree + 1] - complex_knots[i + 1];

    std::complex<double> term1 = 0.0;
    std::complex<double> term2 = 0.0;

    if (denom1.real() > 0)
    {
        term1 = (x-complex_knots[i]) / (denom1) * B(i,degree-1,x);
    }
    if (denom2.real()>0)
    {
        term2 = (complex_knots[i+degree+1] -x ) / (denom2) * B(i+1,degree-1,x);
    }

    return term1+term2;


}

std::complex<double> bsplines::dB(int i, int degree, std::complex<double> x)
{
    if (degree == 0)
    {
        return 0.0;
    }

    std::complex<double> denom1 = complex_knots[i + degree] - complex_knots[i];
    std::complex<double> denom2 = complex_knots[i + degree + 1] - complex_knots[i + 1];

    std::complex<double> term1 = 0.0;
    std::complex<double> term2 = 0.0;

    if (denom1.real() > 0)
    {
        term1 = std::complex<double>(degree)/(denom1) * B(i,degree-1,x);
    }
    if (denom2.real()>0)
    {
        term2 = -std::complex<double>(degree)/(denom2) * B(i+1,degree-1,x);
    }

    return term1+term2;
}

void bsplines::save_debug_info_bsplines(int rank)
{
    if (!misc_data["debug"].get<int>()) return; // Only save if debugging is enabled

    if (rank == 0)
    {   
        auto space_range = grid_data["space_range"].get<std::array<double,3>>();
        double rmin = space_range[0];
        double rmax = space_range[1];
        double dr = space_range[2];

        std::ofstream file1("bsplines.txt");
        if (!file1.is_open())
        {
            std::cerr << "Error: could not open file bsplines.txt" << std::endl;
            return;
        }

        for (int i = 0; i <bspline_data["n_basis"].get<int>(); i++)
        {
            for (int idx = 0; idx < grid_data["Nr"].get<int>(); ++idx)
            {
                double x_val = rmin + idx * dr;
                std::complex<double> x = ecs_x(x_val);
                std::complex<double> val = B(i,bspline_data["degree"].get<int>(),x);
                file1 << val.real() << "\t" << val.imag() << "\n";
            }
            file1 << "\n";
        }
        file1.close();

        std::ofstream file2("dbsplines.txt");
        if (!file2.is_open())
        {
            std::cerr << "Error: could not open file bsplines.txt" << std::endl;
            return;
        }

        for (int i = 0; i <bspline_data["n_basis"].get<int>(); i++)
        {
            for (int idx = 0; idx < grid_data["Nr"].get<int>(); ++idx)
            {
                double x_val = rmin + idx * dr;
                std::complex<double> x = ecs_x(x_val);
                std::complex<double> val = dB(i,bspline_data["degree"].get<int>(),x);
                file2 << val.real() << "\t" << val.imag() << "\n";
            }
            file2 << "\n";
        }
        file2.close();
    }
}
    