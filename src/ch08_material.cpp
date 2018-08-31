#include "rt_math.h"
#include <iostream>
#include <cstdlib>
#include <cfloat>
using namespace std;

rt::vec3 color(rt::ray const& r, rt::hitable* world, int depth)
{
    rt::hit_record rec;
    if (world->hit(r, 0.0001f, MAXFLOAT, rec))
    {
        rt::ray scattered;
        rt::vec3 attenuation;
        if (depth > 0 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * color(scattered, world, depth-1);
        else
            return rt::vec3(0.0f);
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
    int ns = 200;
    cout << "P3\n" << nx << " " << ny << "\n255\n";

    rt::hitable* list[4];
    list[0] = new rt::sphere(rt::vec3(0.0f, 0.0f, -1.0f), 0.5f, new rt::lambertian(rt::vec3(0.8f, 0.3f, 0.3f)));
    list[1] = new rt::sphere(rt::vec3(0.0f, -100.5f, -1.0f), 100.0f, new rt::lambertian(rt::vec3(0.8f, 0.8f, 0.0f)));
    list[2] = new rt::sphere(rt::vec3(1.0f, 0.0f, -1.0f), 0.5f, new rt::metal(rt::vec3(0.8f, 0.6f, 0.2f), 0.2f));
    list[3] = new rt::sphere(rt::vec3(-1.0f, 0.0f, -1.0f), 0.5f, new rt::metal(rt::vec3(0.8f, 0.8f, 0.8f)));
    rt::hitable* world = new rt::hitable_list(list, 4);
    rt::camera cam;
    for (int j = ny-1; j >= 0; --j)
    {
        for (int i = 0; i < nx; ++i)
        {
            rt::vec3 col(0.0f, 0.0f, 0.0f);
            for (int s = 0; s < ns; ++s)
            {
                float u = (float) (i + drand48()) / (float) nx;
                float v = (float) (j + drand48()) / (float) ny;
                rt::ray r = cam.get_ray(u, v);
                col += color(r, world, 50);
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