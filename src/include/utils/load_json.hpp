#ifndef LOAD_JSON_HPP
#define LOAD_JSON_HPP

#include <memory>
#include <unordered_map>
#include <vector>

// From src/include
#include <camera.hpp>
#include <extern/json.hpp>
#include <hittable.hpp>

struct ImageInfo {
    size_t height;
    int max_bounces;
    int spp;
    AspectRatio aspect_ratio;
};

struct Params {
    Camera cam;
    ImageInfo info;
    std::vector<GlobalIllumination> global_lights;
    std::unordered_map<std::string, std::shared_ptr<Material>> materials;
    std::vector<std::shared_ptr<Hittable>> objects;
    std::vector<size_t> sampled_objects;

    static Params load_params(const char * filename);
};

#endif
