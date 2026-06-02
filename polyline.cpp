#include "polyline.h"

// Helper class to store point data for polyline generation. Dxf defines
// polylines as a header with polyline properties and a sequence of vertices
// with (X,Y) coordinates and an optional bulge value which indicates if a
// coordinate belongs to a curve. If the polyline is defined as closed and there
// are no curves, a direct translation to a Kicad polygon can be performed. If
// the polyline is not closed or contains curves, it must be represented in
// Kicad as a sequence of lines and arcs. Since all the vertices must be
// analyzed to detect curves, their values are stored in a FIFO structure and
// retrieved sequentially once a decision is reached.
Polyline::Polyline(unsigned int num_vertices, unsigned int flags) {
    _num_segments = num_vertices;
    _flags = flags;
    _has_curves = false;
    _vertices.clear();
    _vertices.shrink_to_fit();
    _vertices.reserve(num_vertices);
}

// Store a new point in the structure.
// Return true if the received vertex should be the last one of the polyline.
bool Polyline::addPoint(double x, double y, double bulge) {
    if (bulge != 0.0) _has_curves = true;
    // Don't expand the vector if a malformed dxf contains too many vertices
    if (_vertices.size() < _vertices.capacity())
        _vertices.push_back(Vertex(x, y, bulge));
    return _vertices.size() == _vertices.capacity();
}

// Returns the number of vertices comprising the polyline
int Polyline::size(void) const { return _vertices.capacity(); }

// Returns true if the polyline structure contains any points
bool Polyline::isInited() { return _vertices.size() > 0; }

// Returns true if the polyline is a closed geometry
bool Polyline::isClosedGeometry(void) const { return _flags & CLOSED_FLAG; }

// Returns true if the polyline contains any vertex with non-zero bulge
bool Polyline::hasCurves(void) const { return _has_curves; }

// Removes and returns the "oldest" vertex in the polyline structure. If there
// are no elements in the structure, it returns a Vertex with coordinates (0,0)
// and bulge = 0.
Vertex Polyline::popFirst(void) {
    if (_vertices.size() == 0) {
        return Vertex();
    }
    Vertex f = _vertices.front();
    _vertices.erase(_vertices.begin());
    return f;
}

Vertex::Vertex(double x, double y, double b) : _x(x), _y(y), _bulge(b) {}

// Vertex class Methods

double Vertex::x(void) const { return _x; }

double Vertex::y(void) const { return _y; }

double Vertex::bulge(void) const { return _bulge; }