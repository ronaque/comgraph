#pragma once
#include "../primitive/surfel.hpp"
#include "../core/common.hpp"
#include "../core/scene.hpp"
#include <cstdint>
#include <vector>

namespace gc{
// Verifica se há oclusão entre dois pontos de contato.
class VisibilityTester {

public:

  VisibilityTester()=default;
  VisibilityTester( const Surfel&, const Surfel& );
  VisibilityTester( const Surfel&, const Vector3<real_type>& ); // For directional
  bool unoccluded( const Scene& scene );

public:
  Surfel p0, p1; //!< Test visibility between p0 and p1.
  Vector3<real_type> direction; //!< Direction to test visibility from p0.
};

}