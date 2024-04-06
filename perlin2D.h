#ifndef PERLIN2D_H
#define PERLIN2D_H

#include "random.h"
#include "utility.h"
#include <cmath>

void permute(int* arr, int n)
{
    for (int i = n-1; i > 0; i--)
    {
        int random_index = random_int(0, i);
        std::swap(arr[i], arr[random_index]);
    }
}

int* generate_perm(int n) {
    auto p = new int[n];

    for(int i = 0; i < n; i++)
        p[i] = i;

    permute(p, n);

    return p;
}

// Perform dot product of (x, y) with 4 random vectors i, j, -i, -j
double grad(int hash, double x, double y)
{
    hash &= 3;
    if (hash == 0) return x;
    else if (hash == 1) return -x;
    else if (hash == 2) return y;
    else if (hash == 3) return -y;
    return 0;
}

class Perlin2D {
  public:
    Perlin2D(double f, double d) : frequency(f), depth(d)
    {
        permutation = generate_perm(perm_count);

        perm = new int[perm_count*2];

        for(int i = 0; i < perm_count*2; i++)
            perm[i] = permutation[i%256];
    }

    ~Perlin2D() {
        delete[] permutation;
        delete[] perm;
    }

    double noise(double x, double y)
    {
        x *= frequency;
        y *= frequency;
        int xi = static_cast<int>(floor(x)) & 255;
        int yi = static_cast<int>(floor(y)) & 255;
        double xf = x - floor(x);
        double yf = y - floor(y);
        double u = fade(xf);
        double v = fade(yf);

        int aa = perm[perm[xi  ] + yi  ];
        int ba = perm[perm[xi+1] + yi  ];
        int ab = perm[perm[xi  ] + yi+1];
        int bb = perm[perm[xi+1] + yi+1];

        double y1 = lerp(grad(aa, xf, yf  ), grad(ba, xf-1, yf  ), u);
        double y2 = lerp(grad(ab, xf, yf-1), grad(bb, xf-1, yf-1), u);
        return lerp(y1, y2, v);
    }

    double octaves(double x, double y)
    {
        double accum = 0.0;
        double freq = 1.0;
        double weight = 1.0;
        double max_val = 0.0;

        for (int i = 0; i < depth; i++){
            accum += weight * noise(x * freq, y * freq);
            weight *= 0.5;
            freq *= 2;
            max_val += weight;
        }
        
        return (accum / max_val + 1.0) / 2.0;
    }

  private:
    static const int perm_count = 256;
    int *permutation;
    int *perm;

    double frequency;
    double depth;
};

Texture* getPerlinMap(int width, int height, int frequency, int depth)
{
    double dx = 1.0 / width;
    double dy = 1.0 / height;

    double x = 0.0f, y = 0.0f;

    Perlin2D perlin_perm = Perlin2D(frequency, depth);
    double** texture = new double*[height];

    double dis;
    double perlin_height;
    double max_height = 0;

    for (int j = 0; j < height; j++)
    {
        texture[j] = new double[width];
        for (int i = 0; i < width; i++) {
            // std::clog << x << " " << y << std::endl;
            perlin_height = perlin_perm.octaves(x, y);
            texture[j][i] = perlin_height;
            x += dx;
        }
        y += dy;
        x = 0.0f;
    }

    Texture* perlinMap = new Texture(height, width);
    perlinMap->map = texture;
    perlinMap->height = height;
    perlinMap->width = width;

    return perlinMap;
}

#endif