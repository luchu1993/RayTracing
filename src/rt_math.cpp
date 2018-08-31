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


rt::camera::camera
(
    rt::vec3 look_from, 
    rt::vec3 look_at, 
    rt::vec3 up, 
    float fov, 
    float aspect
)  
{
    rt::vec3 u, v, w;
    float theta = fov / 180.0f * M_PI;
    float half_height = tan(theta/2.0f);
    float half_width = half_height * aspect;
    origin = look_from;
    w = rt::normalize(look_from - look_at);
    u = rt::normalize(rt::cross(up, w));
    v = rt::cross(w, u);
    lower_left_corner = origin - half_width * u - half_height * v - w;
    horizontal = 2.0f * half_width * u;
    vertical = 2.0f * half_height * v;
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
            rec.mat_ptr = mat_ptr;
            return true;
        }

        t = (-b + sqrt(discriminant)) / a;
        if (t < t_max && t > t_min)
        {
            rec.t = t;
            rec.point = r.point_at_parameter(t);
            rec.normal = (rec.point - center) / radius; 
            rec.mat_ptr = mat_ptr;
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

bool rt::lambertian::scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
{
    vec3 target = rec.point + rec.normal + random_unit_sphere();
    scattered = ray(rec.point, target - rec.point);
    attenuation = albedo;
    return true;
}

bool rt::metal::scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
{
    vec3 reflected = reflect( normalize(r_in.direction()), rec.normal);
    if (fuzz != 0.0f)
        scattered = ray(rec.point, reflected + fuzz * random_unit_sphere());
    else
        scattered = ray(rec.point, reflected);
    attenuation = albedo;
    return dot(scattered.direction(), rec.normal) > 0;
}

bool rt::dielectric::scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
{
    vec3 outward_normal;
    vec3 dir = normalize(r_in.direction());
    vec3 reflected = reflect(dir, rec.normal);
    float ni_over_nt;
    attenuation = vec3(1.0f, 1.0f, 1.0f);
    vec3 refracted;
    float cosine, reflect_prob;

    if (dot(dir, rec.normal) > 0)
    {
        outward_normal = -rec.normal;
        ni_over_nt = ref_idx;
        cosine = dot(dir, rec.normal);
    }
    else
    {
        outward_normal = rec.normal;
        ni_over_nt = 1.0f/ref_idx;
        cosine = -dot(dir, rec.normal);
    }
    if (refract(dir, outward_normal, ni_over_nt, refracted))
        reflect_prob = rt::schlick(cosine, ref_idx);
    else
        reflect_prob = 1.0f;
    
    if (drand48() < reflect_prob)
        scattered = ray(rec.point, reflected);
    else
        scattered = ray(rec.point, refracted);

    return true;
}

bool rt::refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted)
{
    vec3 uv = rt::normalize(v);
    float dt = dot(uv, n);
    float discriminant = 1.0 - ni_over_nt*ni_over_nt*(1-dt*dt);
    if (discriminant > 0.0f)
    {
        refracted = ni_over_nt*(uv - n*dt) - n*sqrt(discriminant);
        return true;
    }
    else
        return false;
}

/*
 * @description: Schlick's approximation
 *  R(\theta) = R_0 + (1-R_0)(1-\cos\theta)^5
 *  where R_0 = (\frac{n_1-n_2}{n_1+n_2})^2, n_1 and n_2 
 *  are the indices of the two media at the interface,
 *  and \cos\theta = dot(N, N).
 */
float rt::schlick(float cosine, float ref_idx)
{
    float r = (1.0f - ref_idx) / (1.0f + ref_idx);
    r = r * r;
    return r + (1.0f - r) * pow(1.0f - cosine, 5.0f);
}

