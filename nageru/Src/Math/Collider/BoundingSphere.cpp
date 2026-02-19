#include "BoundingSphere.h"

BoundingSphere::BoundingSphere(float radius, const GSvector3& center) :
	radius{ radius }, center{ center } {

}

BoundingSphere BoundingSphere::translate(const GSvector3& position) const {
	return BoundingSphere{ radius, center + position };
}

BoundingSphere BoundingSphere::transform(const GSmatrix4& matrix) const {
	return BoundingSphere{ radius * matrix.getScale().y, matrix.transform(center) };
}

bool BoundingSphere::intersects(const BoundingSphere& other) const {
	return gsCollisionSphereAndSphere(
		&center, radius, &other.center, other.radius) == GS_TRUE;
}

void BoundingSphere::draw() const {
	glPushMatrix();
	glTranslatef(center.x, center.y, center.z);
	glutWireSphere(radius, 16, 16);
	glPopMatrix();
}