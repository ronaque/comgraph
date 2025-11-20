#include "builder.hpp"
#include "../../ext/tiny_obj_loader.h"
#include <chrono>
#include <iterator>
#include <ostream>

namespace gc
{

void extract_obj_data(const tinyobj::attrib_t &attrib, const std::vector<tinyobj::shape_t> &shapes, bool reverse_order,
                      bool compute_normals, bool flip_normals,
                      /* OUT */ std::shared_ptr<TriangleMesh> mesh);

/** ------------------------------ Aux functions ------------------------------ */
/// This function calls the basic tinyobjloader loading function and stores all
/// the data into the tinyobjloader's internal data structures.
bool load_mesh_data(const std::string &filename, bool rvo, bool compute_normals, bool flip_normals,
                    std::shared_ptr<TriangleMesh> mesh)
{
    // Default load parameters
    const char *basepath = nullptr;
    bool triangulate = true;

    std::cout << "Loading " << filename << '\n';

    // This is the tinyobj internal data structures.
    tinyobj::attrib_t attrib;                   // Hold all the data, such as vertex, normals, UV coords
    std::vector<tinyobj::shape_t> shapes;       // Represents a collection of faces, grouped in a unit.
    std::vector<tinyobj::material_t> materials; // Materials, NOT USED HERE.

    // Warnings and Error values.
    std::string warn;
    std::string err;

    // Timing loading.
    //================================================================================
    auto start = std::chrono::steady_clock::now();
    //================================================================================
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str(), basepath, triangulate);
    //================================================================================
    std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
    // Store the time difference between start and end
    std::chrono::duration<double> diff = end - start;

    // Milliseconds (10^-3)
    std::cout << "\t\t>>> " << std::chrono::duration<double, std::milli>(diff).count() << " ms" << '\n';

    // Nanoseconds (10^-9)
    std::cout << "\t\t>>> " << std::chrono::duration<double, std::nano>(diff).count() << " ns" << '\n';

    // Seconds
    auto diff_sec = std::chrono::duration_cast<std::chrono::seconds>(diff);
    std::cout << "\t\t>>> " << diff_sec.count() << " s" << '\n';
    //================================================================================

    if (!warn.empty())
    {
        std::cout << "WARN: " << warn << '\n';
    }

    if (!err.empty())
    {
        std::cerr << "ERR: " << err << '\n';
    }

    if (!ret)
    {
        std::cout << "Failed to load/parse .obj.\n";
        return false;
    }

    // Let us now "convert" or "migrate" the data from tinyobjloader data
    // structure into our mesh data format.
    extract_obj_data(attrib,
                     shapes, // TinyObjeLoader data structures (IN)
                     rvo, compute_normals,
                     flip_normals, // Mesh modifiers (IN)
                     mesh);        // Reference to the mesh data to fill in. (OUT)

    return true;
}

/**
 * This is the function that converts from the tinyobjloader's internal storage
 * to our mesh data organization.
 */
