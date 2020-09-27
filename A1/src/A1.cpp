// #define _CRT_SECURE_NO_WARNINGS
#include <cmath>
#include <cstring>
#include <iostream>
#include <tuple>

void ppmRead(char *filename, unsigned char *data, int *w, int *h) {
    char header[1024];
    FILE *fp = NULL;
    int line = 0;

    fp = fopen(filename, "rb");
    while (line < 2) {
        fgets(header, 1024, fp);
        if (header[0] != '#') {
            ++line;
        }
    }
    sscanf(header, "%d %d\n", w, h);
    fgets(header, 20, fp);
    fread(data, (*w) * (*h) * 3, 1, fp);

    fclose(fp);
}
void ppmWrite(const char *filename, unsigned char *data, int w, int h) {
    FILE *fp;
    fp = fopen(filename, "wb");

    fprintf(fp, "P6\n%d %d\n255\n", w, h);
    fwrite(data, w * h * 3, 1, fp);

    fclose(fp);
}

/* ---- Add Code Below ---- */

using u_char      = unsigned char;
const double pi   = std::acos(-1);
const int x_width = 1001, y_width = 1001;
const std::tuple<u_char, u_char, u_char> axis_color = {255, 255, 255};

inline u_char *get_pixel(u_char *const data, int x, int y) {
    int real_x = x + x_width / 2;  // L-->R
    int real_y = y_width / 2 - y;  // Top-->Bottom
    int offset = 3 * (real_y * x_width + real_x);
    return data + offset;
}

inline void draw_pixel(u_char *pos, const std::tuple<u_char, u_char, u_char> &color) {
    pos[0] = std::get<0>(color);
    pos[1] = std::get<1>(color);
    pos[2] = std::get<2>(color);
}

template <typename T>
inline void auto_swap(T &a, T &b) {
    T tmp = a;
    a     = b;
    b     = tmp;
}

inline void Bresenham(u_char *const data, const std::pair<int, int> &p1,
    const std::pair<int, int> &p2, const std::tuple<u_char, u_char, u_char> ellipse_color) {
    auto x1 = p1.first, x2 = p2.first, y1 = p1.second, y2 = p2.second;
    const auto delta_x = x2 - x1, delta_y = y2 - y1;
    draw_pixel(get_pixel(data, x1, y1), ellipse_color);
    draw_pixel(get_pixel(data, x2, y2), ellipse_color);
    if (abs(delta_x) > abs(delta_y)) {
        if (x1 > x2) {
            auto_swap(x1, x2);
            auto_swap(y1, y2);
        }
        auto P = 2 * delta_y - delta_x;
        int x = x1, y = y1;
        for (auto i = x1 + 1; i < x2; i++) {
            x += 1;
            y = P > 0 ? y + 1 : y;
            P = P > 0 ? P + (delta_y << 2) - (delta_x << 2) : P + (delta_y << 2);
            draw_pixel(get_pixel(data, x, y), ellipse_color);
        }
    } else {
        if (y1 > y2) {
            auto_swap(x1, x2);
            auto_swap(y1, y2);
        }
        auto P = 2 * delta_x - delta_y;
        int x = x1, y = y1;
        for (auto i = y1 + 1; i < y2; i++) {
            y += 1;
            x = P > 0 ? x + 1 : x;
            P = P > 0 ? P + (delta_x << 2) - (delta_y << 2) : P + (delta_x << 2);
            draw_pixel(get_pixel(data, x, y), ellipse_color);
        }
    }
}

inline std::pair<double, double> map_point(const int x, const int y, const int Ra, const int Rb,
    const double deg, const double sin_theta, const double cos_theta) {
    static const double frac = pi / 180.;
    double x_diff = Ra * cos(deg * frac), y_diff = Rb * sin(deg * frac);
    return std::make_pair(
        x + x_diff * cos_theta + y_diff * sin_theta, y + -x_diff * sin_theta + y_diff * cos_theta);
}

enum direction_type { Zero, T, B, L, R };
// enum direction_type { LT, T, RT, L, C, R, LB, B, RB };

