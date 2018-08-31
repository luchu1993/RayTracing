#include "rt_math.h"
#include <iostream>
using namespace std;

float hit_sphere(const rt::vec3& center, float radius, const rt::ray& r)
{
    rt::vec3 origin = r.origin();
    rt::vec3 dir = r.direction();
    rt::vec3 oc = origin - center;

    float a = rt::dot(dir, dir);
    float b = rt::dot(oc, dir);
    float c = rt::dot(oc, oc) - radius * radius;
    float delta = b*b - a*c;

    return delta > 0.0f ? (-b-sqrt(delta))/a : -1.0f;
}

rt::vec3 color(rt::ray const& r)
{
    rt::vec3 center(0.0f, 0.0f, -1.0f);
    float t = hit_sphere(center, 0.5f, r);
    if (t > 0.0f)
    {
        rt::vec3 norm = rt::normalize(r.point_at_parameter(t) - center);
        return 0.5f * (norm + rt::vec3(1.0f));
    }
    rt::vec3 dir = rt::normalize(r.direction());
    t = 0.5 * (dir.y() + 1.0f);
    return (1.0 - t) * rt::vec3(1.0f, 1.0f, 1.0f) + t * rt::vec3(0.5f, 0.7f, 1.0f);
}

int main(int argc, char* argv[])
{
    int nx = 800;
    int ny = 400;
    cout << "P3\n" << nx << " " << ny << "\n255\n";
    
    rt::vec3 origin(0.0f, 0.0f, 0.0f);
    rt::vec3 lower_left_corner(-2.0f, -1.0f, -1.0f);
    rt::vec3 horizontal(4.0f, 0.0f, 0.0f);
    rt::vec3 vertical(0.0f, 2.0f, 0.0f);

    for (int j = ny-1; j >= 0; --j)
    {
        for (int i = 0; i < nx; ++i)
        {
            float u = (float) i / (float) nx;
            float v = (float) j / (float) ny;
            rt::ray r(origin, lower_left_corner + u * horizontal + v * vertical);
            rt::vec3 col = color(r);
            int ir = (int) (255.99f * col[0]);
            int ig = (int) (255.99f * col[1]);
            int ib = (int) (255.99f * col[2]);

            cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}