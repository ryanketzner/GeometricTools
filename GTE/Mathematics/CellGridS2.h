// Addon to GeometricTools

#pragma once

#include <vector>
#include <algorithm>

#include <Mathematics/Logger.h>
#include <Mathematics/AlignedBoxS2.h>
#include <Mathematics/Math.h>

// This class is used for creating cell grids on the spherical surface.
// The cell grid is defined on an AlignedBoxS2.
// The cell grid has possibly variable latitude spacing, but fixed
// longitude spacing along a given latitude.
// A point is contained within a cell in the grid if it is inside the cell
// or on its left or bottom boundary. The only exception to this are the top
// row of cells, which include their top boundary, and the right row of cells,
// which include their right boundary. 

// TODO: Right now, PI and -PI longitude are both allowed. This may need fixing.
// TODO: Make comments more extensive and cleaner, with consistent style.

namespace gte
{
    template <typename Real, typename T>
    class CellGridS2
    {
        public:

            /**
             * @brief Construct a new Cell Grid S2 object
             * 
             * @param lat_bounds Vector of latitude boundaries, in increasing order. Number of latitude bands is lat_bounds - 1.
             * @param num_lon Vector specifying the number of longitude cells in each latitude band.
             * @param box Aligned box on the sphere where grid is defined.
             * @param data Data to be stored in the cell grid. Number of elements in each vector must equal corresponding element in num_lon.
             */
            CellGridS2(std::vector<Real> const& lat_bounds, std::vector<int> const& num_lon, 
            AlignedBoxS2<Real> const& box, std::vector<std::vector<T>> const& data) :
            lat_bounds(lat_bounds),
            num_lon(num_lon),
            box(box),
            data(data)
            {
                LogAssert(lat_bounds.front() == box.latMin && lat_bounds.back() == box.latMax, "First and last lat boundaries must match box boundaries.");
                LogAssert(std::is_sorted(lat_bounds.begin(),lat_bounds.end()), "Lat bounds must be sorted in ascending order.");
                
                int num_lat = lat_bounds.size() - 1;
                LogAssert(num_lat == num_lon.size(), "One value for number of longitude elements must be specified for each latitude element.");

                LogAssert(data.size() == num_lat, "One data vector must be provided per lat band.")
                for (int i = 0; i < num_lat; i++)
                    LogAssert(num_lon[i] == data[i].size(), "Size of data vector at each lat band must equal number of lon cells.");
            }

            // Get the value in the cell touched by point lat,lon
            // Intervals which define lat bands and lon cells have hard left boundaries [ and soft right boundaries ),
            // except for right boundary of last interval.
            // This function assumes the caller has confirmed that  minLat <= lat <= maxLat, minLon <= lon <= maxLon
            T& at(Real lat, Real lon)
            {
                int idx_lat, idx_lon;

                if (lat == box.latMax)
                    idx_lat = lat_bounds.size() - 2;
                else
                {
                    // Get iterator to first element in lat boundary list which is greater than lat
                    auto it = std::upper_bound(lat_bounds.begin(), lat_bounds.end(), lat);
                    // Step back iterator by one to get first element <= lat
                    it--;
                    // Index of the latitude cell
                    idx_lat = std::distance(lat_bounds.begin(), it);
                }

                
                if (lon == box.lonMax)
                    idx_lon = num_lon[idx_lat] - 1;
                else
                {
                    Real delta_lon = box.LonLength()/num_lon[idx_lat];
                    // Cast to int takes floor of Real value
                    idx_lon = static_cast<int>((lon + GTE_C_PI)/delta_lon);
                }

                return data.at(idx_lat).at(idx_lon);
            }

            // Grid is contained within this spherical aligned box
            AlignedBoxS2<Real> box;

        private:

            // Boundaries of latitude bands, which may have unequal spacing
            std::vector<Real> lat_bounds;
            // Number of longitude cells in each latitude band
            std::vector<int> num_lon;
            // Data stored in cells
            std::vector<std::vector<T>> data;
    };
}