#include "Light.hpp"

std::map<std::string, int> PointLight::uniform_ids = {
      {"_pointLightC", -1},
      {"_pointLightP", -1},
    };

std::map<std::string, int> FocalLight::uniform_ids = {
      {"_focalLightC", -1},
      {"_focalLightD", -1},
      {"_focalLightA", -1},
      {"_focalLightP", -1},
    };