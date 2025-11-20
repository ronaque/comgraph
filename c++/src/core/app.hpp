#ifndef _APP_
#define _APP_

#include "../cameras/camera.hpp"
#include "../light/lights.hpp"
#include "../primitive/material.hpp"
#include "../primitive/primitive.hpp"
#include "background.hpp"
#include "builder.hpp"
#include "common.hpp"
#include "scene.hpp"
#include <array>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

extern std::string str_lowercase;

namespace gc
{
enum app_state
{
    UNITIALIZED,
    SETUP,
    RENDER
};

typedef struct
{
    std::vector<std::shared_ptr<Material>> materials;
    std::shared_ptr<Material> current_material;
} GraphicsState;

typedef struct
{
    std::vector<std::shared_ptr<Light>> lights;
    std::unordered_map<std::string, ParamSet> actors;
    std::vector<std::shared_ptr<Primitive>> primitives;
} RenderOptions;

class App
{
  public:
    static RenderOptions render_options;
    static GraphicsState graphics_state;
    static app_state state;
    static RunningOptions options;
    static Scene scene;
    static void init_engine(RunningOptions ro);
    static void run();
    static void clean_up();
    static std::pair<int, int> get_cropped_width_and_height(size_t width, size_t height);
    static std::array<int, 4> get_cropped_values(size_t width, size_t height);

  private:
    static std::shared_ptr<Material> find_material(std::string name);
    static void save_material(const bool isNamed, const ParamSet &params);
    static void save_primitive(const ParamSet &params);
    static void save_light(const ParamSet &params);
    static void init_scene();
    static std::shared_ptr<Film> init_film();
    static std::shared_ptr<Camera> init_camera();
    static std::shared_ptr<LookAt> init_look_at();
    static std::shared_ptr<Background> init_background(const Film &film);
    static void parse_file(const std::string &file_name);
    static void render();
};
} // namespace gc

#endif