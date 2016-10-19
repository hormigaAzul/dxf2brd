# README

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

* **MANTAINER**: Enrique Condes
* **MAIL**: enrique@shapeoko.com

* **AUTHOR**: Andy Goetz
* **MAIL:** <andy@andygoetz.org>
* **PROGRAM:** dxf2brd


This program converts DXF files to kicad's kicad_pcb files. The KiCad
board designer has very primitive support drawing shapes
accurately. You can use this program to create a complex design in
a real cad program, like qcad, and import it into a kicad_pcb file.

## Current status

The program has been updated to generate code compliant with the Pcbnew “S-expression” file format which is implemented on the new version of KiCad.

It now generates code for circles, lines, and 180° arcs on any orientation, but the code for arcs is still **under development** as of 10/19/16.

## Compiling
To use this program, you must have dxflib installed. It can be downloaded and compiled from https://github.com/clothbot/dxflib
Then use the command:

```bash
g++ dxf2brd.cpp -o dxf2brd -ldxflib
```

To compile the program.

## Running the program
To run the program, execute the following command:

```bash
./dxf2brd some_dxf_file.dxf
```

It will produce kicad_pcb code as its output. To add this code to an existing kicad_pcb file, run the following command:

```bash
./dxf2brd some_dxf_file.dxf >> some_brd_file.kicad_pcb
```
## DXF input file format

The input file must be in in millimeters and all closed polygons **MUST** be exploded to lines.