void extract_obj_data(const tinyobj::attrib_t &attrib, const std::vector<tinyobj::shape_t> &shapes, bool reverse_order,
                      bool compute_normal, bool flip_normals,
                      /* OUT */ std::shared_ptr<TriangleMesh> mesh)
{

    // Retrieve the complete list of vertices.
    auto n_vertices{attrib.vertices.size() / 3};
    for (auto idx_v{0U}; idx_v < n_vertices; idx_v++)
    {

        // Store the vertex in the mesh data structure.
        mesh->vertices.push_back(Point3<real_type>{attrib.vertices[(3 * idx_v) + 0], attrib.vertices[(3 * idx_v) + 1],
                                                   attrib.vertices[(3 * idx_v) + 2]});
    }

    // Read the normals
    // TODO: flip normals if requested.
    real_type flip = (flip_normals) ? -1 : 1;
    auto n_normals{attrib.normals.size() / 3};

    // Do we need to compute the normals? Yes only if the user requeste or there
    // are no normals in the file.
    // if (compute_normal == true or n_normals == 0)
    // {
    //     // TODO: COmpute normals here.
    //     compute_normals(mesh);
    // }
    // else {// Read normals from file. This corresponds to the entire 'for'
    // below.
    // else
    // {
    // Traverse the normals read from the OBJ file.
    for (auto idx_n{0U}; idx_n < n_normals; idx_n++)
    {

        // Store the normal.
        mesh->normals.push_back(Vector3<real_type>{attrib.normals[(3 * idx_n) + 0] * flip,
                                                   attrib.normals[(3 * idx_n) + 1] * flip,
                                                   attrib.normals[(3 * idx_n) + 2] * flip});
    }
    // }

    // Read the complete list of texture coordinates.
    auto n_texcoords{attrib.texcoords.size() / 2};
    for (auto idx_tc{0U}; idx_tc < n_texcoords; idx_tc++)
    {

        // Store the texture coords.
        mesh->uvcoords.push_back(
            Point2<real_type>{attrib.texcoords[(2 * idx_tc) + 0], attrib.texcoords[(2 * idx_tc) + 1]});
    }

    // Read mesh connectivity and store it as lists of indices to the real data.
    auto n_shapes{shapes.size()};
    mesh->n_triangles = 0; // We must reset this here.
    // In case the OBJ file has the triangles organized in several shapes or
    // groups, we ignore this and store all triangles as a single mesh dataset.
    // This is why we need to reset the triangle count here.
    for (auto idx_s{0U}; idx_s < n_shapes; idx_s++)
    {

        // For each face print out the indices lists.
        size_t index_offset = 0;
        // # of triangles for this "shape" (group).
        // NOTE that we are accumulate the number of triangles coming from the
        // shapes present in the OBJ file.
        mesh->n_triangles += shapes[idx_s].mesh.num_face_vertices.size();
        for (auto idx_f{0}; idx_f < mesh->n_triangles; idx_f++)
        {
            // Number of vertices per face (always 3, in our case)
            size_t fnum = shapes[idx_s].mesh.num_face_vertices[idx_f];

            // TODO: Invert order of vertices if flag is on. (DONE!)
            mesh->reverse_vertex_order = reverse_order;
            // For each vertex in the face get the corresponding indices
            for (size_t v = 0; v < fnum; v++)
            {
                tinyobj::index_t idx = shapes[idx_s].mesh.indices[index_offset + v];
                // Add the indices to the global list of indices we need to pass on to
                // the mesh object. This goes to the mesh data structure.
                mesh->vertex_indices.push_back(idx.vertex_index);
                mesh->normal_indices.push_back(idx.normal_index);
                mesh->uvcoord_indices.push_back(idx.texcoord_index);
            }
            // }

            // Advance over to the next triangle.
            index_offset += fnum;
        }
    }
}

/// This function creates the internal data structure, required by the RT3.
std::vector<std::shared_ptr<Shape>> create_triangle_mesh(std::shared_ptr<TriangleMesh> mesh, bool backface_cull)
{
    // List of shapes (triangles) we need to return to the client.
    std::vector<std::shared_ptr<Shape>> tris;
    // Create the triangles, which are just references to the mesh database.
    tris.reserve(mesh->n_triangles);
    for (int i = 0; i < mesh->n_triangles; ++i)
    {
        tris.push_back(std::make_shared<Triangle>(mesh, i));
    }

    return tris;
}

/** ------------------------------ Lights ------------------------------ */
std::shared_ptr<AmbientLight> Builder::build_ambient_light(const ParamSet &params)
{
    Spectrum intensity = params.retrieve("I", Spectrum{0.0f, 0.0f, 0.0f});
    Spectrum scale = params.retrieve("scale", Spectrum{0.0f, 0.0f, 0.0f});

    std::shared_ptr<AmbientLight> ambient = std::make_shared<AmbientLight>(light_flag_e::ambient, intensity, scale);
    return ambient;
}

std::shared_ptr<DirectionalLight> Builder::build_directional_light(const ParamSet &params)
{
    Spectrum intensity = params.retrieve("I", Spectrum{0.0f, 0.0f, 0.0f});
    Spectrum scale = params.retrieve("scale", Spectrum{0.0f, 0.0f, 0.0f});
    Point3<real_type> from = params.retrieve("from", Point3<real_type>{0.0f, 0.0f, 0.0f});
    Point3<real_type> to = params.retrieve("to", Point3<real_type>{0.0f, 0.0f, 0.0f});

    std::shared_ptr<DirectionalLight> directional =
        std::make_shared<DirectionalLight>(light_flag_e::directional, intensity, scale);
    directional->set_parameters(from, to);
    return directional;
}

