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
    bool intersects = triangleIntersection(ray, verticies_[i], verticies_[i+1], verticies_[i+2], intersection);

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


bool TriangleMesh::triangleIntersection(const Ray* ray, 
                                        const glm::vec3 v1, 
                                        const glm::vec3 v2, 
                                        const glm::vec3 v3, 
                                        glm::vec3& intersection) const {

  // Find vectors for two edges sharing v1
  const glm::vec3 e1 = v2 - v1;
  const glm::vec3 e2 = v3 - v1;

  const glm::vec3 D = ray->getDirection();

  // // Backface culling
  // const glm::vec3 N = glm::cross(e1, e2);
  // if( glm::dot(N, D) < 0 ) {
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
