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
    // Height of the image
    size_t height;
    // Iteration limit
    int max_bounces;
    // Samples per pixel
    int spp;
    // AspectRatio of the image
    AspectRatio aspect_ratio;
};

// Parameters of a render. Allocates the necessary memory.
struct Params {
    // Virtual camera settings
    Camera cam;
    // Basic image and rendering settings
    ImageInfo info;
    // Vector of global lights
    std::vector<GlobalIllumination> global_lights;
    // Map of named materials
    std::unordered_map<std::string, std::shared_ptr<Material>> materials;
    // Vector of objects
    std::vector<std::shared_ptr<Hittable>> objects;
    // Vector of sampled objects
    std::vector<size_t> sampled_objects;

    // Load the parameters from a JSON filename
    static Params load_params(const char * filename);
};

#endif
