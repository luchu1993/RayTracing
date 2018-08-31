#include "rt_math.h"
#include <iostream>
using namespace std;

rt::vec3 color(rt::ray const& r)
{
    rt::vec3 dir = rt::normalize(r.direction());
    float t = 0.5 * (dir.y() + 1.0f);
    return (1.0 - t) * rt::vec3(1.0f, 1.0f, 1.0f) + t * rt::vec3(0.5f, 0.7f, 1.0f);
}

int main(int argc, char* argv[])
{
    int nx = 200;
    int ny = 100;
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