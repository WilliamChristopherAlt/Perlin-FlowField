#ifndef VEC2_H
#define VEC2_H

#include <cmath>
#include <iostream>

class vec2
{
  public:
    double e[2];

    vec2() : e{0.0, 0.0} {}
    vec2(double e0, double e1) : e{e0, e1} {}
    vec2(double r, double angle, bool polar) : e{r*cos(angle), r*sin(angle)} {}

    double x() const {return e[0];}
    double y() const {return e[1];}

    vec2 operator-() const {return vec2(-e[0], -e[1]);}
    double operator[](int i) const {return e[i];}
    double& operator[](int i) {return e[i];}

    vec2& operator+=(const vec2 &v)
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        return *this;
    }

    vec2& operator*=(double t)
    {
        e[0] *= t;
        e[1] *= t;
        return *this;
    }

    vec2& operator/=(double t)
    {
        return *this *= 1/t;
    }

    double length() const
    {
        return sqrt(length_square());
    }

    double length_square() const 
    {
        return e[0]*e[0] + e[1]*e[1];
    }
};

inline std::ostream& operator<<(std::ostream &out, const vec2 &v) 
{
    return out << v.e[0] << ' ' << v.e[1];
}

inline vec2 operator+(const vec2 &u, const vec2 &v){
    return vec2(u.e[0] + v.e[0], u.e[1] + v.e[1]);
}

inline vec2 operator-(const vec2 &u, const vec2 &v){
    return vec2(u.e[0] - v.e[0], u.e[1] - v.e[1]);
}

inline vec2 operator*(const vec2 &u, const vec2 &v){
    return vec2(u.e[0] * v.e[0], u.e[1] * v.e[1]);
}

inline vec2 operator*(double t, const vec2 &v){
    return vec2(t * v.e[0], t * v.e[1]);
}

inline vec2 operator*(const vec2 &v, double t){
    return t * v;
}

inline vec2 operator/(const vec2 &v, double t){
    return (1/t) * v;
}

inline vec2 operator+(const vec2 &u, double t){
    return vec2(u.e[0] + t, u.e[1] + t);
}

inline vec2 operator+(double t, const vec2 &u){
    return u + t;
}

inline double dot(const vec2 &u, const vec2 &v){
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1];
}

inline vec2 unit_vector(vec2 v){
    return v / v.length();
}

inline vec2 lerp(const vec2& u, const vec2& v, double t)
{
    return vec2(
        u[0] + (v[0] - u[0]) * t,
        u[1] + (v[1] - u[1]) * t
    );
}

#endif