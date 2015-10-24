#include "TriangleMesh.h"

TriangleMesh::TriangleMesh(const std::vector<glm::vec3> verticies) 
: verticies_{verticies}
{

}


std::tuple<Mesh::Intersection, float, float> TriangleMesh::getIntersections(const Ray* ray) const {

  const glm::vec3 origin = ray->getOrigin();
  
  bool hit = false;
  float nearestHitDistance{std::numeric_limits<float>::max()};
  glm::vec3 nearestHit;

  for(unsigned int i=0; i<verticies_.size(); i+=3) {
    glm::vec3 intersection;
    glm::vec3 normal;
    bool intersects = triangleIntersection(ray, verticies_[i], verticies_[i+1], verticies_[i+2], intersection, normal);

    if( intersects ) {
      float distance = glm::distance(intersection, origin);
      if( distance < nearestHitDistance ) {
        nearestHit = intersection;
        hit = true;
      }
    }
  }

  float sMin = 0.5f; // TODO: FIX!
  float sMax = 1.0f;

  if( hit ) {
    return std::make_tuple(Mesh::Intersection::SINGLE_HIT, sMin, sMax);
  }

  return std::make_tuple(Mesh::Intersection::MISS, sMin, sMax);

}


std::tuple<Mesh::Intersection, glm::vec3, glm::vec3> TriangleMesh::hit(const Ray* ray) const {
 const glm::vec3 origin = ray->getOrigin();
  
  bool hit = false;
  float nearestHitDistance{std::numeric_limits<float>::max()};
  glm::vec3 nearestHit{0.0f, 0.0f, 0.0f};
  glm::vec3 nearestNormal{0.0f, 0.0f, 0.0f};

  for(unsigned int i=0; i<verticies_.size(); i+=3) {
    glm::vec3 intersection;
    glm::vec3 normal;
    bool intersects = triangleIntersection(ray, verticies_[i], verticies_[i+1], verticies_[i+2], intersection, normal);

    if( intersects ) {
      float distance = glm::distance(intersection, origin);
      if( distance < nearestHitDistance ) {
        nearestHit = intersection;
        nearestNormal = normal;
        hit = true;
      }
    }
  }

  if( hit ) {
    return std::make_tuple(Mesh::Intersection::SINGLE_HIT, nearestHit, nearestNormal);
  }

  return std::make_tuple(Mesh::Intersection::MISS, nearestHit, nearestNormal);
}


glm::vec3 TriangleMesh::getNormal(const glm::vec3& position) const {

  for(unsigned int i=0; i<verticies_.size(); i+=3) {
    const bool b1 = sign(position, verticies_[i], verticies_[i+1]) < 0.0f;
    const bool b2 = sign(position, verticies_[i+1], verticies_[i+2]) < 0.0f;
    const bool b3 = sign(position, verticies_[i+2], verticies_[i]) < 0.0f;

    if( (b1 == b2) && (b2 == b3) ) {
      // std::cout << "AA" << std::endl;
      glm::vec3 edge1 = verticies_[i+1] - verticies_[i];
      glm::vec3 edge2 = verticies_[i+2] - verticies_[i]; 
      return glm::normalize(glm::cross(edge1, edge2));
    }
  }

  for(unsigned int i=0; i<verticies_.size(); i+=3) {
   
    const glm::vec3 v0 = verticies_[i+1] - verticies_[i];
    const glm::vec3 v1 = verticies_[i+2] - verticies_[i];
    const glm::vec3 v2 = position - verticies_[i];

    // Compute dot products
    const float dot00 = glm::dot(v0, v0);
    const float dot01 = glm::dot(v0, v1);
    const float dot02 = glm::dot(v0, v2);
    const float dot11 = glm::dot(v1, v1);
    const float dot12 = glm::dot(v1, v2);

    // Compute barycentric coordinates
    const float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
    const float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    const float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    // if( (u >= 0) && (v >= 0) && (u + v < 1) ) {
    if( (u + getEpsilon() >= 0.0f) && (v + getEpsilon() >= 0.0f) && (u + v <= 1.0f + getEpsilon()) ) {
      // std::cout << "BB" << std::endl;
      glm::vec3 edge1 = verticies_[i+1] - verticies_[i];
      glm::vec3 edge2 = verticies_[i+2] - verticies_[i]; 
      return glm::normalize(glm::cross(edge1, edge2));
    }
  }

  // std::cout << "EH" << std::endl;
  return glm::vec3(0.0f, 1.0f, 0.0f);
}


bool TriangleMesh::triangleIntersection(const Ray* ray, 
                                        const glm::vec3& v1, 
                                        const glm::vec3& v2, 
                                        const glm::vec3& v3, 
                                        glm::vec3& intersection,
                                        glm::vec3& normal) const {

  // Find vectors for two edges sharing v1
  const glm::vec3 e1 = v2 - v1;
  const glm::vec3 e2 = v3 - v1;

  const glm::vec3 D = ray->getDirection();

  // // Backface culling
  normal = glm::normalize(glm::cross(e1, e2));
  // if( glm::dot(N, -D) <= getEpsilon() ) {
  //   return false;
  // }

  // Begin calculating determinant - also used to calculate u parameter
  const glm::vec3 P = glm::cross(D, e2);

  // If determinant is near zero, ray lies in plane of triangle
  const float det = glm::dot(e1, P);

  // NOT CULLING
  if( det > -EPSILON && det < EPSILON ) {
    return false;
  }

  const glm::vec3 O = ray->getOrigin();

  // Calculate distance from v1 to ray origin
  const glm::vec3 T = O - v1;

  const float inv_det = 1.0f / det;

  // Calculate u parameter and test bound
  const float u = glm::dot(T, P) * inv_det;

  // The intersection lies outside of the triangle
  if( u < 0.0f || u > 1.0f ) {
    return false;
  }

  // Prepare to test v parameter
  const glm::vec3 Q = glm::cross(T, e1);

  // Calculate V parameter and test bound
  const float v = glm::dot(D, Q) * inv_det;

  // The intersection lies outside of the triangle
  if( v < 0.0f || u + v  > 1.0f ) {
    return false;
  }

  const float t = glm::dot(e2, Q) * inv_det;

  if( t > EPSILON ) { //ray intersection
    intersection = O + t * D;
    return true;
  }

  return false;
}
