#ifndef PARTICLE_H
#define PARTICLE_H

#include "vec2.h"
#include "random.h"
#include "utility.h"

// Postition normalized to 0.0 to 1.0
class Particle {
    public:
        static double speed;

        Particle()
        {
            pos = random_unit_square();
            vel = vec2();
            acc = vec2();
        }

        void update(const vec2& force, Texture* finalImage)
        {
            acc = force;
            vel += acc;
            if (vel.length() > speed)
                vel = vel / vel.length() * speed;
            pos += vel;
            
            // Edge cases
            if (pos[0] < 0.0)
                pos[0] = 1.0;
            if (pos[0] > 1.0)
                pos[0] = 0.0; 
            if (pos[1] < 0.0)
                pos[1] = 1.0;
            if (pos[1] > 1.0)
                pos[1] = 0.0; 

            // finalImage->setIDX(pos[0], pos[1], clamp(finalImage->IDX(pos[0], pos[1]) + 0.1, 0.0, 1.0)); 
            finalImage->addIDX(pos[0], pos[1], 0.1);
        }

        void getPos(double& x, double& y) 
        {
            x = pos[0];
            y = pos[1];
        }

    // private:
        vec2 pos;
        vec2 vel;
        vec2 acc;
};

// Postition normalized to 0.0 to 1.0
class Particle2D {
    public:
        static double speed;

        Particle2D()
        {
            pos = random_unit_square();
            vel = vec2();
            acc = vec2();
        }

        void update(VecField* field, Bitmap* finalImage)
        {
            acc = field->IDX(pos[0], pos[1]);;
            vel += acc;
            if (vel.length() > speed)
                vel = vel / vel.length() * speed;
            pos += vel;
            
            // Edge cases
            if (pos[0] < 0.0)
                pos[0] = 1.0;
            if (pos[0] > 1.0)
                pos[0] = 0.0; 
            if (pos[1] < 0.0)
                pos[1] = 1.0;
            if (pos[1] > 1.0)
                pos[1] = 0.0; 

            // finalImage->setIDX(pos[0], pos[1], clamp(finalImage->IDX(pos[0], pos[1]) + 0.1, 0.0, 1.0)); 
            finalImage->setIDX(pos[0], pos[1], true);
        }

        void getPos(double& x, double& y) 
        {
            x = pos[0];
            y = pos[1];
        }

    // private:
        vec2 pos;
        vec2 vel;
        vec2 acc;
};

#endif