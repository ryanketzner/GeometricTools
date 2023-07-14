
#pragma once

#include <random>

#include <Mathematics/Hypersphere.h>

namespace gte
{
    template <int32_t N, typename Real>
    std::vector<Vector<N,Real>> RandomSurfaceGrid(int num, Hypersphere<N,Real>
        const& sphere, unsigned int seed = std::random_device{}())
    {
        std::mt19937 gen(seed);
        std::normal_distribution<Real> dist((Real)0, (Real)1);

        std::vector<Vector<N,Real>> grid(num);
        for (int i = 0; i < num; i++)
        {
            for (int j = 0; j < N; j++)
                grid[i][j] = dist(gen);

            Normalize(grid[i]);
            // Translate and scale
            grid[i] = grid[i]*sphere.radius + sphere.center;
        }

        return grid;
    }
}
