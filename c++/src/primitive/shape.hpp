#pragma once
#include "../core/common.hpp"
#include "../core/ray.hpp"
#include "../math/geometry.hpp"
#include "../math/math_operators.hpp"
#include "primitive.hpp"
#include "surfel.hpp"

namespace gc
{

class Shape
{
  public:
    virtual bool intersect(const Ray &r, Surfel *sf) const = 0;
    virtual bool intersect_p(const Ray &r) const = 0;
    virtual std::string to_string() const = 0;
    virtual Bounds3f get_bounds() const = 0;
};

class Sphere : public Shape
{
  public:
    Sphere(const Point3<real_type> &center, float radius) : radius(radius), center(center)
    {
    }

    bool intersect(const Ray &r, Surfel *sf) const override;
    bool intersect_p(const Ray &r) const override;
    Bounds3f get_bounds() const override;

    std::string to_string() const override;

    float radius;

  private:
    Point3<real_type> center;
};

class Plane : public Shape
{
  public:
    Plane(const Point3<real_type> &point, const Vector3<real_type> &normal) : point(point), normal(normal)
    {
    }

    bool intersect(const Ray &r, Surfel *sf) const override;
    bool intersect_p(const Ray &r) const override;
    Bounds3f get_bounds() const override;

    std::string to_string() const override;

  private:
    Point3<real_type> point;
    Vector3<real_type> normal;
};

class Box : public Shape
{
  public:
    Box(const Point3<real_type> &min, const Point3<real_type> &max) : min(min), max(max)
    {
    }

    bool intersect(const Ray &r, Surfel *sf) const override;
    bool intersect_p(const Ray &r) const override;
    Bounds3f get_bounds() const override;

    std::string to_string() const override;

  private:
    Point3<real_type> min;
    Point3<real_type> max;
};

struct TriangleMesh
{
    /// # of triangles in the mesh.
    int n_triangles;

    bool backface_cull;
    bool reverse_vertex_order;
    // The size of the three lists below should be 3 * nTriangles. Every 3 values
    // we have a triangle.
    std::vector<int> vertex_indices;  // The list of indices to the vertex list, for
                                      // each individual triangle.
    std::vector<int> normal_indices;  // The list of indices to the normals list, for
                                      // each individual triangle.
    std::vector<int> uvcoord_indices; // The list of indices to the UV coord list,
                                      // for each individual triangle.

    // Here comes the data associated with each vertex.
    // WARNING: The length of the vectors below may differ.
    std::vector<Point3<real_type>> vertices; //!< The 3D geometric coordinates
    std::vector<Vector3<real_type>> normals; //!< The 3D normals.
    std::vector<Point2<real_type>> uvcoords; //!< The 2D texture coordinates.

    // Regular constructor (almost all data is imported via move semantics).
    TriangleMesh() = default;
    /// Copy constructor disabled
    TriangleMesh(const TriangleMesh &) = delete;
    /// Assign operator disabled.
    TriangleMesh &operator=(const TriangleMesh &) = delete;
    /// Move constructor.
    TriangleMesh(TriangleMesh &&other) = delete;

    TriangleMesh(int n_triangles_val, bool backface_cull_val, bool reverse_vertex_order_val,
                 std::vector<int> &&vertex_indices_val, std::vector<int> &&normal_indices_val,
                 std::vector<int> &&uvcoord_indices_val, std::vector<Point3<real_type>> &&vertices_val,
                 std::vector<Vector3<real_type>> &&normals_val, std::vector<Point2<real_type>> &&uvcoords_val)
        : n_triangles(n_triangles_val), backface_cull(backface_cull_val),
          reverse_vertex_order(reverse_vertex_order_val), vertex_indices(std::move(vertex_indices_val)),
          normal_indices(std::move(normal_indices_val)), uvcoord_indices(std::move(uvcoord_indices_val)),
          vertices(std::move(vertices_val)), normals(std::move(normals_val)), uvcoords(std::move(uvcoords_val))
    {
    }

    std::string to_string() const;
};

std::ostream &operator<<(std::ostream &os, const TriangleMesh &mesh);

class Triangle : public Shape
{
  public:
    Triangle(std::shared_ptr<TriangleMesh> mesh, int idx) : mesh{mesh}
    {
        vertices_idx = &mesh->vertex_indices[idx * 3];
        normals_idx = &mesh->normal_indices[idx * 3];
        uvs_idx = &mesh->uvcoord_indices[idx * 3];
    };

    Triangle(int *v, int *n, int *uv, std::shared_ptr<TriangleMesh> mesh) : mesh{mesh}
    {
        vertices_idx = v;
        normals_idx = n;
        uvs_idx = uv;
    }

    bool intersect(const Ray &r, Surfel *sf) const override;
    bool intersect_p(const Ray &r) const override;
    Bounds3f get_bounds() const override;

    std::string to_string() const override;

  private:
    /**
     * These three pointers are respective to a three int vector storing the indexes of the vertices, normals and uvs of
     * this triangles So, the access to the indexes will be done with pointer arithmetic, knowing that the first 3
     * integers correspond to this trinagle
     */
    int *vertices_idx;
    int *normals_idx;
    int *uvs_idx;
    std::shared_ptr<TriangleMesh> mesh;
};
} // namespace gc