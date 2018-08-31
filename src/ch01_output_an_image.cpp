#include "rt_math.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    int nx = 200;
    int ny = 100;

    cout << "P3\n" << nx << " " << ny << "\n255\n";
    for (int j = ny-1; j >= 0; --j)
    {
        for (int i = 0; i < nx; ++i)
        {
            rt::vec3 color( (float) i / (float) nx, (float) j / (float) ny, 0.2f );
            int ir = (int) (255.99f * color.x());
            int ig = (int) (255.99f * color.y());
            int ib = (int) (255.99f * color.z());
            cout << ir << " " << ig << " " << ib << "\n";
        }
    }

    return 0;
}
