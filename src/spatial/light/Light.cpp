#include "Light.hpp"

std::map<std::string, int> DirectionalLight::uniform_ids ={
      {"_dirLightC", -1},
      {"_dirLightD", -1},
    };

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

std::map<std::string, int> AmbientLight::uniform_ids = {
      {"_ambientLightC", -1},
    };