std::shared_ptr<PointLight> Builder::build_point_light(const ParamSet &params)
{
    Spectrum intensity = params.retrieve("I", Spectrum{0.0f, 0.0f, 0.0f});
    Spectrum scale = params.retrieve("scale", Spectrum{0.0f, 0.0f, 0.0f});
    Point3<real_type> from = params.retrieve("from", Point3<real_type>{0.0f, 0.0f, 0.0f});

    std::shared_ptr<PointLight> point = std::make_shared<PointLight>(light_flag_e::point, intensity, scale);
    point->set_parameters(from);
    return point;
}

std::shared_ptr<SpotLight> Builder::build_spot_light(const ParamSet &params)
{
    Spectrum intensity = params.retrieve("I", Spectrum{0.0f, 0.0f, 0.0f});
    Spectrum scale = params.retrieve("scale", Spectrum{0.0f, 0.0f, 0.0f});
    Point3<real_type> from = params.retrieve("from", Point3<real_type>{0.0f, 0.0f, 0.0f});
    Point3<real_type> to = params.retrieve("to", Point3<real_type>{0.0f, 0.0f, 0.0f});
    real_type cutoff = params.retrieve("cutoff", 0.0f);
    real_type falloff = params.retrieve("falloff", 0.0f);

    std::shared_ptr<SpotLight> spot = std::make_shared<SpotLight>(light_flag_e::spot, intensity, scale);
    spot->set_parameters(from, to, cutoff, falloff);
    return spot;
}

/** ------------------------------ Primitives ------------------------------ */
std::shared_ptr<Shape> Builder::build_sphere(const ParamSet &params)
{
    Point3<real_type> center = params.retrieve<Point3<real_type>>("center", {0.0f, 0.0f, 0.0f});
    float radius = params.retrieve<float>("radius", 1.0f);

    std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>(center, radius);
    return sphere;
}

std::shared_ptr<Shape> Builder::build_box(const ParamSet &params)
{
    Point3<real_type> min = params.retrieve<Point3<real_type>>("min", {0.0f, 0.0f, 0.0f});
    Point3<real_type> max = params.retrieve<Point3<real_type>>("max", {1.0f, 1.0f, 1.0f});

    std::shared_ptr<Box> box = std::make_shared<Box>(min, max);

    return box;
}

std::shared_ptr<Shape> Builder::build_plane(const ParamSet &params)
{
    Point3<real_type> point = params.retrieve<Point3<real_type>>("point", {0.0f, 0.0f, 0.0f});
    Vector3<real_type> normal = params.retrieve<Vector3<real_type>>("normal", {0.0f, 0.0f, 0.0f});

    std::shared_ptr<Plane> plane = std::make_shared<Plane>(point, normal);
    return plane;
}

