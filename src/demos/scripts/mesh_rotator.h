#include "../../spatial/mesh/MeshInstance.hpp"
#include "../../scriptable/Scriptable.hpp"

__START_SCRIPT__ (mesh_rotator, MeshInstance)
  double value = 0.01;

  $.public_parameters["value"] = (void*)&value;

  $.on_update = [&](const float delta_time) {
    $.rotation().y += value;
  };

__END_SCRIPT__
