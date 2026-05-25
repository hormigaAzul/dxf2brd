#define CLOSED_FLAG 1
class Vertex {
   private:
    double _x;
    double _y;

   public:
    Vertex(double x = 0, double y = 0);
    double x(void) const;
    double y(void) const;
};

class Polyline {
   protected:
    bool _open;
    unsigned int _flags;
    unsigned int _num_segments;
    Vertex _last = Vertex();
    Vertex _first = Vertex();

   public:
    // Constructor
    Polyline(unsigned int num_vertices, unsigned int flags = 0);
    // Store the X, Y coordinates of a polyline point
    bool addPoint(double x, double y);
    // Check if the polyline is open and therefore contains valid data
    bool isOpen(void);
    // Check the closed flag from the polyline instance in the DXF file
    bool isClosedGeometry(void) const;
    // Get the last vertex stored
    Vertex getLastVertex(void) const;
    // Get the initial Vertex of a polyline
    Vertex getInitialVertex(void) const;
};