std::vector<std::shared_ptr<Shape>> Builder::build_triangle_mesh(bool flip_normals, const ParamSet &params)
{
    std::string filename = params.retrieve<std::string>("filename", "");
    std::shared_ptr<TriangleMesh> mesh_ptr = std::make_shared<TriangleMesh>(); // Default Ctro.

    bool reverse_vertex_order = params.retrieve("reverse_vertex_order", false);
    bool compute_normals = params.retrieve("compute_normals", false);
    bool backface_cull = params.retrieve("backface_cull_on", false);
    DEBUG(FORMAT("Mesh with rvo, cn, bn: ", reverse_vertex_order, " ", compute_normals, " ", backface_cull));

    if (!filename.empty())
    {
        if (load_mesh_data(filename, reverse_vertex_order, compute_normals, flip_normals, mesh_ptr))
        {
            std::cout << ">>> Mesh data successfuly loaded!\n";
        }
        else
        {
            std::cout << ">>> Mesh data loading failed!\n";
        }
        mesh_ptr->backface_cull = backface_cull;
    }
    else
    {
        int n_triangles = params.retrieve("ntriangles", 0);
        std::vector<int> vertex_indices = params.retrieve<std::vector<int>>("indices");
        std::cout << "vertex indices:" << vertex_indices << std::endl;
        std::vector<int> normal_indices = params.retrieve<std::vector<int>>("normal_indices", {});
        std::cout << "normal indices:" << normal_indices << std::endl;
        if (normal_indices.empty()) {
            normal_indices = vertex_indices;
        }
        std::vector<int> uv_indices = vertex_indices;

        std::vector<Vector3<real_type>> normals = params.retrieve<std::vector<Vector3<real_type>>>("normals", {});
        std::vector<Point3<real_type>> vertexes = params.retrieve<std::vector<Point3<real_type>>>("vertices", {});
        std::vector<Point2<real_type>> uv = params.retrieve<std::vector<Point2<real_type>>>("uv", {});

        mesh_ptr =
            std::make_shared<TriangleMesh>(n_triangles,          // n_triangles
                                           backface_cull,        // backface_cull
                                           reverse_vertex_order, // reverse_vertex_order
                                           std::move(vertex_indices), std::move(normal_indices), std::move(uv_indices),
                                           std::move(vertexes), std::move(normals), std::move(uv));
    }
    DEBUG(FORMAT("Builded Triangle Mesh: ", mesh_ptr->to_string()));

    int mesh_n_triangle = mesh_ptr->n_triangles;
    std::vector<std::shared_ptr<Shape>> triangles;
    triangles.reserve(mesh_n_triangle);

    for (size_t i = 0; i < mesh_n_triangle; i++)
    {
        Triangle new_triangle = Triangle(mesh_ptr, i);

        triangles.push_back(std::make_shared<Triangle>(new_triangle));
    }

    return triangles;
}

/** ------------------------------ Materials ------------------------------ */
std::shared_ptr<Material> Builder::build_flat_material(const ParamSet &params, bool isNamed)
{
    std::shared_ptr<Material> new_material_ptr = nullptr;
    Spectrum color = params.retrieve("color", Spectrum{0, 0, 0});
    if (isNamed)
    {
        std::string name_val = params.retrieve<std::string>("name", "");
        if (name_val.empty()) // Usar .empty() para strings
        {
            ERROR("A named material doesn't have a name");
            return new_material_ptr;
        }
        new_material_ptr = std::make_shared<FlatMaterial>(name_val, color);
    }
    else
    {
        new_material_ptr = std::make_shared<FlatMaterial>(color);
    }

    return new_material_ptr;
}

std::shared_ptr<Material> Builder::build_blinn_material(const ParamSet &params, bool isNamed)
{
    std::shared_ptr<BlinnPhongMaterial> blinn_specific_ptr;
    std::shared_ptr<Material> new_material_ptr = nullptr;
    Spectrum color = params.retrieve("color", Spectrum{0, 0, 0});

    if (isNamed)
    {
        std::string name_val = params.retrieve<std::string>("name", "");
        TRACE(FORMAT("isNamed=true, retrieved name_val: '", name_val, "'"));
        if (name_val.empty())
        {
            ERROR("A named material doesn't have a name");
            return new_material_ptr;
        }
        blinn_specific_ptr = std::make_shared<BlinnPhongMaterial>(name_val, color);
    }
    else
    {
        blinn_specific_ptr = std::make_shared<BlinnPhongMaterial>(color);
    }

    if (blinn_specific_ptr)
    {
        Spectrum kd = params.retrieve("diffuse", Spectrum{0, 0, 0});
        Spectrum ks = params.retrieve("specular", Spectrum{0, 0, 0});
        Spectrum ka = params.retrieve("ambient", Spectrum{0, 0, 0});
        Spectrum km = params.retrieve("mirror", Spectrum{0, 0, 0});
        real_type gloss = params.retrieve("glossiness", 0.0f);
        blinn_specific_ptr->set_parameters(kd, ks, ka, km, gloss);

        new_material_ptr = blinn_specific_ptr;
    }

    return new_material_ptr;
}
} // namespace gc