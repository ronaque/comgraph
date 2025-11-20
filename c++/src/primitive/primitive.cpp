#include "primitive.hpp"

namespace gc
{
std::string GeometricPrimitive::to_string() const
{
    std::ostringstream oss;
    oss << "GeometricPrimitive = { Material (from Primitive): " << (material ? material->to_string() : "nullptr")
        << ", Shape: " << (shape ? shape->to_string() : "nullptr") << "}";
    return oss.str();
}

std::string PrimList::to_string() const
{
    std::ostringstream oss;
    oss << "PrimList = {Count: " << primitive_list.size() << "\n";
    for (size_t i = 0; i < primitive_list.size(); ++i)
    {
        oss << "  Primitive[" << i << "]: ";
        if (primitive_list[i])
        {
            // Para melhor formatação de primitivas multi-linha, você pode querer
            // uma função auxiliar para indentar a string retornada.
            // Por simplicidade, aqui apenas chamamos diretamente.
            std::string prim_str = primitive_list[i]->to_string();
            std::string line;
            std::istringstream iss(prim_str);
            bool first_line = true;
            while (std::getline(iss, line))
            {
                if (!first_line)
                    oss << "               "; // Indentação para linhas subsequentes
                oss << line << "\n";
                first_line = false;
            }
        }
        else
        {
            oss << "nullptr\n";
        }
    }
    oss << "}";
    return oss.str();
}

bool GeometricPrimitive::intersect_p(const Ray &r) const
{
    return shape->intersect_p(r);
};



bool GeometricPrimitive::intersect(const Ray &r, Surfel *sf) const
{
    return shape->intersect(r, sf);
};

bool PrimList::intersect(const Ray &r, Surfel *sf) const
{
    for (std::shared_ptr<Primitive> prim : primitive_list)
    {
        if (prim->intersect(r, sf))
            return true;
    }

    return false;
}

bool PrimList::intersect_p(const Ray &r) const
{
    for (std::shared_ptr<Primitive> prim : primitive_list)
    {
        if (prim->intersect_p(r))
            return true;
    }

    return false;
}
} // namespace gc