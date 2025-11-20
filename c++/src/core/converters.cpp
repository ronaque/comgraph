#include "converters.hpp"
#include "../msg_system/error.hpp"
#include <sstream>

namespace gc
{

std::unordered_map<std::string, std::function<bool(const std::uint8_t *, size_t, size_t, size_t, const std::string &)>>
    img_type_conv({{"ppm6", save_ppm6}, {"ppm3", save_ppm3}, {"png", save_png}});

std::unordered_map<std::string, std::unordered_map<std::string, ParamParser>> attr_converter = 
{
    {"background",
        {
            {"bl", [](const char *value, gc::ParamSet &ps) { ps.assign("bl", parse_color24(value)); }},
            {"tl", [](const char *value, gc::ParamSet &ps) { ps.assign("tl", parse_color24(value)); }},
            {"tr", [](const char *value, gc::ParamSet &ps) { ps.assign("tr", parse_color24(value)); }},
            {"br", [](const char *value, gc::ParamSet &ps) { ps.assign("br", parse_color24(value)); }},
            {"color", [](const char *value, gc::ParamSet &ps) { ps.assign("color", parse_color24(value)); }},
            {"type", [](const char *value, gc::ParamSet &ps) { ps.assign("type", std::string(value)); }}
        }
    },
    {"material",
        {
            {"type", [](const char *value, gc::ParamSet &ps) { ps.assign("type", std::string(value)); }},
            {"color",[](const char *value, gc::ParamSet &ps) { ps.assign("color",  parse_spectrum(value)); }},
            {"ambient",[](const char *value, gc::ParamSet &ps) { ps.assign("ambient",  parse_spectrum(value)); }},
            {"diffuse",[](const char *value, gc::ParamSet &ps) { ps.assign("diffuse",  parse_spectrum(value)); }},
            {"specular",[](const char *value, gc::ParamSet &ps) { ps.assign("specular",  parse_spectrum(value)); }},
            {"mirror",[](const char *value, gc::ParamSet &ps) { ps.assign("mirror",  parse_spectrum(value)); }},
            {"glossiness",[](const char *value, gc::ParamSet &ps) { ps.assign("glossiness",  static_cast<real_type>(std::atof(value))); }},
        }
    },
    { "make_named_material",
        {
            {"name", [](const char *value, gc::ParamSet &ps) { ps.assign("name", std::string(value)); }},
            {"type", [](const char *value, gc::ParamSet &ps) { ps.assign("type", std::string(value)); }},
            {"color",[](const char *value, gc::ParamSet &ps) { ps.assign("color",  parse_spectrum(value)); }},
            {"ambient",[](const char *value, gc::ParamSet &ps) { ps.assign("ambient",  parse_spectrum(value)); }},
            {"diffuse",[](const char *value, gc::ParamSet &ps) { ps.assign("diffuse",  parse_spectrum(value)); }},
            {"specular",[](const char *value, gc::ParamSet &ps) { ps.assign("specular",  parse_spectrum(value)); }},
            {"mirror",[](const char *value, gc::ParamSet &ps) { ps.assign("mirror",  parse_spectrum(value)); }},
            {"glossiness",[](const char *value, gc::ParamSet &ps) { ps.assign("glossiness",  static_cast<real_type>(std::atof(value))); }},
        }
    },
    {"object",
        {
            {"type", [](const char *value, gc::ParamSet &ps) { ps.assign("type", std::string(value)); }},
            {"name", [](const char *value, gc::ParamSet &ps) { ps.assign("name", std::string(value)); }},
            /* Sphere parameters */
            {"radius", [](const char *value, gc::ParamSet &ps) { ps.assign("radius", static_cast<real_type>(std::atof(value)));}},
            {"center", [](const char *value, gc::ParamSet &ps) { ps.assign("center", parse_point3<real_type>(value)); }},
            /* Plane parameters */
            {"point", [](const char *value, gc::ParamSet &ps) { ps.assign("point", parse_point3<real_type>(value)); }},
            {"normal", [](const char *value, gc::ParamSet &ps) { ps.assign("normal", parse_vector3<real_type>(value)); }},
            /* Box parameters */
            {"min", [](const char *value, gc::ParamSet &ps) { ps.assign("min", parse_point3<real_type>(value)); }},
            {"max", [](const char *value, gc::ParamSet &ps) { ps.assign("max", parse_point3<real_type>(value)); }},
            /* Triangle mesh parameters*/
            {"ntriangles",[](const char *value, gc::ParamSet &ps) { ps.assign("ntriangles",  std::atoi(value)); }},
            {"reverse_vertex_order",[](const char *value, gc::ParamSet &ps) { ps.assign("reverse_vertex_order", parse_bool(value)); }},
            {"compute_normals",[](const char *value, gc::ParamSet &ps) { ps.assign("compute_normals", parse_bool(value)); }},
            {"backface_cull_on",[](const char *value, gc::ParamSet &ps) { ps.assign("backface_cull_on", parse_bool(value)); }},
            {"indices",[](const char *value, gc::ParamSet &ps) { ps.assign("indices", parse_vector<int>(value)); }},
            {"normals",[](const char *value, gc::ParamSet &ps) { ps.assign("normals", parse_vector_of_vector3<real_type>(value)); }},
            {"normal_indices",[](const char *value, gc::ParamSet &ps) { ps.assign("normal_indices", parse_vector<int>(value)); }},
            {"vertices",[](const char *value, gc::ParamSet &ps) { ps.assign("vertices", parse_vector_of_point3<real_type>(value)); }},
            {"uv",[](const char *value, gc::ParamSet &ps) { ps.assign("uv", parse_vector_of_vector2<real_type>(value)); }},
            {"filename", [](const char *value, gc::ParamSet &ps) { ps.assign("filename", std::string(value)); }},
        }
    },
    {"film",
        {
            {"type", [](const char *value, gc::ParamSet &ps) { ps.assign("type", std::string(value)); }},
            {"w_res", [](const char *value, gc::ParamSet &ps) { ps.assign("w_res", std::atoi(value)); }},
            {"h_res", [](const char *value, gc::ParamSet &ps) { ps.assign("h_res", std::atoi(value)); }},
            {"filename", [](const char *value, gc::ParamSet &ps) { ps.assign("filename", std::string(value)); }},
            {"img_type", [](const char *value, gc::ParamSet &ps) { ps.assign("img_type", std::string(value)); }},
            {"gamma_corrected",[](const char *value, gc::ParamSet &ps) { ps.assign("gamma_corrected", parse_bool(value)); }}
        }
    },
    {"camera",
        {
            {"type", [](const char *value, gc::ParamSet &ps) { ps.assign("type", std::string(value)); }},
            {"screen_window",
            [](const char *value, gc::ParamSet &ps) { ps.assign("screen_window", parse_screen_window(value)); }},
            {"fovy",
            [](const char *value, gc::ParamSet &ps) {
                real_type fovy = static_cast<real_type>(std::atof(value));
                TRACE(FORMAT("Parsed fovy: ", fovy));
                ps.assign("fovy", fovy);
            }},
        }
    },
    {"lookat",
        {
            {"look_from", [](const char *value, gc::ParamSet &ps) { ps.assign("look_from", parse_point3<real_type>(value)); }},
            {"look_at", [](const char *value, gc::ParamSet &ps) { ps.assign("look_at", parse_point3<real_type>(value)); }},
            {"up", [](const char *value, gc::ParamSet &ps) { ps.assign("up", parse_vector3<real_type>(value)); }},
        }
    },
    {"integrator",
        {
            {"type", [](const char *value, gc::ParamSet &ps) { ps.assign("type", std::string(value)); }},
        }
    },
    {"light_source",
        {
            {"type", [](const char *value, gc::ParamSet &ps) { ps.assign("type", std::string(value)); }},
            {"I", [](const char *value, gc::ParamSet &ps) { ps.assign("I", parse_spectrum(value)); }},
            {"scale", [](const char *value, gc::ParamSet &ps) { ps.assign("scale", parse_spectrum(value)); }},
            {"from", [](const char *value, gc::ParamSet &ps) { ps.assign("from", parse_point3<real_type>(value)); }},
            {"to", [](const char *value, gc::ParamSet &ps) { ps.assign("to", parse_point3<real_type>(value)); }},
            /** Spot light params */
            {"cutoff",[](const char *value, gc::ParamSet &ps) { ps.assign("cutoff",  static_cast<real_type>(std::atof(value))); }},
            {"falloff",[](const char *value, gc::ParamSet &ps) { ps.assign("falloff",  static_cast<real_type>(std::atof(value))); }},
        }
    },
    {"include", 
    {
        {"filename", [](const char *value, gc::ParamSet &ps) { ps.assign("filename", std::string(value)); }},
    }}
};

bool parse_bool(const std::string &str)
{
    std::string lowered = str;
    std::transform(lowered.begin(), lowered.end(), lowered.begin(), ::tolower);

    if (lowered == "true")
        return true;
    if (lowered == "false")
        return false;

    throw std::invalid_argument("Invalid boolean string: " + str);
}

Color24 parse_color24(const char *val)
{
    Vector3<int> vec = parse_vector3<int>(val);
    if (vec.x < 0 || vec.x > 255 || vec.y < 0 || vec.y > 255 || vec.z < 0 || vec.z > 255)
    {
        throw std::runtime_error("Color values must be between 0 and 255");
    }
    return {static_cast<unsigned char>(vec.x), static_cast<unsigned char>(vec.y), static_cast<unsigned char>(vec.z)};
}

Spectrum parse_spectrum(const char *val)
{
    Vector3<real_type> vec = parse_vector3<real_type>(val);
    if (vec.x > 1 || vec.y > 1 || vec.z > 1)
    {
        vec.x = vec.x / 255.f;
        vec.y = vec.y / 255.f;
        vec.z = vec.z / 255.f;
    }

    return {vec.x, vec.y, vec.z};
}

template <typename T> Point3<T> parse_point3(const char *val)
{
    Vector3<T> vec = parse_vector3<T>(val);
    return {vec.x, vec.y, vec.z};
}

template <typename T> Vector3<T> parse_vector3(const char *val)
{
    TRACE(FORMAT("Parsing Vector3 from: ", val));
    std::istringstream iss(val);
    T x_int, y_int, z_int;
    iss >> x_int >> y_int >> z_int;
    TRACE(FORMAT("Parsed x: ", x_int, " y: ", y_int, " z: ", z_int));
    return Vector3<T>{static_cast<T>(x_int), static_cast<T>(y_int), static_cast<T>(z_int)};
}

template <typename T>
std::vector<Point3<T>> parse_vector_of_point3(const char *val)
{
    std::vector<Point3<T>> result_vector;

    std::istringstream iss(val);

    T x, y, z;

    while (iss >> x >> y >> z)
    {
        result_vector.emplace_back(x, y, z);
    }

    return result_vector;
}

template <typename T>
std::vector<T> parse_vector(const char *val)
{
    std::vector<T> result_vector;

    std::istringstream iss(val);

    T parsed;

    while (iss >> parsed)
    {
        result_vector.push_back(parsed);
    }

    return result_vector;
}


template <typename T>
std::vector<Vector3<T>> parse_vector_of_vector3(const char *val)
{
    std::vector<Vector3<T>> result_vector;

    std::istringstream iss(val);

    T x, y, z;

    while (iss >> x >> y >> z)
    {
        result_vector.emplace_back(x, y, z);
    }

    return result_vector;
}

template <typename T>
std::vector<Vector2<T>> parse_vector_of_vector2(const char *val)
{
    std::vector<Vector2<T>> result_vector;

    std::istringstream iss(val);

    T x, y;

    while (iss >> x >> y)
    {
        result_vector.emplace_back(x, y);
    }

    return result_vector;
}

std::vector<real_type> parse_screen_window(const char *val)
{
    std::istringstream iss(val);
    real_type l, r, b, t;
    iss >> l >> r >> b >> t;
    return {l, r, b, t};
}
} // namespace gc
