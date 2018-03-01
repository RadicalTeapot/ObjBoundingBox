#include "BoundingBox.h"

namespace objfiles {

BoundingBox::BoundingBox(const Vertex min, const Vertex max) : _min(min), _max(max) {
	set_center_size();
}

BoundingBox::BoundingBox(const std::vector<Vertex>& vertices) {
	// Initialize min to max double as it's the max possible value for point location
	_min = Vertex { DBL_MAX, DBL_MAX, DBL_MAX };
	// Initialize max to min double as it's the min possible value for point location
	_max = Vertex { -DBL_MAX, -DBL_MAX, -DBL_MAX };

	// Find the min and max corners
	for (const Vertex& vertex : vertices)
	{
		if (vertex.x < _min.x) _min.x = vertex.x;
		if (vertex.y < _min.y) _min.y = vertex.y;
		if (vertex.z < _min.z) _min.z = vertex.z;

		if (vertex.x > _max.x) _max.x = vertex.x;
		if (vertex.y > _max.y) _max.y = vertex.y;
		if (vertex.z > _max.z) _max.z = vertex.z;
	}

	set_center_size();
}


BoundingBox::~BoundingBox() {}

void BoundingBox::set_center_size() {
	// Center is the average of the corner position
	_center = Vertex {
		(_min.x + _max.x) / 2.0,
		(_min.y + _max.y) / 2.0,
		(_min.z + _max.z) / 2.0
	};

	// Size is the vector from the min to the max corner
	_size = Vertex {
		_max.x - _min.x,
		_max.y - _min.y,
		_max.z - _min.z
	};
}

bool BoundingBox::contains_point(const Vertex& point) {
	return (
		point.x < _max.x && point.x > _min.x &&
		point.y < _max.y && point.y > _min.y &&
		point.z < _max.z && point.z > _min.z
	);
}

bool BoundingBox::contains_bbox(const BoundingBox& bbox) {
	// Bbox is contained if both it's corners are contained
	return (
		contains_point(bbox._min) && contains_point(bbox._max)
	);
}

bool BoundingBox::intersects_bbox(const BoundingBox& bbox) {
	// Check for intersections on the x axis
	bool intersect_x = (
		(_min.x < bbox._min.x && _max.x > bbox._min.x) ||
		(_min.x < bbox._max.x && _max.x > bbox._max.x) ||
		(bbox._min.x < _min.x && bbox._max.x > _min.x) ||
		(bbox._min.x < _max.x && bbox._max.x > _max.x)
		);
	// Check for intersections on the y axis
	bool intersect_y = (
		(_min.y < bbox._min.y && _max.y > bbox._min.y) ||
		(_min.y < bbox._max.y && _max.y > bbox._max.y) ||
		(bbox._min.y < _min.y && bbox._max.y > _min.y) ||
		(bbox._min.y < _max.y && bbox._max.y > _max.y)
		);
	// Check for intersections on the z axis
	bool intersect_z = (
		(_min.z < bbox._min.z && _max.z > bbox._min.z) ||
		(_min.z < bbox._max.z && _max.z > bbox._max.z) ||
		(bbox._min.z < _min.z && bbox._max.z > _min.z) ||
		(bbox._min.z < _max.z && bbox._max.z > _max.z)
		);

	// Intersects if intersections on all axes
	return (intersect_x && intersect_y && intersect_z);
}

}  // namespace objfiles
