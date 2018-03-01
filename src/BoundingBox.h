#ifndef OBJ_FILES_BOUNDINGBOX_H_
#define OBJ_FILES_BOUNDINGBOX_H_

#include <vector>

namespace objfiles {

struct Vertex {
	double x, y, z;
};

class BoundingBox
{
public:
	// Build the bouding box from the specified corners
	BoundingBox(
		const Vertex min, const Vertex max
	);
	// Build the bouding box by scanning the vertex list for the min and max corners
	BoundingBox(
		const std::vector<Vertex>& Vertices
	);

	~BoundingBox();

	// Set bounding box min corner
	inline void set_min( const Vertex new_min ) { _min = new_min; };
	// Set bounding box max corner
	inline void set_max( const Vertex new_max ) { _max = new_max; };

	// Return bounding box min corner
	inline Vertex get_min() { return _min; };
	// Return bounding box max corner
	inline Vertex get_max() { return _max; };
	// Return bounding box center
	inline Vertex get_center() { return _center; };
	// Return bounding box size
	inline Vertex get_size() { return _size; };

	// Return whether the given point is contained in the bbox
	bool contains_point( const Vertex& point );
	// Return whether the given bounding box is contained in the bbox
	bool contains_bbox( const BoundingBox& bbox );
	// Return whether the given bounding box intersects the bbox
	bool intersects_bbox( const BoundingBox& bbox );

private:
	// The min corner
	Vertex _min;
	// The max corner
	Vertex _max;
	// The center
	Vertex _center;
	// The size
	Vertex _size;

	// Get the center and size from the corners
	void set_center_size();
};

}  // namespace objfiles

#endif  // OBJ_FILES_BOUNDINGBOX_H_