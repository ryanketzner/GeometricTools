
#pragma once

#include <random>

#include <Mathematics/AlignedBox.h>

namespace gte
{
    template <int32_t N, typename Real>
    std::vector<Vector<N,Real>> RandomGrid(int num, AlignedBox<N,Real> const& 
        box, unsigned int seed = std::random_device{}())
    {
        std::vector<Vector<N,Real>> grid(num);
        std::mt19937 gen(seed);
 
        std::array<std::uniform_real_distribution<Real>,N> dis;
        
        // Create distribution over the interval in each dimension
        for (int i = 0; i < N; i++)
            dis[i] = 
                std::uniform_real_distribution<Real>(box.min[i], box.max[i]);

        for (int i = 0; i < num; i++)
            for (int j = 0; j < N; j++)
                grid[i][j] = dis[j](gen);

        return grid;
    }
}