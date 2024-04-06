#include <iostream>

#include "particle.h"
#include "perlin2D.h"
#include "utility.h"
#include "random.h"

double Particle2D::speed = 0.005;

int main()
{
    int perlin_height = 100;
    int perlin_width = 100;
    double frequency = 6.0;
    int octave = 5;

    int imgHeight = 1000;
    int imgWidth = 1000;
    int STEPS = 30000;

    Texture* perlin_map = getPerlinMap(perlin_height, perlin_width, frequency, octave);
    VecField* field = toVecField(perlin_map);
    // Texture* finalImage = new Texture(imgHeight, imgWidth);
    Bitmap* finalImage = new Bitmap(imgHeight, imgWidth);

    int numParticles = 1000;
    int i, j;

    Particle2D* particles = new Particle2D[numParticles];
    for (i = 0; i < numParticles; i++)
        particles[i] = Particle2D(random_unit_square());
    
    for(i = 0; i < STEPS; i++)
        for (j = 0; j < numParticles; j++)
            particles[j].update(field, finalImage);
    
    imageToPPM(finalImage->map, imgHeight, imgWidth);
}