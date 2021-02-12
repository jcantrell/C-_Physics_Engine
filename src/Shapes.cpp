#include "Shapes.hpp"

bool Triangle::contains(const vec3& in) const {
  vec3 a = points.at(0);
  vec3 b = points.at(1);
  vec3 c = points.at(2);
	return feq( ((b-a).cross_product(c-a)).dot_product(in-a) , 0 );
}

vec3 Triangle::findCenter() const {
  return (points.at(0)+points.at(1)+points.at(2))/3;
}

bool Box::operator==(const Box& rhs) const {
	return center==rhs.center && points[0]==rhs.points[0] && points[1]==rhs.points[1];
}

std::ostream& operator<<(std::ostream &out, const Box& rhs) {
	out << rhs.center << " " << rhs.points[0] << " " << rhs.points[1] ;
	return out;
}

std::ostream& operator<<(std::ostream &out, const Sphere& rhs) {
	out << rhs.center << " " << rhs.radius ;
	return out;
}

std::ostream& operator<<(std::ostream &out, const Triangle& rhs) {
  out << "(";
  for (auto p: rhs.points)
	  out << p << ", ";
  out << ")";
	return out;
}

Box Triangle::getBoundingBox() const {
	vec3 min(points[0]);
	vec3 max(points[0]);
	for (auto p : points) {
		max = max.max_components(p);
		min = min.min_components(p);
	}
	return Box(0.5*(max+min), -0.5*(max-min), 0.5*(max-min));
}

Sphere Triangle::getBoundingSphere() const {
	vec3 min(points[0]);
	vec3 max(points[0]);
	for (auto p : points) {
		max = max.max_components(p);
		min = min.min_components(p);
	}
	return Sphere(0.5*(max+min), max.length());
}

/**
* @ brief Determines if a point is contained by the point
* We represent a box as 1. It's center, as a vector from the origin
* and 2. Two vectors to define opposite corners of the box, as defined
* from the box center.
* 
* To determine if a point is in the box, we first find the volume vector
* from the bottom left to the top right. Then we find the difference 
* vector of the point in reference to the bottom corner of the box.
* Now that we have two vectors in reference to the same starting point,
* we can subtract the point from the volume vector, and if any of the
* parts are negative, we know the point is outside the box.
*/
bool Box::contains(const vec3& in) const {
	vec3 corner = center + points[0];
	vec3 volume = 2*points[1];
	vec3 diff = in - corner;
	return !(volume - diff).negative();
}

vec3 Box::findCenter() const {
	return center;
}

Box Box::getBoundingBox() const {
	return Box(*this);
}

Sphere Box::getBoundingSphere() const {
	return Sphere(center, points[0].length());
}

bool Sphere::operator== (const Sphere& rhs) const {
	return center == rhs.center && feq(radius , rhs.radius);
}

bool Sphere::contains(const vec3& in) const {
	vec3 d = center - in;
	return radius >= d.length();
}

vec3 Sphere::findCenter() const {
	return center;
}

Sphere Sphere::getBoundingSphere() const {
	return Sphere(center, radius);
}

Box Sphere::getBoundingBox() const {
	return Box(center, vec3(radius,radius,radius), vec3(-radius,-radius,-radius));
}

void Prism::addPoint(vec3 point) {
	points.push_back(point);
}

// TODO: Get an algorithm for properly checking for point inclusion
bool Prism::contains(const vec3& in) const {
	return getBoundingBox().contains(in)
				 && getBoundingSphere().contains(in);
}

vec3 Prism::findCenter() const {
	return center;
}

Box Prism::getBoundingBox() const {
	vec3 min(points[0]);
	vec3 max(points[0]);
	for (auto p : points) {
		max = max.max_components(p);
		min = min.min_components(p);
	}
	return Box(center, max, min);
}

Sphere Prism::getBoundingSphere() const {
	vec3 max(points[0]);
	for (auto p : points) {
		max = max.max_components(p);
	}
	return Sphere(center, max.length());
}
