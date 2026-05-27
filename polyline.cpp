#include "polyline.h"

// Helper class to store point data for line generation. Kicad requires two
// coordinates to be specified on each line command, the start and finish point.
// However, polylines in dxf are defined via vertices, specifying one coordinate
// at a time. To translate that into something that Kicad understands, a small
// "stack" is implemented that remembers the last vertex that was read. This
// way, a line can be drawn from the previous vertex to the current one.
Polyline::Polyline(unsigned int num_vertices, unsigned int flags) {
    _open = false;
    _num_segments = num_vertices;
    _last = Vertex();
    _flags = flags;
}

// Store a new point in the "stack". If the polyline is currently marked as
// closed, mark it as open.
// Return true if the received vertex should be the last one of the polyline.
bool Polyline::addPoint(double x, double y) {
    if (!_open) {
        // This transition indicates this is the first vertex
        _open = true;
    }
    _last = Vertex(x, y);
    _num_segments--;
    return _num_segments == 0;
}

bool Polyline::isOpen() { return _open; }

// Returns true if the polyline is a closed geometry
bool Polyline::isClosedGeometry(void) const { return _flags & CLOSED_FLAG; }

Vertex Polyline::getLastVertex(void) const { return _last; }

Vertex::Vertex(double x, double y) : _x(x), _y(y) {}

// Vertex class Methods

double Vertex::x(void) const { return _x; }

double Vertex::y(void) const { return _y; }