#pragma once
#include "../core/ray.hpp"
#include "material.hpp"
#include "shape.hpp"
#include "surfel.hpp"

#include <vector>
#include <algorithm>

namespace gc
{

class Primitive
{
  public:
    virtual ~Primitive() = default;
    virtual bool intersect(const Ray &r, Surfel *sf) const = 0;
    // Simpler & faster version of intersection that only return true/false.
    // It does not compute the hit point information.
    virtual bool intersect_p(const Ray &r) const = 0;
    virtual std::string to_string() const = 0;

    std::shared_ptr<Material> material;
};

class GeometricPrimitive : public Primitive
{
  public:
    ~GeometricPrimitive() = default;
    GeometricPrimitive()
    {
    }
    GeometricPrimitive(std::shared_ptr<Material> material, std::shared_ptr<Shape> shape) : shape{shape}
    {
        this->material = material;
    }

    bool intersect(const Ray &r, Surfel *sf) const;

    bool intersect_p(const Ray &r) const;
    std::string to_string() const override;

    std::shared_ptr<Shape> shape;
};

class AggregatePrimitive : public Primitive
{
  public:
    virtual ~AggregatePrimitive() = default;
    virtual bool intersect(const Ray &r, Surfel *sf) const = 0;
    virtual bool intersect_p(const Ray &r) const = 0;
};

class PrimList : public AggregatePrimitive
{
  public:
    PrimList(std::vector<std::shared_ptr<Primitive>> prim_list) : primitive_list{prim_list} {};
    std::vector<std::shared_ptr<Primitive>> primitive_list;
    bool intersect(const Ray &r, Surfel *sf) const;
    bool intersect_p(const Ray &r) const;
    std::string to_string() const override;

    auto begin() const
    {
        return primitive_list.begin();
    }
    auto end() const
    {
        return primitive_list.end();
    }
};

struct BVHBuildNode
{
    Bounds3f bounds;
    BVHBuildNode *left;
    BVHBuildNode *right;
    int nPrimitives;
};

inline int next_split_axis(int split_axis)
{
    if (split_axis == 0)
        return 1;
    else if (split_axis == 1)
        return 2;
    else
        return 0;
}

class BVHTree : public AggregatePrimitive
{
  public:
    BVHTree(const std::vector<std::shared_ptr<Primitive>> primitives, int start = 0)
    {
      int end = primitives.size();
      Bounds3f root_bound;
      for (size_t i = start; i < end; i++)
      {
        std::shared_ptr<GeometricPrimitive> primitive = std::dynamic_pointer_cast<GeometricPrimitive>(primitives[i]);

        root_bound = b_union(root_bound, primitive->shape->get_bounds());
      }
      

      std::vector<std::shared_ptr<Primitive>> ordered_primitives = 
        std::sort(primitives.begin(), primitives.end(), 
        [](std::shared_ptr<Primitive> a, std::shared_ptr<Primitive> b) {
          return a->
        }
      );
    }

    bool intersect(const Ray &r, Surfel *sf) const;
    bool intersect_p(const Ray &r) const;
    std::string to_string() const override;

    int total_nodes = 0;
    BVHBuildNode *root;

    void recursive_build(std::vector<std::shared_ptr<Primitive>> primitives, int start, int end, int split_axis);
};
}; // namespace gc