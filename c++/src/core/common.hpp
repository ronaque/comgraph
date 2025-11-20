#ifndef COMMON_HPP
#define COMMON_HPP

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>

// #include <glm/ext/vector_float3.hpp>
// #include <glm/ext/vector_int3.hpp>

namespace gc
{

//== Global Forward Declarations
class AmbientLight;
class AreaLight;
class Camera;
class DirectionalLight;
class Film;
class FlatMaterial;
class FlatShader;
class Integrator;
class Light;
class Material;
class OrthographicCamera;
class ParamSet;
class PerspectiveCamera;
class PointLight;
class Primitive;
class Ray;
class Scene;
class Shape;
class Sphere;
class Surfel;
class Transform;
class WhittedShader;

//=== aliases
// Forward declarations
template <typename T> class Vector3
{
  public:
    T x;
    T y;
    T z;

    T length_squared() const
    {
        return x * x + y * y + z * z;
    }

    T length() const
    {
        return std::sqrt(length_squared());
    }

    Vector3<T> normalize() const
    {
        T norm = std::sqrt(x * x + y * y + z * z);
        if (norm == 0)
            return Vector3<T>{0, 0, 0};
        return Vector3<T>{x / norm, y / norm, z / norm};
    }

    std::string to_string() const
    {
        std::stringstream ss;
        ss << "Vec3(";

        if constexpr (std::is_integral<T>::value && sizeof(T) == 1)
        {
            ss << "x=" << static_cast<int>(x) << ", "
               << "y=" << static_cast<int>(y) << ", "
               << "z=" << static_cast<int>(z);
        }
        else
        {
            ss << "x=" << x << ", "
               << "y=" << y << ", "
               << "z=" << z;
        }

        ss << ")";
        return ss.str();
    }

    Vector3<T> operator*(const Vector3<T> &v) const
    {
        return Vector3{x * v.x, y * v.y, z * v.z};
    }

    T magnitude() const
    {
        return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
    }

    Vector3(T x_val, T y_val, T z_val) : x(x_val), y(y_val), z(z_val)
    {
    }
    Vector3() = default;

    Vector3<T> operator*(T scalar) const
    {
        return Vector3<T>{x * scalar, y * scalar, z * scalar};
    }
};

template <typename T> class Vector2
{
  public:
    T x;
    T y;

    Vector2() = default;
    Vector2(T x_val, T y_val) : x(x_val), y(y_val)
    {
    }
};

template <typename T> struct Point2 : public Vector2<T>
{
  public:
    using Vector2<T>::Vector2;
};

template <typename T> class Point3 : public Vector3<T>
{
  public:
    using Vector3<T>::Vector3;
};

using real_type = float;
using size_type = size_t;
using byte = std::uint8_t;
using Color24 = Vector3<byte>;
using Spectrum = Vector3<real_type>;
using Offset3f = Vector3<real_type>;
using Radiance3f = Vector3<real_type>;
using Biradiance3f = Vector3<real_type>;
using Power3f = Vector3<real_type>;

template <typename T> inline std::ostream &operator<<(std::ostream &os, const Vector3<T> &vec)
{
    os << "(" << vec.x << "," << vec.y << "," << vec.z << ")";
    return os;
}

inline std::ostream &operator<<(std::ostream &os, const Color24 &vec)
{
    os << "(" << static_cast<int>(vec.x) << "," << static_cast<int>(vec.y) << "," << static_cast<int>(vec.z) << ")";
    return os;
}

template <typename T> bool operator==(const Vector3<T> &a, const Vector3<T> &b)
{
    if (a.x == b.x && a.y == b.y && a.z == b.z)
        return true;

    return false;
}

template <typename T> Vector3<T> operator-(const Vector3<T> &a, const Vector3<T> &b)
{
    return Vector3<T>{a.x - b.x, a.y - b.y, a.z - b.z};
}
template <typename T> Vector3<T> operator-(const Vector3<T> &a)
{
    return Vector3<T>{-a.x, -a.y, -a.z};
}

template <typename T> Vector3<T> operator+(const Vector3<T> &a, const Vector3<T> &b)
{
    return Vector3<T>{a.x + b.x, a.y + b.y, a.z + b.z};
}

template <typename T> Point3<T> operator+(const Point3<T> &a, const Vector3<T> &b)
{
    return Point3<T>{a.x + b.x, a.y + b.y, a.z + b.z};
}

template <typename T> Vector3<T> operator*(const real_type &b, const Vector3<T> &a)
{
    return Vector3<T>{a.x * b, a.y * b, a.z * b};
}

template <typename T> Vector3<T> operator/(const Vector3<T> &a, const real_type &b)
{
    return Vector3<T>{a.x / b, a.y / b, a.z / b};
}

template <class T> std::ostream &operator<<(std::ostream &os, const std::vector<T> &v)
{
    os << "[";
    for (typename std::vector<T>::const_iterator ii = v.begin(); ii != v.end(); ++ii)
    {
        os << " " << *ii;
    }
    os << "]";
    return os;
}

template <typename T> Vector3<T> cross(const Vector3<T> &a, const Vector3<T> &b)
{
    return Vector3<T>{a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

template <typename T> T dot(const Vector3<T> &a, const Vector3<T> &b)
{
    return T{a.x * b.x + a.y * b.y + a.z * b.z};
}

#ifdef M_PI
#undef M_PI
#endif
#define M_PI 3.14159265358979323846f
#define INV_PI 0.31830988618379067154f
#define INV_TWOPI 0.15915494309189533577f
#define INV_FOURPI 0.07957747154594766788f
#ifndef INFINITY
#define INFINITY FLT_MAX
#endif

static constexpr real_type machine_epsilon = std::numeric_limits<gc::real_type>::epsilon() * 0.5;
static constexpr real_type shadow_epsilon = 0.001F;
static constexpr real_type pi = 3.14159265358979323846;
static constexpr real_type two_pi = 2 * pi;
static constexpr real_type inv_pi = 0.31830988618379067154;
static constexpr real_type inv2_pi = 0.15915494309189533577;
static constexpr real_type inv4_pi = 0.07957747154594766788;
static constexpr real_type pi_over2 = 1.57079632679489661923;
static constexpr real_type pi_over4 = 0.78539816339744830961;
static constexpr real_type Sqrt2 = 1.41421356237309504880;

template <typename T> static T clamp(T field, T min, T max)
{
    if (field < min)
        return min;

    if (field > max)
        return max;

    return field;
}

// Add here Option structure.
struct RunningOptions
{
    RunningOptions(std::string filename = "", std::string outfile = "")
        : filename(std::move(filename)), outfile(std::move(outfile))
    {
    }
    // -----------------------------------------
    // [ x0, x1 ] -> X values
    // [ y0, y1 ] -> Y values
    // where x0,x1,y0,y1 in [0.0, 1.0].
    // 1 = 100% of the full resolution.
    // -----------------------------------------
    /// Crop window to render.

    std::array<float, 4> crop_window{0, 1, 0, 1};
    std::string filename;     //!< input scene file name.
    std::string outfile;      //!< output image file name.
    bool quick_render{false}; //!< when set, render image with 1/4 of the requested resolition.
};

inline std::ostream &operator<<(std::ostream &os, const RunningOptions &opts)
{
    os << "RunningOptions { "
       << "filename=\"" << opts.filename << "\", "
       << "outfile=\"" << opts.outfile << "\", "
       << "quick_render=" << (opts.quick_render ? "true" : "false") << ", "
       << "crop_window=[" << opts.crop_window[0] << ", " << opts.crop_window[1] << ", " << opts.crop_window[2] << ", "
       << opts.crop_window[3] << "]"
       << " }";
    return os;
}

/// Lambda expression that returns a lowercase version of the input string.
inline auto str_lowercase = [](std::string str) -> std::string {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
};

inline void progress_bar(const int percentage)
{
    const size_t MAX = 80;
    std::stringstream ss;
    ss << "[";

    size_t filled = (MAX * percentage) / 100;
    size_t empty = MAX - filled;

    for (size_t i = 0; i < filled; i++)
        ss << "=";

    if (filled < MAX)
        ss << ">";

    for (size_t i = 0; i < empty - (filled < MAX ? 1 : 0); i++)
        ss << " ";

    ss << "] " << percentage << "%\r";

    std::cout << ss.str();
    std::cout.flush();

    if (percentage >= 100)
    {
        std::cout << std::endl; // Finaliza com quebra de linha ao terminar
    }
}

} // namespace gc

#endif // !COMMON_HPP
