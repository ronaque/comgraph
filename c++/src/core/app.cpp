#include "app.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "../cameras/orthographic.hpp"
#include "../cameras/perspective.hpp"
#include "../integrators/integrators.hpp"
#include "../msg_system/error.hpp"
#include "../primitive/shape.hpp"
#include "converters.hpp"
#include "film.hpp"
#include "paramset.hpp"

#include "tinyxml2.h"

std::string str_lowercase;

namespace gc
{
Scene App::scene;
app_state App::state;
RunningOptions App::options;
GraphicsState App::graphics_state;
RenderOptions App::render_options;

std::array<int, 4> App::get_cropped_values(size_t width, size_t height)
{
    RunningOptions options = App::options;
    int x_0 = options.crop_window[0] * width;
    int x_1 = options.crop_window[1] * width;
    int y_0 = options.crop_window[2] * height;
    int y_1 = options.crop_window[3] * height;
    return {x_0, x_1, y_0, y_1};
}

std::pair<int, int> App::get_cropped_width_and_height(size_t width, size_t height)
{
    auto [x_0, x_1, y_0, y_1] = get_cropped_values(width, height);
    return std::pair{x_1 - x_0, y_1 - y_0};
}

std::shared_ptr<Material> App::find_material(std::string name)
{
    if (name == "none")
    {
        DEBUG(FORMAT("Name is none, returning current_mateiral: ", graphics_state.current_material->to_string()));
        return graphics_state.current_material;
    }
    else
    {
        if (graphics_state.current_material->name == name)
        {
            DEBUG(FORMAT("Current material is named, returning current_mateiral: ",
                         graphics_state.current_material->to_string()));
            return graphics_state.current_material;
        }

        for (std::shared_ptr<Material> material : graphics_state.materials)
        {
            if (material->name == name)
            {
                DEBUG(FORMAT("Found name in graphics materials: ", material->to_string()));
                return material;
            }
        }

        ERROR("Tried to apply a named material that don't exist");
    }

    return nullptr;
};

void App::save_material(const bool isNamed, const ParamSet &params)
{
    std::string material_type = params.retrieve<std::string>("type", "flat");
    TRACE(FORMAT("Material type being processed: '", material_type, "'"));
    std::shared_ptr<Material> new_material_ptr = nullptr;

    if (material_type == "flat")
    {
        new_material_ptr = Builder::build_flat_material(params, isNamed);
    }
    else if (material_type == "blinn")
    {
        new_material_ptr = Builder::build_blinn_material(params, isNamed);
    }

    if (new_material_ptr)
    {
        graphics_state.materials.push_back(new_material_ptr);
        graphics_state.current_material = new_material_ptr;
        DEBUG(FORMAT("Saved material: ", graphics_state.current_material->to_string()));
    }
    else
    {
        TRACE("Failed to create a material");
    }
}

void App::save_primitive(const ParamSet &params)
{
    std::string primitive_type = params.retrieve<std::string>("type", "sphere");
    std::string material_name = params.retrieve<std::string>("name", "none");
    std::shared_ptr<Material> material = find_material(material_name);
    std::shared_ptr<Shape> primitive_shape;
    std::shared_ptr<Primitive> primitive;

    if (primitive_type == "sphere")
    {
        primitive_shape = Builder::build_sphere(params);
        primitive = std::make_shared<GeometricPrimitive>(material, primitive_shape);
    }
    else if (primitive_type == "box")
    {
        primitive_shape = Builder::build_box(params);
        primitive = std::make_shared<GeometricPrimitive>(material, primitive_shape);
    }
    else if (primitive_type == "plane")
    {
        primitive_shape = Builder::build_plane(params);
        primitive = std::make_shared<GeometricPrimitive>(material, primitive_shape);
    }
    else if (primitive_type == "trianglemesh")
    {
        // TODO: refator to return vector of shapes
        std::vector<std::shared_ptr<Shape>> triangles_shapes = Builder::build_triangle_mesh(false, params);

        // Build List of primitives for each shape
        std::vector<std::shared_ptr<GeometricPrimitive>> triangle_primitives;
        triangle_primitives.reserve(triangles_shapes.size());
        for (size_t i = 0; i < triangles_shapes.size(); i++)
        {
            std::shared_ptr<Primitive> triangle_primitive =
                std::make_shared<GeometricPrimitive>(material, triangles_shapes[i]);
            DEBUG(FORMAT("Saved Primitive: ", triangle_primitive->to_string()));
            render_options.primitives.push_back(triangle_primitive);
        }

        // Move to PrimList
        // primitive = std::make_shared<PrimList>(triangle_primitives);
    }
    else
        ERROR("Invalid object type");

    if (primitive_type != "trianglemesh")
    {
        DEBUG(FORMAT("Saved Primitive: ", primitive->to_string()));
        render_options.primitives.push_back(primitive);
    }
}

void App::save_light(const ParamSet &params)
{
    std::string light_type = params.retrieve<std::string>("type", "ambient");

    if (light_type == "ambient")
    {
        std::shared_ptr<AmbientLight> ambient = Builder::build_ambient_light(params);

        DEBUG(FORMAT("Saved Light: ", ambient->to_string()));
        render_options.lights.push_back(ambient);
    }
    else if (light_type == "directional")
    {
        std::shared_ptr<DirectionalLight> directional = Builder::build_directional_light(params);

        DEBUG(FORMAT("Saved Light: ", directional->to_string()));
        render_options.lights.push_back(directional);
    }
    else if (light_type == "point")
    {
        std::shared_ptr<PointLight> point = Builder::build_point_light(params);

        DEBUG(FORMAT("Saved Light: ", point->to_string()));
        render_options.lights.push_back(point);
    }
    else if (light_type == "spot")
    {
        std::shared_ptr<SpotLight> spot = Builder::build_spot_light(params);

        DEBUG(FORMAT("Saved Light: ", spot->to_string()));
        render_options.lights.push_back(spot);
    }
}

std::shared_ptr<Background> App::init_background(const Film &film)
{
    TRACE("Initializing Background");
    if (render_options.actors.find("background") == render_options.actors.end())
        ERROR(FORMAT("Background node isn't present"));

    Background background = Background();

    background.process_background(render_options.actors["background"]);

    return std::make_shared<Background>(background);
}

void App::init_scene()
{
    std::shared_ptr<Camera> camera = init_camera();
    std::shared_ptr<Background> background = init_background(camera->film);

    // Get aggregator

    // If type = list
    std::shared_ptr<AggregatePrimitive> prim_list = std::make_shared<PrimList>(render_options.primitives);
    scene = Scene(prim_list, camera, background, render_options.lights);

    // If type = bhv
    // Build bhv tree

}

std::shared_ptr<Film> App::init_film()
{
    TRACE("Initializing film");

    if (render_options.actors.find("film") == render_options.actors.end())
        ERROR(FORMAT("Film node isn't present"));

    Film film = Film();

    film.process_film(render_options.actors["film"], options.quick_render);

    return std::make_shared<Film>(film);
}

std::shared_ptr<LookAt> App::init_look_at()
{
    TRACE("Initializing look at");

    if (render_options.actors.find("lookat") == render_options.actors.end())
        ERROR(FORMAT("Look at node isn't present"));

    LookAt look_at = LookAt();

    look_at.process_look_at(render_options.actors["lookat"]);

    return std::make_shared<LookAt>(look_at);
}

std::shared_ptr<Camera> App::init_camera()
{
    TRACE("Initializing camera");
    std::shared_ptr<Film> film = init_film();
    std::shared_ptr<LookAt> look_at = init_look_at();

    if (render_options.actors.find("camera") == render_options.actors.end())
        ERROR(FORMAT("Camera node isn't present"));

    ParamSet params = render_options.actors["camera"];

    std::string type = params.retrieve<std::string>("type", "orthographic");

    TRACE(FORMAT("Camera type: ", type));

    if (type == "perspective")
    {
        float fovy = params.retrieve<float>("fovy", 30.0);
        TRACE(FORMAT("Camera fovy: ", fovy));

        auto camera = std::make_shared<PerspectiveCamera>(fovy, film.get(), look_at.get());

        DEBUG(FORMAT("Processed Perspective camera: ", camera->to_string()));
        return camera;
    }
    else if (type == "orthographic")
    {
        std::vector<float> screen_window = params.retrieve<std::vector<float>>("screen_window", {0, 0, 0, 0});
        TRACE(
            FORMAT("Camera screen window: (", screen_window[0], screen_window[1], screen_window[2], screen_window[3]));

        auto camera = std::make_shared<OrthographicCamera>(screen_window[0], screen_window[1], screen_window[2],
                                                           screen_window[3], film.get(), look_at.get());

        DEBUG(FORMAT("Processed Ortographic camera: ", camera->to_string()));
        return camera;
    }

    ERROR("Couldn't initialize camera");
    return std::shared_ptr<Camera>();
}

void App::parse_file(const std::string &file_name)
{
    tinyxml2::XMLDocument doc;

    if (doc.LoadFile(file_name.c_str()) != tinyxml2::XML_SUCCESS)
    {
        std::cerr << "Error loading XML file: " << file_name << std::endl;
        return;
    }

    tinyxml2::XMLElement *root = doc.RootElement();
    if (!root)
    {
        std::cerr << "Error: Root element not found!" << std::endl;
        return;
    }

    std::shared_ptr<Material> current_material;

    // Percorre as tags no XML
    for (tinyxml2::XMLElement *element = root->FirstChildElement(); element != nullptr;
         element = element->NextSiblingElement())
    {
        std::string node_name = element->Name();
        ParamSet params;

        // Para cada atributo esperado para o nó, tenta fazer o parser de seu valor para o tipo esperado
        auto it = attr_converter.find(node_name);
        if (it != attr_converter.end())
        {
            DEBUG(FORMAT("Parsing node: ", node_name));
            for (const auto &[attr, parser_fn] : it->second)
            {
                TRACE(FORMAT("Parsing attribute: ", attr));
                const char *val = element->Attribute(attr.c_str());
                if (val)
                {
                    parser_fn(val, params);
                }
            }
        }
        else
        {
            WARNING(FORMAT("No attributes for node: ", node_name));
        }

        if (node_name == "material")
        {
            save_material(false, params);
        }
        else if (node_name == "make_named_material")
        {
            save_material(true, params);
        }
        else if (node_name == "object")
        {
            save_primitive(params);
        }
        else if (node_name == "light_source")
        {
            save_light(params);
        }
        else if (node_name == "world_end")
        {
            render();
        }
        else if (node_name == "include")
        {
            parse_file(params.retrieve<std::string>("filename", ""));
        }
        else
        {
            render_options.actors[node_name] = params;
        }
    }
}

void App::render()
{
    // Cria a cena com objetos
    init_scene();

    if (render_options.actors.find("integrator") == render_options.actors.end())
        ERROR(FORMAT("Integrator node isn't present"));

    DEBUG("Renderizando cena");
    ParamSet params = render_options.actors["integrator"];

    std::string integrator_type = params.retrieve<std::string>("type", "flat");

    std::shared_ptr<std::vector<byte>> pixels_output;
    if (integrator_type == "flat")
    {
        RayCastIntegrator integrator;

        pixels_output = integrator.render(scene);
    }
    else if (integrator_type == "blinn_phong")
    {
        BlinnPhongIntegrator integrator;
        pixels_output = integrator.render(scene);
    }

    // Salva a imagem
    std::string filename = !options.outfile.empty() ? options.outfile : scene.camera->film.filename;
    DEBUG(FORMAT("Salvando imagem renderizada em: ", filename));
    auto it = img_type_conv.find(scene.camera->film.img_type);
    if (it != img_type_conv.end())
    {
        auto [width, height] = get_cropped_width_and_height(scene.camera->film.width, scene.camera->film.height);
        it->second(pixels_output->data(), width, height, 3, filename);
    }
    else
    {
        ERROR("Unsupported image format: " + scene.camera->film.img_type);
    }
}

void App::init_engine(RunningOptions ro)
{
    if (state == UNITIALIZED)
    {
        options = ro;
        state = SETUP;
    }
    else
    {
        ERROR("Error initializing the engine");
    }

    MESSAGE(FORMAT("Initializing engine with options: ", static_cast<const gc::RunningOptions &>(ro)));
}

void App::run()
{
    std::ostringstream oss;
    oss << "Running the API...";

    MESSAGE(std::string_view(oss.str()));
    // Aqui chamamos parse passando o nome do arquivo XML
    parse_file(options.filename);
}

void App::clean_up()
{
    std::cout << "Cleaning up resources..." << std::endl;
}

} // namespace gc
