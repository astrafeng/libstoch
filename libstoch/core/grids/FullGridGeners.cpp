#include "libstoch/core/grids/FullGridGeners.h"
// add include for all derived classes
#include "libstoch/core/grids/RegularLegendreGridDerivedGeners.h"
#include "libstoch/core/grids/GeneralSpaceGridDerivedGeners.h"
#include "libstoch/core/grids/RegularSpaceGridDerivedGeners.h"

// Register all wrappers
SerializationFactoryForFullGrid::SerializationFactoryForFullGrid()
{
    this->registerWrapper<RegularLegendreGridDerivedGeners>();
    this->registerWrapper<GeneralSpaceGridDerivedGeners>();
    this->registerWrapper<RegularSpaceGridDerivedGeners>();
}
