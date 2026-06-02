#include <vector>
#define CLOSED_FLAG 1

class Vertex {
   private:
    double _x;
    double _y;
    double _bulge;

   public:
    Vertex(double x = 0, double y = 0, double b = 0);
    double x(void) const;
    double y(void) const;
    double bulge(void) const;
};

class Polyline {
   protected:
    unsigned int _flags;
    unsigned int _num_segments;
    std::vector<Vertex> _vertices;
    bool _has_curves;

   public:
    // Constructor
    Polyline(unsigned int num_vertices = 0, unsigned int flags = 0);
    // Get the expected number of vertices in the polyline
    int size(void) const;
    // Store the (X, Y) coordinates and bulge value of a polyline vertex
    bool addPoint(double x, double y, double bulge);
    // Check if a polyline structure contains valid data
    bool isInited(void);
    // Check the closed flag from the polyline instance in the DXF file
    bool isClosedGeometry(void) const;
    // Check if the polyline contains curves
    bool hasCurves(void) const;
    // Returns a vertex in a FIFO fashion
    Vertex popFirst(void);
};