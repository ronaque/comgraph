#ifndef _CONVERTERS_
#define _CONVERTERS_

#include <functional>
#include <unordered_map>
#include <utility>
#include <vector>

#include "app.hpp"
#include "common.hpp"
#include "image_io.hpp"
#include "paramset.hpp"

namespace gc
{
enum ATTR_TYPE
{
    STRING,
    INT,
    BOOL,
    COLOR24
};

enum FILM_TYPE
{
    IMAGE
};

enum FILM_IMG_TYPE
{
    PPM6,
    PPM3,
    PNG
};

enum INTEGRATOR_TYPE
{
    FLAT
};

extern std::unordered_map<std::string,
                          std::function<bool(const std::uint8_t *, size_t, size_t, size_t, const std::string &)>>
    img_type_conv;

// Ponteiro para função void recebendo valor como char* e ponteiro para um ParamSet para que possa ser manuseado
using ParamParser = std::function<void(const char *, ParamSet &)>;

// Dicionário de atributos de cada nó, com uma função de conversão para cada
extern std::unordered_map<std::string, std::unordered_map<std::string, ParamParser>> attr_converter;

int parse_film_img_type(const std::string &type);
int parse_film_type(const std::string &type);
Color24 parse_color24(const char *val);
Spectrum parse_spectrum(const char *val);
bool parse_bool(const std::string &str);
std::vector<float> parse_vec_float(const char *val);
std::vector<float> parse_screen_window(const char *val);
template <typename T> Point3<T> parse_point3(const char *val);
template <typename T> Vector3<T> parse_vector3(const char *val);
template <typename T> std::vector<T> parse_vector(const char *val);
template <typename T> std::vector<Point3<T>> parse_vector_of_point3(const char *val);
template <typename T> std::vector<Vector3<T>> parse_vector_of_vector3(const char *val);
template <typename T> std::vector<Vector2<T>> parse_vector_of_vector2(const char *val);
} // namespace gc

#endif // _CONVERTERS_