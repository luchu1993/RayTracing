#include "rt_math.h"
#include <iostream>
#include <cfloat>
using namespace std;

rt::vec3 color(rt::ray const& r, rt::hitable* world)
{
    rt::hit_record rec;
    if (world->hit(r, 0.0f, MAXFLOAT, rec))
    {
        return 0.5f * (rec.normal + rt::vec3(1.0f));
    }
    else
    {
        rt::vec3 dir = rt::normalize(r.direction());
        float t = 0.5f * (dir.y() + 1.0f);
        return rt::vec3(1.0f -t) * rt::vec3(1.0f, 1.0f, 1.0f) + t * rt::vec3(0.5f, 0.7f, 1.0f);
    }
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

    rt::hitable* list[2];
    list[0] = new rt::sphere(rt::vec3(.0f, 0.0f, -1.0f), 0.5f);
    list[1] = new rt::sphere(rt::vec3(.0f, -100.5f, -1.0f), 100.0f);
    rt::hitable* world = new rt::hitable_list(list, 2);

    for (int j = ny-1; j >= 0; --j)
    {
        for (int i = 0; i < nx; ++i)
        {
            float u = (float) i / (float) nx;
            float v = (float) j / (float) ny;
            rt::ray r(origin, lower_left_corner + u * horizontal + v * vertical);
            rt::vec3 col = color(r, world);
            int ir = (int) (255.99f * col[0]);
            int ig = (int) (255.99f * col[1]);
            int ib = (int) (255.99f * col[2]);

            cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}