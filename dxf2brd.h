#include <dxflib/dl_creationadapter.h>
#include <dxflib/dl_dxf.h>

#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>

#include "polyline.h"

#ifndef DXF2BRD_H
#define DXF2BRD_H

// these defines control the kicad output. All units are mm:
#define LINE_THICKNESS 0.15  // thickness of traces
#define X_OFFSET 13          // x offset of DXF origin in kicad coordinate space
#define Y_OFFSET 198         // y offset of DXF origin in kicad coordinate space
#define LAYER "(layer \"Edge.Cuts\")"  // layer to render output on

// This class contains the callbacks from dxflib only 3 important ones
// matter: the callbacks for lines, circles, and arcs.
class Dxf2BrdFilter : public DL_CreationAdapter {
    // called when a line has been detected
    virtual void addLine(const DL_LineData &d);
    // called when a circle has been detected
    virtual void addCircle(const DL_CircleData &d);
    // called when an arc has been detected
    virtual void addArc(const DL_ArcData &d);
    // called when a polyline beginning has been detected
    virtual void addPolyline(const DL_PolylineData &d);
    // called when a vertex addition to a polyline is detected
    virtual void addVertex(const DL_VertexData &d);

   protected:
    // converts a DXF Coordinate, angle and radius into a KICAD
    // coordinate that is offset by the radius in the direction of
    // angle
    void convertangle(double xin, double yin, double radius, double angle,
                      double &xout, double &yout);
    // converts a DXF coordinate to a KICAD coordinate
    void convert(double xin, double yin, double &xout, double &yout);
    // generate a random 128 bits UUID
    inline void uuid(void);
    // output a named coordinate
    inline void named_coordinate(std::string text, double x, double y);
    // output the stroke definition block
    inline void stroke_info(std::string type);
    // output the layer data block
    inline void layer_info(void);
    // offset (in mm) to insert origin of DXF
    // drawing in KICAD drawing.
    int xoffset;
    int yoffset;
    // layer to draw on in KICAD drawing
    std::string layer;
    // thickness in mm of drawing in Kicad
    double thickness;
    Polyline currentPolyline = Polyline(0);

   public:
    // constructor
    Dxf2BrdFilter(int xoffset = X_OFFSET, int yoffset = Y_OFFSET,
                  std::string layer = LAYER, double thickness = LINE_THICKNESS);
};

#endif