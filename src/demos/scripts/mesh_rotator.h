#include "../../spatial/mesh/MeshInstance.hpp"
#include "../../scriptable/Scriptable.hpp"

__START_SCRIPT__ (mesh_rotator, MeshInstance)

  double public_ (value) = 0.01;
  std::function<void (void)> public_ (call) = [&]{
    std::cout << "Called" << std::endl;
  };

  $.on_update = [&](const float delta_time) {
    $.rotation().y += value;
  };

__END_SCRIPT__
