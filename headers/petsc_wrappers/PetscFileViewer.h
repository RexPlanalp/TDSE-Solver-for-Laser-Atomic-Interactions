#pragma once

#include <petscviewer.h>
#include <petscviewerhdf5.h>

//////////////////////////
// Petsc Viewer Wrapper //
//////////////////////////

enum class ViewerMode
{
    BINARY,
    HDF5
};

class PetscFileViewer
{
    public:

        // Default constructor
        PetscFileViewer() = default;

        // Destructor
        ~PetscFileViewer();

        // Internal viewer
        PetscViewer viewer;
};

//////////////////////////
// HDF5 Viewer Wrapper  //
//////////////////////////

class PetscHDF5Viewer : public PetscFileViewer
{
    public:

        // Explicit Constructor
        PetscHDF5Viewer(const char* filename);      
};





