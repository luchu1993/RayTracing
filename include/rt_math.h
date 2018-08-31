#pragma once
#include <cmath>
#include <iostream>

namespace rt {

class material;

class vec3 {
public:
    vec3() { }
    vec3(float t) { e[0] = t; e[1] = t; e[2] = t;}
    vec3(float x, float y, float z) { e[0] = x; e[1] = y; e[2] = z; }
    inline float x() const { return e[0]; }
    inline float y() const { return e[1]; }
    inline float z() const { return e[2]; }
    inline float r() const { return e[0]; }
    inline float g() const { return e[1]; }
    inline float b() const { return e[2]; }
    inline const vec3& operator+() const { return *this; }
    inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    inline float operator[](int i) const { return e[i]; } 
    inline float& operator[](int i) { return e[i]; } 

    vec3& operator+=(const vec3& v);
    vec3& operator-=(const vec3& v);
    vec3& operator*=(const vec3& v);
    vec3& operator/=(const vec3& v);
    vec3& operator*=(float t);
    vec3& operator/=(float t);

    inline float length() const {
        return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    }

    inline float squared_length() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    inline void make_unit_vector();

    float e[3];
};

inline std::istream& operator>> (std::istream &is, vec3& t)
{
    is >> t.e[0] >> t.e[1] >> t.e[2];
    return is;
}

inline std::ostream& operator<< (std::ostream &os, const vec3& t)
{
    os << t.e[0] << " " << t.e[1] << " " << t.e[2];
    return os;
}

vec3 operator+(const vec3& v1, const vec3& v2);
vec3 operator-(const vec3& v1, const vec3& v2);
vec3 operator*(const vec3& v1, const vec3& v2);
vec3 operator/(const vec3& v1, const vec3& v2);

vec3 operator*(const vec3& v, float t);
vec3 operator/(const vec3& v, float t);
vec3 operator*(float t, const vec3& v);

float dot(const vec3& v1, const vec3& v2);
vec3 cross(const vec3& v1, const vec3& v2);
vec3 normalize(const vec3& v);

class ray 
{
public:
    ray() { }
    ray(const vec3& a, const vec3& b)
    {
        A = a;
        B = b;
    }

    inline vec3 origin() const { return A; }
    inline vec3 direction() const { return B; }
    inline vec3 point_at_parameter(float t) const { return A + t*B; }

    vec3 A;
    vec3 B;
};
vec3 random_unit_sphere();

inline vec3 reflect(const vec3& v, const vec3& n)
{
    return v - 2.0f * dot(n, v) * n;
}

bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted);
float schlick(float cosine, float ref_idx);

inline float clamp(float f, float min, float max)
{
    float ret = f;
    if (f < min) ret = f;
    if (f > max) ret = f;
    return f;
}

typedef struct
{
    float t;
    vec3 point;
    vec3 normal;
    material* mat_ptr;
} hit_record;

class hitable
{
public:
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

class hitable_list : public hitable
{
public:
    hitable_list() {}
    hitable_list(hitable** l, int n) : list(l), list_size(n) { }
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;

    hitable** list;
    int list_size;
};

class sphere : public hitable
{
public:
    sphere() { }
    sphere(vec3 const& cen, float r) : center(cen), radius(r) { }
    sphere(vec3 const& cen, float r, material* mat) : center(cen), radius(r), mat_ptr(mat) { }
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;

    vec3 center;
    float radius;
    material* mat_ptr;
};

class camera 
{
public:
    camera() 
    {
        origin = vec3(0.0f, 0.0f, 0.0f);
        lower_left_corner = vec3(-2.0f, -1.0f, -1.0f);
        horizontal = vec3(4.0f, 0.0f, 0.0f);
        vertical = vec3(0.0f, 2.0f, 0.0f);
    }

    camera(rt::vec3 look_from, rt::vec3 look_at, rt::vec3 up, float fov, float aspect);


    inline ray get_ray(float u, float v)
    {
        return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
    }

    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
};

class material 
{
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

class lambertian : public material
{
public:
    lambertian(const vec3& a) : albedo(a) { }
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const;

    vec3 albedo;
};

class metal : public material
{
public:
    metal(const vec3& a) : albedo(a), fuzz(0.0f) { }
    metal(const vec3& a, float f) : albedo(a), fuzz(clamp(f, 0.0f, 1.0f)) { }
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const;
    vec3 albedo;
    float fuzz;
};

class dielectric : public material
{
public:
    dielectric(float ri) : ref_idx(ri) { }
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const;
    float ref_idx;
};

}