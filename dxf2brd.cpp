#include "dxf2brd.h"

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// MAINTAINER: Enrique Condes
// MAIL: enrique@shapeoko.com

// AUTHOR: Andy Goetz
// MAIL: andy@andygoetz.org
// PROGRAM: dxf2brd

// This program converts DXF files to kicad BRD files.  The KICad
// board designer has very primitive support drawing shapes
// accurately. You can use this program to create a complex design in
// a real cad program, like qcad, and import it into a kicad BRD file.

// Compiling:
// to use this program, you must have dxflib installed.
// then use the command:
//
// g++ polyline.cpp dxf2brd.cpp -o dxf2brd -ldxflib
//
// To compile the program.

// running the program:
// to run the program, execute the following command:
//
// ./dxf2brd some_dxf_file.dxf
//
// It will produce BRD code as its output. to add this code to an existing BRD
// file, run the following command:
//
// ./dxf2brd some_dxf_file.dxf >> some_brd_file.kicad_pcb
//
// After that some editing is needed. Open some_brd_file.kicad_pcb on any text
// editor, add a ')' at the very end and remove the ')' that is before the
// generated code.
void Dxf2BrdFilter::addPolyline(const DL_PolylineData& d) {
    // if (currentPolyline.isOpen()) {
    //    currentPolyline.close();
    // }
    // New instances are closed by default, so no need to close it before.
    currentPolyline = Polyline(d.number, d.flags);
}

void Dxf2BrdFilter::addVertex(const DL_VertexData& d) {
    double x = 0;
    double y = 0;
    Vertex p2;

    convert(d.x, d.y, x, y);
    if (currentPolyline.isOpen()) {
        p2 = currentPolyline.getLastVertex();
        std::cout << "(gr_line (start " << p2.x() << " " << p2.y() << ") (end "
                  << x << " " << y << ") (angle 90) " << layer << " (width "
                  << thickness << "))" << std::endl;
    }

    // Check if this is the last vertex and the trace should be closed
    if (currentPolyline.addPoint(x, y) && currentPolyline.isClosedGeometry()) {
        p2 = currentPolyline.getInitialVertex();
        std::cout << "(gr_line (start " << x << " " << y << ") (end " << p2.x()
                  << " " << p2.y() << ") (angle 90) " << layer << " (width "
                  << thickness << "))" << std::endl;
    }
}

// Lines are the easiest to convert: both kicad and dxf use the same
// format: pairs of points for the start and end.
void Dxf2BrdFilter::addLine(const DL_LineData& d) {
    double x1 = 0;
    double x2 = 0;
    double y1 = 0;
    double y2 = 0;

    convert(d.x1, d.y1, x1, y1);
    convert(d.x2, d.y2, x2, y2);

    std::cout << "\t(gr_line" << std::endl;
    std::cout << "\t\t(start " << x1 << " " << y1 << ")" << std::endl;
    std::cout << "\t\t(end " << x2 << " " << y2 << ")" << std::endl;
    std::cout << "\t\t(stroke" << std::endl;
    std::cout << "\t\t\t(width " << thickness << ")" << std::endl;
    std::cout << "\t\t\t(type default)" << std::endl;
    std::cout << "\t\t)" << std::endl;
    std::cout << "\t\t" << layer << std::endl;
    uuid();
    std::cout << "\t)" << std::endl;
}

// Circles are a bit more complex. Kicad uses two points: one at the
// center, and one on the circumference to describe a circle, while
// DXF uses a center and radius.

void Dxf2BrdFilter::addCircle(const DL_CircleData& d) {
    double cx = 0;
    double cy = 0;
    double xend, yend;
    double rad = d.radius;
    double crad = rad + 150;

    convert(d.cx, d.cy, cx, cy);
    yend = cy;
    xend = cx + rad;

    std::cout << "\t(gr_circle" << std::endl;
    std::cout << "\t\t(center " << cx << " " << cy << ")" << std::endl;
    std::cout << "\t\t(end " << xend << " " << yend << ")" << std::endl;
    std::cout << "\t\t(stroke" << std::endl;
    std::cout << "\t\t\t(width " << thickness << ")" << std::endl;
    std::cout << "\t\t\t(type default)" << std::endl;
    std::cout << "\t\t)" << std::endl;
    std::cout << "\t\t(fill no)" << std::endl;
    std::cout << "\t\t" << layer << std::endl;
    uuid();
    std::cout << "\t)" << std::endl;
}