void seed_fill(u_char *const data, const int x, const int y, direction_type dir,
    const std::tuple<u_char, u_char, u_char> &ellipse_color) {
    static const int X[] = {0, 0, -1, 1};
    static const int Y[] = {1, -1, 0, 0};
    // static const int X[] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
    // static const int Y[] = {1, 1, 1, 0, 0, 0, -1, -1, -1};

    u_char *p = get_pixel(data, x, y);
    if (p[0] == std::get<0>(ellipse_color) && p[1] == std::get<1>(ellipse_color) &&
        p[2] == std::get<2>(ellipse_color))
        return;

    draw_pixel(p, ellipse_color);

    // ppmWrite("./pic/test.ppm", data, x_width, y_width)

    switch (dir) {
        case direction_type(T):
            seed_fill(data, x, y + 1, direction_type(T), ellipse_color);
            break;
        case direction_type(B):
            seed_fill(data, x, y - 1, direction_type(B), ellipse_color);
            break;
        case direction_type(L):
            seed_fill(data, x - 1, y, direction_type(L), ellipse_color);
            seed_fill(data, x, y + 1, direction_type(T), ellipse_color);
            seed_fill(data, x, y - 1, direction_type(B), ellipse_color);
            break;
        case direction_type(R):
            seed_fill(data, x + 1, y, direction_type(R), ellipse_color);
            seed_fill(data, x, y + 1, direction_type(T), ellipse_color);
            seed_fill(data, x, y - 1, direction_type(B), ellipse_color);
            break;
        case direction_type(Zero):
            seed_fill(data, x - 1, y, direction_type(L), ellipse_color);
            seed_fill(data, x + 1, y, direction_type(R), ellipse_color);
            seed_fill(data, x, y + 1, direction_type(T), ellipse_color);
            seed_fill(data, x, y - 1, direction_type(B), ellipse_color);
            break;
        default:
            break;
    }
}

/**
 * @param ellipse_color: tuple RGB
 * @param ratio: the smaller the ratio, the higher resolution and the slower running
 */
void draw_ellipse(u_char *const data, int x, int y, int Ra, int Rb, double theta = 0,
    const std::tuple<u_char, u_char, u_char> ellipse_color = std::make_tuple(255, 255, 255),
    bool fill = false, double ratio = 5) {
    /* Same Parameter */
    const double sep       = ratio / sqrt((double)Ra * (double)Rb);  // sep in degre
    const double sin_theta = sin((180 - theta) * pi / 180.);
    const double cos_theta = cos((180 - theta) * pi / 180.);

    /* Draw ellipse*/
    for (double i = 0; i < 360; i += sep)
        Bresenham(data, map_point(x, y, Ra, Rb, i, sin_theta, cos_theta),
            map_point(x, y, Ra, Rb, i + sep, sin_theta, cos_theta), ellipse_color);

    /* Fill */
    if (fill)
        seed_fill(data, x, y, direction_type(Zero), ellipse_color);
}

int main(int argc, char const *argv[]) {
    using namespace std;

    u_char *data = new u_char[x_width * y_width * 3 * sizeof(u_char)];
    fill(data, data + x_width * y_width * 3 * sizeof(u_char), 0);

    /* Draw Axis */
    for (int i = -x_width / 2; i <= x_width / 2; i++)
        draw_pixel(get_pixel(data, i, 0), axis_color);
    for (int i = -y_width / 2; i <= y_width / 2; i++)
        draw_pixel(get_pixel(data, 0, i), axis_color);

    /* Draw Ellipses*/
    draw_ellipse(data, 173, 100, 300, 160, 30, make_tuple(255, 0, 0));
    draw_ellipse(data, -173, 100, 300, 160, 150, make_tuple(0, 255, 0));
    draw_ellipse(data, 0, -200, 300, 160, 270, make_tuple(0, 0, 255));
    draw_ellipse(data, 0, 0, 320, 320, 270, make_tuple(255, 255, 0));
    draw_ellipse(data, 400, 0, 80, 20, 0, make_tuple(0, 255, 255), true);
    draw_ellipse(data, -400, 0, 80, 20, 0, make_tuple(0, 255, 255), true);
    draw_ellipse(data, 0, 400, 80, 20, 90, make_tuple(0, 255, 255), true);
    draw_ellipse(data, 0, -400, 80, 20, 90, make_tuple(0, 255, 255), true);
    ppmWrite("./pic/test.ppm", data, x_width, y_width);

    delete[] data;
    return 0;
}
