#include <iostream>
#include <fstream>
#include <cmath>

#define PI 3.141592654

using namespace std;

class RayCaster {
    private:

    int map_size;
    int unit_box;
    int world_size;

    int **map;
    float **sdf;

    float player_gaze;
    int player_x, player_y;

    public:

    RayCaster(const char* filepath) {
        fstream file(filepath);

        file >> map_size >> unit_box;
        
        map = new int *[map_size];
        for(int i = 0; i < map_size; i++)
            map[i] = new int[map_size];
        
        for (int n = 0; n < map_size * map_size; ++n)
        {
            int i = n % map_size;
            int j = n / map_size;

            file >> map[i][j];
        }

        world_size = map_size * unit_box;

        sdf = new float *[world_size];
        for(int i = 0; i < world_size; i++)
            sdf[i] = new float[world_size]; 

        cout << "Map Generation, Please Wait... \n"; 

        generate_sdf();

        player_x = world_size / 2;
        player_y = world_size / 2;
        player_gaze = PI / 2;

    }

    void generate_sdf() {

        for (int point = 0; point < world_size * world_size; ++point) {
            int x = point % world_size;
            int y = point / world_size;

            bool found_box = false;

            for (int box = 0; box < map_size * map_size; ++box) {
                int i = box % map_size;
                int j = box / map_size;

                if (map[i][j] == 0)
                    continue;
                
                float distance;

                if (map[i][j] == 2)
                    distance = circle_sdf(i, j, x, y);
                else
                    distance = box_sdf(i, j, x, y);

                if (!found_box) {
                    found_box = true;
                    sdf[x][y] = distance;
                    continue;
                }

                if (sdf[x][y] > distance)
                    sdf[x][y] = distance; 
            }
        }
    }

    float box_sdf(int i, int j, int x, int y) {
        float box_x = i * unit_box + 0.5 * unit_box;
        float box_y = j * unit_box + 0.5 * unit_box;

        float dx = fabs(box_x - x) - 0.5 * unit_box;
        if (dx < 0)
            dx = 0;

        float dy = fabs(box_y - y) - 0.5 * unit_box;
        if (dy < 0)
            dy = 0;

        return sqrtf(dx * dx + dy * dy);
    }

    float circle_sdf(int i, int j, int x, int y) {
        float circle_x = i * unit_box + 0.5 * unit_box;
        float circle_y = j * unit_box + 0.5 * unit_box;

        float dx = circle_x - x;
        float dy = circle_y - y;

        return sqrtf(dx * dx + dy * dy) - 0.5 * unit_box;
    }

    void ray_cast(float ray_angle, float &perp_distance) {
        float ray_x = player_x;
        float ray_y = player_y;

        float radian_angle = PI * ray_angle / 180;

        while (sdf[int(ray_x)][int(ray_y)] > 0) {

            float coef = sqrtf(sdf[int(ray_x)][int(ray_y)]);

            ray_x += sdf[int(ray_x)][int(ray_y)] * cosf(player_gaze + radian_angle) / coef;
            ray_y += sdf[int(ray_x)][int(ray_y)] * sinf(player_gaze + radian_angle) / coef;
        }

        float dx = player_x - ray_x;
        float dy = player_y - ray_y;

        perp_distance = sqrtf(dx * dx + dy * dy) * cosf(radian_angle) / unit_box;
    }
    
    void move_player(int step) {
        int new_x = player_x + int(step * cosf(player_gaze));
        int new_y = player_y + int(step * sinf(player_gaze));

        if (sdf[new_x][new_y] > 0)
        {
            player_x = new_x;
            player_y = new_y;
        }
    }

    void rotate_player(float alpha) {
        player_gaze += PI * alpha / 180;
    }
};