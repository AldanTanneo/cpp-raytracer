#include <hittable.hpp>
#include <objects/sphere.hpp>
#include <utils/vec3.hpp>

bool Sphere::hit(Ray ray_in, double tmin, double tmax, HitRecord & hit_record) const {
    // sovling ( ray_origin + t * ray_direction - sphere_centre )^2 = sphere_radius^2
    double a = ray_in.get_direction().dot(ray_in.get_direction());
    double b = 2. * ray_in.get_direction().dot(ray_in.get_origin() - centre);
    double c = (ray_in.get_origin() - centre).dot(ray_in.get_origin() - centre);
    double delta = b * b - 4. * a * c;
    if (delta < 0) { // sphere not hit
        return false;
    } else if (delta == 0) { // sphere hit in 1 point
        double t = -b / (2. * a);
        hit_record.hit_point = ray_in.get_origin() + t * ray_in.get_direction();
        return true;
    } else { // sphere hit in 2 points
        double t = (-b - sqrt(delta)) / (2. * a);
        t = (t > utils::EPSILON) ? t : (-b - sqrt(delta)) / (2. * a); // smallest positive solution
        hit_record.hit_point = ray_in.get_origin() + t * ray_in.get_direction();
        return true;
    }
}
