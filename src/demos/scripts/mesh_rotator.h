#ifndef _MESH_ROTATOR_
#define _MESH_ROTATOR_

#include "../../spatial/mesh/MeshInstance.hpp"
#include "../../scriptable/Scriptable.hpp"

__START_SCRIPT__ (mesh_rotator, MeshInstance)

  double export_ (value) = 0.01;

  $.on_update = [&](const float delta_time) {
    $.rotation().y += value;
  };

__END_SCRIPT__

#endif