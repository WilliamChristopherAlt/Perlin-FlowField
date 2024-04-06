#ifndef UTILITY_H
#define UTILITY_H

#include <cmath>
#include "vec2.h"
#define PI atan(1.0)*4.0

double clamp(double x, double min, double max)
{
    return std::max(std::min(x, max), min);
}

int clamp(int x, int min, int max)
{
    return std::max(std::min(x, max), min);
}

struct Texture {
    double** map;
    int height;
    int width;

    // Constructor
    Texture(int h, int w) : height(h), width(w) {
        map = new double*[height];
        for (int i = 0; i < height; ++i) {
            map[i] = new double[width];
            std::fill(map[i], map[i] + width, 0.0);
        }
    }

    // Destructor
    ~Texture() {
        for (int i = 0; i < height; ++i) {
            delete[] map[i];
        }
        delete[] map;
    }

    // Takes in normalized position and returns the value at that position
    double IDX(double x, double y) {
        int row = clamp(static_cast<int>((1.0 - y) * height), 0, height-1);
        int col = clamp(static_cast<int>(x * width), 0, width-1);
        return map[row][col];
    }

    // Takes in normalized position and sets the value at that position
    void setIDX(double x, double y, double val) {
        int row = clamp(static_cast<int>((1.0 - y) * height), 0, height-1);
        int col = clamp(static_cast<int>(x * width), 0, width-1);
        map[row][col] = val;
    }

    void addIDX(double x, double y, double val) {
        int row = clamp(static_cast<int>((1.0 - y) * height), 0, height-1);
        int col = clamp(static_cast<int>(x * width), 0, width-1);
        map[row][col] = clamp(map[row][col] + val, 0.0, 1.0-1e-10);
    }
};

struct Bitmap {
    bool** map;
    int height;
    int width;

    // Constructor
    Bitmap(int h, int w) : height(h), width(w) {
        map = new bool*[height];
        for (int i = 0; i < height; ++i) {
            map[i] = new bool[width];
            std::fill(map[i], map[i] + width, false);
        }
    }

    // Destructor
    ~Bitmap() {
        for (int i = 0; i < height; ++i) {
            delete[] map[i];
        }
        delete[] map;
    }

    // Takes in normalized position and returns the value at that position
    bool IDX(double x, double y) {
        int row = clamp(static_cast<int>((1.0 - y) * height), 0, height-1);
        int col = clamp(static_cast<int>(x * width), 0, width-1);
        return map[row][col];
    }

    // Takes in normalized position and sets the value at that position
    void setIDX(double x, double y, bool val) {
        int row = clamp(static_cast<int>((1.0 - y) * height), 0, height-1);
        int col = clamp(static_cast<int>(x * width), 0, width-1);
        // std::cout << row << ' ' << col << std::endl;
        map[row][col] = val;
    }
};

struct VecField {
    vec2** field;
    int height;
    int width;

    // Constructor
    VecField(int h, int w) : height(h), width(w) {
        field = new vec2*[height];
        for (int i = 0; i < height; ++i) {
            field[i] = new vec2[width];
            std::fill(field[i], field[i] + width, vec2());
        }
    }

    // Destructor
    ~VecField() {
        for (int i = 0; i < height; ++i) {
            delete[] field[i];
        }
        delete[] field;
    }

    // Takes in normalized position and returns the value at that position
    vec2 IDX(double x, double y) {
        int row = clamp(static_cast<int>((1.0 - y) * height), 0, height-1);
        int col = clamp(static_cast<int>(x * width), 0, width-1);
        return field[row][col];
    }

    // Takes in normalized position and sets the value at that position
    void setIDX(double x, double y, const vec2& val) {
        int row = clamp(static_cast<int>((1.0 - y) * height), 0, height-1);
        int col = clamp(static_cast<int>(x * width), 0, width-1);
        field[row][col] = val;
    }
};

VecField* toVecField(Texture* image)
{
    int height = image->height;
    int width = image->width;
    VecField* field = new VecField(height, width);

    for (int j = 0; j < height; j++)
        for (int i = 0; i < width; i++)
            field->field[j][i] = vec2(1.0, image->map[j][i]*2.0*PI, true);
    
    return field;
}

// Map (x, y) in R^2 coordinate to appropiate [j][i] index of image
bool** toImage(double* xs, double* ys, int steps, double xMin, double xMax, double yMin, double yMax, int imgWidth, int& imgHeight)
{
    int i, j;
    double coorWidth = xMax - xMin;
    double coorHeight = yMax - yMin;
    imgHeight = imgWidth * (coorHeight / coorWidth);

    bool** image = new bool*[imgHeight];
    for (j = 0; j < imgHeight; j++)
    {
        image[j] = new bool[imgWidth];
        for (i = 0; i < imgWidth; i++)
            image[j][i] = false;
    }

    int i_idx, j_idx;
    for(i = 0; i < steps; i++)
    {
        i_idx = clamp(static_cast<int>((xs[i] - xMin) * imgWidth / coorWidth), 0, imgWidth - 1);
        j_idx = clamp(static_cast<int>((ys[i] - yMin) * imgHeight / coorHeight), 0, imgHeight - 1);
        image[j_idx][i_idx] = true;
    }

    return image;
}

void imageToPPM(bool** image, int imgWidth, int imgHeight)
{
    int i, j;
    std::cout << "P3\n" << imgWidth << ' ' << imgHeight << "\n255\n";
    for (j = 0; j < imgHeight; j++)
        for (i = 0; i < imgWidth; i++)
            std::cout << (image[j][i] ? "0 0 0\n" : "255 255 255\n");
}

void imageToPPM(double** image, int imgWidth, int imgHeight, double gamma)
{
    int i, j;
    double brightness;
    std::cout << "P3\n" << imgWidth << ' ' << imgHeight << "\n255\n";
    for (j = 0; j < imgHeight; j++)
        for (i = 0; i < imgWidth; i++)
        {
            brightness = pow(image[j][i], gamma);
            std::cout << static_cast<int>(brightness * 256) << ' ' <<
                         static_cast<int>(brightness * 256) << ' ' <<
                         static_cast<int>(brightness * 256) << '\n';
        }
}

void imageToPPM(double** image, int imgWidth, int imgHeight)
{
    int i, j;
    double brightness;
    std::cout << "P3\n" << imgWidth << ' ' << imgHeight << "\n255\n";
    for (j = 0; j < imgHeight; j++)
        for (i = 0; i < imgWidth; i++)
        {
            brightness = image[j][i];
            std::cout << static_cast<int>(brightness * 256) << ' ' <<
                         static_cast<int>(brightness * 256) << ' ' <<
                         static_cast<int>(brightness * 256) << '\n';
        }
}

double ease_out(double x)
{
    return std::max(0.0, std::min(1.0, 1.0 - pow(1.0 - x, 6.0)));
}

double lerp(double a, double b, double t)
{
    return a + (b - a) * t;
}

double fade(double t)
{
    return t * t * t * (t * (t*6.0 - 15.0) + 10.0);
}

#endif