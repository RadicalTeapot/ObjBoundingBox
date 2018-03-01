#pragma once

#include <vector>

namespace obj_tools {

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
		void set_min( const Vertex new_min );

		// Set bounding box max corner
		void set_max( const Vertex new_max );

		// Return bounding box min corner
		Vertex get_min();
		// Return bounding box max corner
		Vertex get_max();
		// Return bounding box center
		Vertex get_center();
		// Return bounding box size
		Vertex get_size();

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

}