#ifndef LOAD_JSON_HPP
#define LOAD_JSON_HPP

#include <memory>
#include <unordered_map>
#include <vector>

// From src/include
#include <camera.hpp>
#include <extern/json.hpp>
#include <hittable.hpp>

// Exception returned by the JSON parser on invalid input (ex: invalid object
// type, unknown material...)
class ParseJsonException : public std::exception {
    const char * msg;

public:
    ParseJsonException(const char * msg) {}

    virtual const char * what() const noexcept override { return msg; }
};

// Basic image information
struct ImageInfo {
    size_t height;
    int max_bounces;
    int spp;
    AspectRatio aspect_ratio;
};

// Parameters of a render. Allocates the necessary memory.
struct Params {
    Camera cam;
    ImageInfo info;
    std::vector<GlobalIllumination> global_lights;
    std::unordered_map<std::string, std::shared_ptr<Material>> materials;
    std::vector<std::shared_ptr<Hittable>> objects;
    std::vector<size_t> sampled_objects;

    // Load the parameters from a JSON filename
    static Params load_params(const char * filename);
};

#endif
