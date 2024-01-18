#include "libstoch/core/grids/SpaceGridGeners.h"
// add include for all derived classes
#include "libstoch/core/grids/RegularLegendreGridGeners.h"
#include "libstoch/core/grids/GeneralSpaceGridGeners.h"
#include "libstoch/core/grids/RegularSpaceGridGeners.h"
#include "libstoch/core/grids/SparseSpaceGridBoundGeners.h"
#include "libstoch/core/grids/SparseSpaceGridNoBoundGeners.h"

// Register all wrappers
SerializationFactoryForSpaceGrid::SerializationFactoryForSpaceGrid()
{
    this->registerWrapper<RegularLegendreGridGeners>();
    this->registerWrapper<GeneralSpaceGridGeners>();
    this->registerWrapper<RegularSpaceGridGeners>();
    this->registerWrapper<SparseSpaceGridBoundGeners>();
    this->registerWrapper<SparseSpaceGridNoBoundGeners>();
}
