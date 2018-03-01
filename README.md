# Bounding Box for .obj

Parse an obj file and build its bounding box as well as getting its total area and volume.

## Bounding Box

The bounding box has the following methods:
 - get_min: Return the min corner value.
 - get_max: Return the max corner value.
 - get_size: Return the size.
 - get_center: Return the center.
 - contains_point: Check if a point is contained in the bounding box.
 - contains_bbox: Check if another bounding box is contained within the bounding box.
 - intersects_bbox: Check if another bounding box is intersecting the bounding box.

It can be built either by specifying its min and max corners or by scanning a list of points for the min and max corners.

## Area

Basic trigonometry is used here, this is prefered to heron's formula as this method uses only 2 sqrt vs 4 for heron's formula.

Basic math rundown:

```
     third
      /|\
     / | \
    /  |h \
   /___|___\
first      second

area = length(first - third) * h / 2

sin(angle) = h / length(first - third)
dot = length(first - second) * length(first - third) * cos(angle)

=> h = length(first - second) * sin(acos(dot / (length(first - second) * length(first - third))))
```

This could be improved CPU wise by caching already computed edge lengths to avoid un-necessary sqrts.

## Volume

The volume of a triangulated mesh is the sum the signed volumes of tetrahedrons built from each face with a 4th point at 0, 0, 0 (from google searches).

The volume of a tetraherdon is (https://en.wikipedia.org/wiki/Tetrahedron#Volume):
```
1/6 * abs(dot(first, cross(second, third)))
```

The expanded formula is:
```
dot(a, cross(b, c)) = a.x*b.y*c.z - a.x*b.z*c.y  + a.y*b.z*c.x - a.y*b.x*c.z  + a.z*b.x*c.y  - a.z*b.y*c.x
```

The non expanded version is prefered here for readability.
