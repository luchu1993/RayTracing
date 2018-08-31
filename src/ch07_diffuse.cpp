#include "rt_math.h"
#include <iostream>
#include <random>
#include <cfloat>
using namespace std;

rt::vec3 color(rt::ray const& r, rt::hitable* world)
{
    rt::hit_record rec;
    if (world->hit(r, 0.0001f, MAXFLOAT, rec))
    {
        rt::vec3 target = rec.point + rec.normal + rt::random_unit_sphere();
        return 0.5f * color(rt::ray(rec.point, target - rec.point), world);
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
    int nx = 200;
    int ny = 100;
    int ns = 100;
    cout << "P3\n" << nx << " " << ny << "\n255\n";

    rt::hitable* list[2];
    list[0] = new rt::sphere(rt::vec3(.0f, 0.0f, -1.0f), 0.5f);
    list[1] = new rt::sphere(rt::vec3(.0f, -100.5f, -1.0f), 100.0f);
    rt::hitable* world = new rt::hitable_list(list, 2);
    rt::camera cam;

    default_random_engine generator;
    uniform_real_distribution<float> distribution(0.0f, 1.0f);
    for (int j = ny-1; j >= 0; --j)
    {
        for (int i = 0; i < nx; ++i)
        {
            rt::vec3 col(0.0f, 0.0f, 0.0f);
            for (int s = 0; s < ns; ++s)
            {
                float u = (float) (i + distribution(generator)) / (float) nx;
                float v = (float) (j + distribution(generator)) / (float) ny;
                rt::ray r = cam.get_ray(u, v);
                col += color(r, world);
            }
            
            col /= (float) ns;
            col = rt::vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = (int) (255.99f * col[0]);
            int ig = (int) (255.99f * col[1]);
            int ib = (int) (255.99f * col[2]);

            cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}