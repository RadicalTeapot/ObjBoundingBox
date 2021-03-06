#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iterator>
#include <vector>

#include "BoundingBox.h"

namespace objfiles {

struct Face {
	Vertex *first, *second, *third;
};

std::vector<std::string> split(const std::string& s) {
	// Split the given string along spaces
	std::istringstream string_stream(s);
	return std::vector<std::string>(
		std::istream_iterator<std::string>(string_stream),
		std::istream_iterator<std::string>()
		);
}

bool read_file(const std::string& path,
			   std::vector<Vertex>* vertices, std::vector<Face>* faces) {
	// The two chars used as a vertex declaration identifier in the obj file
	std::string vertex = "v ";
	// The two chars used as a face declaration identifier in the obj file
	std::string face = "f ";

	std::ifstream obj_file(path);

	std::string line;

	if (obj_file.is_open()) {
		while (getline(obj_file, line)) {
			// Check if line is a vertex declaration
			if (line.compare(0, 2, vertex) == 0) {
				// Extract the vertex position from the line
				std::vector<std::string> values = split(line);
				vertices->push_back(Vertex {
					atof(values[1].c_str()),
					atof(values[2].c_str()),
					atof(values[3].c_str())
				});
			}
			// Check if line is a face declaration
			else if (line.compare(0, 2, face) == 0) {
				// Extract the vertex indices from the line
				std::vector<std::string> values = split(line);

				// The index is assumed to exists for performance reasons
				// Vector->at(index) could be used if that is not the case
				// Subtract one to the index as vertex indices are 1-based in the obj file
				faces->push_back(Face {
					&(*vertices)[atoi(values[1].c_str()) - 1],
					&(*vertices)[atoi(values[2].c_str()) - 1],
					&(*vertices)[atoi(values[3].c_str()) - 1]
				});
			}
		}
		obj_file.close();
		return true;
	}

	return false;
}

double get_area(const Face &face) {
	// Vector representing the first side
	double first_side[3] {
		face.first->x - face.second->x,
		face.first->y - face.second->y,
		face.first->z - face.second->z
	};

	double first_side_length = sqrt(
		first_side[0] * first_side[0] +
		first_side[1] * first_side[1] +
		first_side[2] * first_side[2]
	);

	// Vector representing the second side
	double second_side[3] {
		face.first->x - face.third->x,
		face.first->y - face.third->y,
		face.first->z - face.third->z
	};

	double second_side_length = sqrt(
		second_side[0] * second_side[0] +
		second_side[1] * second_side[1] +
		second_side[2] * second_side[2]
	);

	// Dot product of the two vectors
	double dot = (
		first_side[0] * second_side[0] +
		first_side[1] * second_side[1] +
		first_side[2] * second_side[2]
		);

	// Use the formula above to get the height of the triangle
	double h = first_side_length * sin(
		acos(dot / (first_side_length * second_side_length))
	);

	return second_side_length * h / 2.0;
}

double get_volume(const Face &face) {
	// Don't use absolute values as we need the signed volume
	double cross[3] {
		face.second->y * face.third->z - face.second->z * face.third->y,
		face.second->z * face.third->x - face.second->x * face.third->z,
		face.second->x * face.third->y - face.second->y * face.third->x
	};

	double dot = (
		face.first->x * cross[0] +
		face.first->y * cross[1] +
		face.first->z * cross[2]
		);

	return dot / 6.0;
}

}  // namespace objfiles

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cout << "Usage: obj_files.exe path/to/file.obj" << '\n';
		return -1;
	}

	// Read the file and store it's data
	std::vector<objfiles::Vertex> vertices;
	std::vector<objfiles::Face> faces;
	bool success = read_file(argv[1], &vertices, &faces);

	if (!success)
		return -1;

	// Compute the total area and volume
	double area = 0.;
	double volume = 0.;
	for (const objfiles::Face& face : faces) {
		area += get_area(face);
		volume += get_volume(face);
	}

	// Results output

	std::cout << "Point count : " << vertices.size() << '\n';
	std::cout << "Triangle count : " << faces.size() << '\n';

	std::cout << '\n';

	std::cout << "Area : " << area << '\n';
	std::cout << "Volume : " << volume << '\n';

	std::cout << '\n';

	objfiles::BoundingBox bbox = objfiles::BoundingBox(vertices);
	std::cout << "BBox:" << '\n';
	objfiles::Vertex min = bbox.get_min();
	std::cout << "\tMin : " << min.x << ' ' << min.y << ' ' << min.z << '\n';
	objfiles::Vertex max = bbox.get_max();
	std::cout << "\tMax : " << max.x << ' ' << max.y << ' ' << max.z << '\n';
	objfiles::Vertex size = bbox.get_size();
	std::cout << "\tSize : " << size.x << ' ' << size.y << ' ' << size.z << '\n';
	objfiles::Vertex center = bbox.get_center();
	std::cout << "\tCenter : " << center.x << ' ' << center.y << ' ' << center.z << '\n';

	std::cout << '\n';

	objfiles::Vertex test_position { 0, 0, 0 };
	std::cout << "Contains point {0 0 0} : " << bbox.contains_point(test_position) << '\n';
	test_position.x = -10;
	std::cout << "Contains point {-10 0 0} : " << bbox.contains_point(test_position) << '\n';

	std::cout << '\n';

	objfiles::BoundingBox other(objfiles::Vertex { -.25, -.25, -.25 }, objfiles::Vertex { .25, .25, .25 });
	std::cout << "Contains bbox {-.25 -.25 -.25} {.25 .25 .25} : " << bbox.contains_bbox(other) << '\n';
	std::cout << "Intersects bbox {-.25 -.25 -.25} {.25 .25 .25} : " << bbox.intersects_bbox(other) << '\n';
	other.set_min(objfiles::Vertex { -10, -10, -10 });
	std::cout << "Contains bbox {-10 -10 -10} {.25 .25 .25} : " << bbox.contains_bbox(other) << '\n';
	std::cout << "Intersects bbox {-10 -10 -10} {.25 .25 .25} : " << bbox.intersects_bbox(other) << '\n';
	other.set_max(objfiles::Vertex { -9, -9, -9 });
	std::cout << "Contains bbox {-10 -10 -10} {-9 -9 -9} : " << bbox.contains_bbox(other) << '\n';
	std::cout << "Intersects bbox {-10 -10 -10} {-9 -9 -9} : " << bbox.intersects_bbox(other) << '\n';

	return 0;
}
