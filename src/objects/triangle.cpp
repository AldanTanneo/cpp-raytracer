// From src/include
#include <hittable.hpp>
#include <objects/triangle.hpp>
#include <utils/vec3.hpp>

bool Triangle::hit(const Ray & ray,
                 double tmin,
                 double tmax,
                 HitRecord & hit_record) const noexcept {
    
    // Tests if ray's direction is (almost) colinear to the triangle ?
    const double det = ray.direction.dot(normal);
    if (std::abs(det) < utils::EPSILON) {return false;}

    // Finding the moment of impact
    double time = (vertex - ray.origin).dot(normal) / det;

    // Tests if is negative (behind the camera) or not in the time window 
    if (time<tmin || time>tmax) {return false;}

    // Tests if I is in the triangle (using barycentric coordinates w1,w2)
    Point3 I = ray.at(time);
    //Vec3 origin_to_triangle = ray.origin - vertex;
    const double w1 = (I - vertex).cross(edge1).z / edge2.cross(edge1).z;
    //const double w1 = (ray.direction.cross(edge2)).dot(origin_to_triangle) / det;
    if (w1 < 0) {return false;}
    
    //const double w2 = (edge1.cross(ray.direction)).dot(origin_to_triangle) / det;
    const double w2 = -(I - vertex).cross(edge2).z / edge2.cross(edge1).z;
    if (w2 < 0 || w1+w2 > 1) {return false;}
    
    // Else, there was a hit in the time window
    hit_record.time = time;
    hit_record.hit_point = ray.at(time);
    Vec3 outward_normal = normal;
    hit_record.set_face_normal(ray, outward_normal);
    hit_record.material = material;
    return true;
}
