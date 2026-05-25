class Vertex {
   private:
    double _x;
    double _y;

   public:
    Vertex();
    Vertex(double x, double y);
    double x(void) const;
    double y(void) const;
};

class Polyline {
   protected:
    bool _open;
    unsigned int _num_segments;
    Vertex _last = Vertex(0, 0);
    Vertex _first = Vertex(0, 0);

   public:
    // Constructor
    Polyline(unsigned int num_vertices);
    // Store the X, Y coordinates of a polyline point
    bool addPoint(double x, double y);
    // Check if the polyline is open and therefore contains valid data
    bool isOpen(void);
    // Get the last vertex stored
    Vertex getLastVertex(void) const;
    // Get the initial Vertex of a polyline
    Vertex getInitialVertex(void) const;
};