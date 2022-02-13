#ifndef LOAD_JSON_HPP
#define LOAD_JSON_HPP

#include "utils/image.hpp"

#include <camera.hpp>
#include <hittable.hpp>
#include <memory>
#include <vector>
#include <extern/json.hpp>

using namespace nlohmann;
using namespace std;

struct ImageInfo {
    size_t height;
    int max_bounces;
    int spp;
};

struct Params {
    Camera cam;
    ImageInfo info;
    std::vector<GlobalIllumination> global_lights;
    std::vector<std::unique_ptr<Material>> materials;
    std::vector<std::unique_ptr<Hittable>> objects;
};

Vec3 load_vec3(json j);

Colour load_colour(json j);

AspectRatio load_aspect_ratio(json & j);

Camera load_cam(json & j);

ImageInfo load_image_info(json & j);

vector<GlobalIllumination> load_lights(json & j);

vector<unique_ptr<Material>> load_materials(json & j);

vector<unique_ptr<Hittable>>
load_objects(json & j, vector<unique_ptr<Material>> & materials);

Params load_params(const char * file_name);

#endif