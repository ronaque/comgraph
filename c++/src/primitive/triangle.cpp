#include "../msg_system/error.hpp"
#include "shape.hpp"

namespace gc
{
std::string Triangle::to_string() const
{
    std::ostringstream oss;
    oss << "Triangle: {\n"
        << "  Vertices: [" << mesh->vertices[vertices_idx[0]] << ", " << mesh->vertices[vertices_idx[1]] << ", "
        << mesh->vertices[vertices_idx[2]] << "],\n"
        << "  Normals: [" << mesh->normals[normals_idx[0]] << ", " << mesh->normals[normals_idx[1]] << ", "
        << mesh->normals[normals_idx[2]] << "],\n"
        << "  UVs: " << uvs_idx[0] << ", " << uvs_idx[1] << ", " << uvs_idx[2] << ",\n"
        << "}";
    return oss.str();
}

std::string TriangleMesh::to_string() const
{
    std::stringstream ss;
    ss << "--- TriangleMesh ---\n";
    ss << "  Triangles: " << n_triangles << "\n";
    ss << "  Backface Cull: " << std::boolalpha << backface_cull << "\n";
    ss << "  Reverse Vertex Order: " << std::boolalpha << reverse_vertex_order << "\n\n";

    ss << "  --- Indices (counts) ---\n";
    ss << "  Vertex Indices:  " << vertex_indices.size() << ", " << vertex_indices << "\n";
    ss << "  Normal Indices:  " << normal_indices.size() << ", " << normal_indices << "\n";
    ss << "  UVCoord Indices: " << uvcoord_indices.size() << ", " << uvcoord_indices << "\n\n";

    ss << "  --- Data (counts) ---\n";
    ss << "  Vertices: " << vertices.size() << "\n";
    ss << "  Normals:  " << normals.size() << "\n";
    ss << "  UVCoords: " << uvcoords.size() << "\n";
    ss << "----------------------";

    return ss.str();
}

std::ostream &operator<<(std::ostream &os, const TriangleMesh &mesh)
{
    os << mesh.to_string();
    return os;
}

bool Triangle::intersect(const Ray &r, Surfel *sf) const
{
    Vector3<real_type> edge1, edge2, tvec, pvec, qvec, dir;
    Point3<real_type> v0, v1, v2, orig;
    real_type det, inv_det, u, v, t;

    if (mesh->reverse_vertex_order)
    {
        v0 = mesh->vertices[vertices_idx[0]];
        v1 = mesh->vertices[vertices_idx[2]];
        v2 = mesh->vertices[vertices_idx[1]];
    }
    else
    {

        v0 = mesh->vertices[vertices_idx[0]];
        v1 = mesh->vertices[vertices_idx[1]];
        v2 = mesh->vertices[vertices_idx[2]];
    }
    // std::cout << "Intersecting with vertices: [" << v0 << ", " << v1 << ", " << v2 << "]" << std::endl;
    dir = r.direction;
    orig = r.origin;

    edge1 = v1 - v0;
    edge2 = v2 - v0;

    pvec = cross(dir, edge2);
    det = dot(edge1, pvec);

    if (mesh->backface_cull)
    {
        if (det < shadow_epsilon)
            return false;

        tvec = orig - v0;

        u = dot(pvec, tvec);
        if (u < 0.0 || u > (det + shadow_epsilon))
            return false;

        qvec = cross(tvec, edge1);
        v = dot(qvec, dir);
        if (v < 0.0 || u + v > (det + shadow_epsilon))
            return false;

        t = dot(qvec, edge2);
        inv_det = 1.0 / det;
        t *= inv_det;
        u *= inv_det;
        v *= inv_det;
    }
    else
    {
        if (det > -shadow_epsilon && det < shadow_epsilon)
            return false;
        inv_det = 1.0 / det;

        tvec = orig - v0;

        u = dot(tvec, pvec) * inv_det;
        if (u < 0.0 || u > (1.0 + shadow_epsilon))
            return false;

        qvec = cross(tvec, edge1);

        v = dot(dir, qvec) * inv_det;
        if (v < 0.0 || u + v > (1.0 + shadow_epsilon))
            return false;

        t = dot(edge2, qvec) * inv_det;
    }

    if (t >= r.t_min && t <= r.t_max)
    {
        r.t_max = t;
        Point3<real_type> hit_point = r(t);
        // std::cout << "Ray(t)" << r(t);
        // Vector3<real_type> normal = mesh->normals[normals_idx[0]];
        // Vector3<real_type> normal = cross(edge1, edge2).normalize();
        std::array<Vector3<real_type>, 3> normals{mesh->normals[normals_idx[0]], mesh->normals[normals_idx[1]],
                                                  mesh->normals[normals_idx[2]]};
        Vector3<real_type> normal = normals[0] * (1 - u - v) + normals[1] * u + normals[2] * v;

        // std::cout << ", T, U, V, normal intersected: " << t << ", " << u << ", " << v << ", " << normal << std::endl;
        *sf = Surfel(hit_point, normal, -r.direction, {u, v}, nullptr);

        return true;
    }
    return false;
}

bool Triangle::intersect_p(const Ray &r) const
{
    Vector3<real_type> edge1, edge2, tvec, pvec, qvec, dir;
    Point3<real_type> v0, v1, v2, orig;
    real_type det, inv_det, u, v, t;

    if (mesh->reverse_vertex_order)
    {
        v0 = mesh->vertices[vertices_idx[0]];
        v1 = mesh->vertices[vertices_idx[2]];
        v2 = mesh->vertices[vertices_idx[1]];
    }
    else
    {

        v0 = mesh->vertices[vertices_idx[0]];
        v1 = mesh->vertices[vertices_idx[1]];
        v2 = mesh->vertices[vertices_idx[2]];
    }

    dir = r.direction;
    orig = r.origin;

    edge1 = v1 - v0;
    edge2 = v2 - v0;

    pvec = cross(dir, edge2);
    det = dot(edge1, pvec);

    if (mesh->backface_cull)
    {
        if (det < shadow_epsilon)
            return false;

        tvec = orig - v0;

        u = dot(pvec, tvec);
        if (u < 0.0 || u > (det + shadow_epsilon))
            return false;

        qvec = cross(tvec, edge1);
        v = dot(qvec, dir);
        if (v < 0.0 || u + v > (det + shadow_epsilon))
            return false;

        t = dot(qvec, edge2);
        inv_det = 1.0 / det;
        t *= inv_det;
        u *= inv_det;
        v *= inv_det;
    }
    else
    {
        if (det > -shadow_epsilon && det < shadow_epsilon)
            return false;
        inv_det = 1.0 / det;

        tvec = orig - v0;

        u = dot(tvec, pvec) * inv_det;
        if (u < 0.0 || u > (1.0 + shadow_epsilon))
            return false;

        qvec = cross(tvec, edge1);

        v = dot(dir, qvec) * inv_det;
        if (v < 0.0 || u + v > (1.0 + shadow_epsilon))
            return false;

        t = dot(edge2, qvec) * inv_det;
    }

    if (t >= r.t_min && t <= r.t_max)
    {
        return true;
    }
    return false;
}

Bounds3f Triangle::get_bounds() const
{
    Point3<real_type> v0, v1, v2;

    v0 = mesh->vertices[vertices_idx[0]];
    v1 = mesh->vertices[vertices_idx[2]];
    v2 = mesh->vertices[vertices_idx[1]];

    Vector3<real_type> min, max;
    int minx, miny, minz, maxx, maxy, maxz;

    minx = mmin(v0.x, v1.x, v2.x);
    miny = mmin(v0.y, v1.y, v2.y);
    minz = mmin(v0.z, v1.z, v2.z);

    maxx = mmax(v0.x, v1.x, v2.x);
    maxy = mmax(v0.y, v1.y, v2.y);
    maxz = mmax(v0.z, v1.z, v2.z);
    
    min = {minx, miny, minz};
    max = {maxx, maxy, maxz};

    return {min, max};
}

} // namespace gc