// Kicad uses two points to describe an arc: the first point is
// located at the center of the arc. The second point is located at
// one terminus of the arc. The other terminus is automatically
// defined by moving -N degrees from the start point.

void Dxf2BrdFilter::addArc(const DL_ArcData& d) {
    double ka1 = d.angle1;
    double ka2 = d.angle2;

    double xstart = 0;
    double ystart = 0;
    double xmid = 0;
    double ymid = 0;
    double xend = 0;
    double yend = 0;

    if (fabs(ka1 - ka2) != 180) {
        if (ka1 == 0) {
            ka1 = 360;
        }
        if (ka1 > ka2 || ka2 == 0) {
            ka2 += 360;
        }
    }
    double angle = ka2 - ka1;
    if (angle >= 0) {
        convertangle(d.cx, d.cy, d.radius, ka2, xstart, ystart);
        convertangle(d.cx, d.cy, d.radius, ka1, xend, yend);
    } else {
        convertangle(d.cx, d.cy, d.radius, ka1, xstart, ystart);
        convertangle(d.cx, d.cy, d.radius, ka2, xend, yend);
    }
    convertangle(d.cx, d.cy, d.radius, angle / 2 + d.angle1, xmid, ymid);

    std::cout << "\t(gr_arc" << std::endl;
    std::cout << "\t\t(start " << xstart << " " << ystart << ")" << std::endl;
    std::cout << "\t\t(mid " << xmid << " " << ymid << ")" << std::endl;
    std::cout << "\t\t(end " << xend << " " << yend << ")" << std::endl;
    std::cout << "\t\t(stroke" << std::endl;
    std::cout << "\t\t\t(width " << thickness << ")" << std::endl;
    std::cout << "\t\t\t(type solid)" << std::endl;
    std::cout << "\t\t)" << std::endl;
    std::cout << "\t\t" << layer << std::endl;
    uuid();
    std::cout << "\t)" << std::endl;
}

// constructor
Dxf2BrdFilter::Dxf2BrdFilter(int xoffset, int yoffset, std::string layer,
                             double thickness)
    : xoffset(xoffset), yoffset(yoffset), layer(layer), thickness(thickness) {}

void Dxf2BrdFilter::convert(double xin, double yin, double& xout,
                            double& yout) {
    xout = xin + xoffset;
    yout = yoffset - yin;
}

void Dxf2BrdFilter::convertangle(double xin, double yin, double radius,
                                 double angle, double& xout, double& yout) {
    convert(xin, yin, xout, yout);

    /*
    x = r cos(theta)
    y = r sin(theta)
    */
    xout += radius * (cos(angle * M_PI / 180.0));
    // Y must be inverted to work with KiCad's drawing logic
    yout -= radius * (sin(angle * M_PI / 180.0));
}

// Generate a random 128 bits UUID concatenating two random numbers genertaes
// using the ms19937 Mersene Twister algorithm
void Dxf2BrdFilter::uuid(void) {
    std::random_device rd;
    std::mt19937_64 generator(rd());
    std::uniform_int_distribution<uint64_t> dist;
    uint64_t lower = dist(generator);
    uint64_t higher = dist(generator);

    uint8_t bytes[16];

    for (uint8_t i = 0; i < 8; ++i) {
        bytes[i] = (lower >> (56 - 8 * i)) & 0xff;
        bytes[i + 8] = (higher >> (56 - 8 * i)) & 0xff;
    }
    bytes[6] = (bytes[6] & 0x0f) | 0x40;
    bytes[8] = (bytes[8] & 0x3f) | 0x80;

    std::cout << "\t\t(uuid \"";
    std::cout << std::hex << std::setfill('0');
    for (uint8_t i = 0; i < 16; ++i) {
        std::cout << std::setw(2) << static_cast<int>(bytes[i]);
        if (i == 3 || i == 5 || i == 7 || i == 9) {
            std::cout << "-";
        }
    }
    std::cout << std::dec << "\")" << std::endl;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Insufficient arguments." << std::endl;
        return -1;
    }
    Dxf2BrdFilter f;
    DL_Dxf* dxf = new DL_Dxf();

    if (!dxf->in(argv[1], &f)) {
        std::cerr << "drawing could not be opened.\n";
    }
    delete dxf;
    return 0;
}
