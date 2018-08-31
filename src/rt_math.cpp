#include "rt_math.h"
#include <stdlib.h>
using namespace rt;

vec3& vec3::operator+=(const vec3& v)
{
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
}

vec3& vec3::operator-=(const vec3& v)
{
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    e[2] -= v.e[2];
    return *this;
}

vec3& vec3::operator*=(const vec3& v)
{
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    e[2] *= v.e[2];
    return *this;
}

vec3& vec3::operator/=(const vec3& v)
{
    e[0] /= v.e[0];
    e[1] /= v.e[1];
    e[2] /= v.e[2];
    return *this;
}

vec3& vec3::operator*=(float t)
{
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
}

vec3& vec3::operator/=(float t)
{
    e[0] /= t;
    e[1] /= t;
    e[2] /= t;
    return *this;
}

void vec3::make_unit_vector()
{
    float k = 1.0f / sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    e[0] *= k; 
    e[1] *= k; 
    e[2] *= k;
}

vec3 rt::operator+(const vec3& v1, const vec3& v2)
{
    return vec3
    (
        v1.e[0] + v2.e[0],
        v1.e[1] + v2.e[1],
        v1.e[2] + v2.e[2]
    );
}

vec3 rt::operator-(const vec3& v1, const vec3& v2)
{
    return vec3
    (
        v1.e[0] - v2.e[0],
        v1.e[1] - v2.e[1],
        v1.e[2] - v2.e[2]
    );
}

vec3 rt::operator*(const vec3& v1, const vec3& v2)
{
    return vec3
    (
        v1.e[0] * v2.e[0],
        v1.e[1] * v2.e[1],
        v1.e[2] * v2.e[2]
    );
}

vec3 rt::operator/ (const vec3& v1, const vec3& v2)
{
    return vec3
    (
        v1.e[0] / v2.e[0],
        v1.e[1] / v2.e[1],
        v1.e[2] / v2.e[2]
    );
}

vec3 rt::operator* (const vec3& v, float t)
{
    return vec3
    (
        v.e[0] * t,
        v.e[1] * t,
        v.e[2] * t
    );
}

vec3 rt::operator/ (const vec3& v, float t)
{
    return vec3
    (
        v.e[0] / t,
        v.e[1] / t,
        v.e[2] / t
    );
}

vec3 rt::operator* (float t, const vec3& v)
{
    return vec3
    (
        v.e[0] * t,
        v.e[1] * t,
        v.e[2] * t
    );
}

float rt::dot(const vec3& v1, const vec3& v2)
{
    return  v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

vec3 rt::cross(const vec3& v1, const vec3& v2)
{
    return vec3
    (
        v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1],
        v1.e[2] * v2.e[0] - v1.e[0] * v2.e[2],
        v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]
    );
}

vec3 rt::normalize(const vec3& v)
{
    float k = 1.0f / sqrt(v.e[0] * v.e[0] + v.e[1] * v.e[1] + v.e[2] * v.e[2]);
    return vec3
    (
        v.e[0] * k,
        v.e[1] * k,
        v.e[2] * k
    );
}

bool rt::sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b*b - a*c;
    if (discriminant > 0.0f)
    {
        float t = (-b - sqrt(discriminant)) / a;
        if (t < t_max && t > t_min)
        {
            rec.t = t;
            rec.point = r.point_at_parameter(t);
            rec.normal = (rec.point - center) / radius; 
            return true;
        }

        t = (-b + sqrt(discriminant)) / a;
        if (t < t_max && t > t_min)
        {
            rec.t = t;
            rec.point = r.point_at_parameter(t);
            rec.normal = (rec.point - center) / radius; 
            return true;
        }
    }
    return false;
}

bool rt::hitable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
    hit_record temp_rec;
    bool hit_anything = false;
    float closest_so_far = t_max;

    for (int i = 0; i < list_size; ++i) 
    {
        if (list[i]->hit(r, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

vec3 rt::random_unit_sphere()
{
    vec3 p;
    do {
        p = 2.0f * vec3(drand48(), drand48(), drand48()) - vec3(1.0f);
    } while (p.squared_length() >= 1.0f);
    
    return p;
}