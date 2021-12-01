#include "Light.hpp"

std::map<std::string, int> DirectionalLight::uniform_ids ={
      {"DirLightC", -1},
      {"DirLightD", -1},
    };

std::map<std::string, int> PointLight::uniform_ids = {
      {"PointLightC", -1},
      {"PointLightP", -1},
    };

std::map<std::string, int> FocalLight::uniform_ids = {
      {"FocalLightC", -1},
      {"FocalLightD", -1},
      {"FocalLightA", -1},
      {"FocalLightP", -1},
    };

std::map<std::string, int> AmbientLight::uniform_ids = {
      {"AmbientLightC", -1},
    };