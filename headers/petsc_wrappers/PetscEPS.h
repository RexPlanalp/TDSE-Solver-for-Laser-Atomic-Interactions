#pragma once

#include <slepceps.h>
#include "simulation.h"
#include "petsc_wrappers/PetscMatrix.h"
#include "petsc_wrappers/PetscVector.h"

//////////////////////////
// Petsc EPS Wrapper   //
//////////////////////////


class PetscEPS
{
    public:

        // Default Constructor
        PetscEPS();

        // Destructor
        ~PetscEPS();

        // Set convergence params
        void setConvergenceParams(const simulation& sim); 

        // Set parameters for solver
        void setSolverParams(int requested_pairs);

        // Set operators
        void setOperators(const PetscMatrix& H, const PetscMatrix& S);

        // Solve the eigenvalue problem
        int solve();

        // Get eigenvalue of pair
        std::complex<double> getEigenvalue(int i);

        // Get the eigenvector of pair
        PetscVector getEigenvector(int i, const PetscMatrix& S);

        // Internal EPS 
        EPS eps;
};
