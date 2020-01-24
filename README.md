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

* **MANTAINER**: Enrique Condés
* **MAIL**: <enrique@shapeoko.com>

* **AUTHOR**: Andy Goetz
* **MAIL:** <andy@andygoetz.org>
* **PROGRAM:** dxf2brd


This program converts DXF files to kicad's kicad_pcb files. The KiCad
board designer has very primitive support drawing shapes
accurately. You can use this program to create a complex design in
a real cad program, like qcad, and import it into a kicad_pcb file.

## Current status

The program has been updated to generate code compliant with the Pcbnew “S-expression” file format which is implemented on the new version of KiCad. As of 10/19/16, all functions have been updated to the new standard and the function to generate arcs now supports any arc length on any orientation.

## Compiling for Linux
To use this program, you must have dxflib installed. It can be downloaded and compiled from https://github.com/clothbot/dxflib
Then use the command:

```bash
g++ dxf2brd.cpp -o dxf2brd -ldxflib
```

To compile the program.

## Compiling for Windows
To compile and use in Windows, you must download and install MSYS2 (http://sourceforge.net/projects/msys2/).
Download MSYS2 and install it somewhere, then run "MinGW-w64 Win32 Shell" (if you want a i686 binary) or "MinGW-w64 Win64 Shell" (if you want a x86_64 binary). The following commands are for the i686 binary, if you want the x86_64 binary replace all the "/mingw32" with "/mingw64" and all the mingw-w64-i686-* packages with mingw-w64-x86_64-*

    $ pacman -Sy
    $ pacman --needed -S bash pacman pacman-mirrors msys2-runtime
    $ pacman -Su

Close and reopen the shell

    $ pacman -Su
    $ pacman --needed -S base-devel git mingw-w64-i686-gcc

That leaves it MSYS2 ready for compilation. The following commands do the compiling.

    $ git clone https://github.com/clothbot/dxflib.git
    $ cd dxflib
    $ mv src dxflib
    $ cd dxflib
    $ g++ -c dl_dxf.cpp -o dl_dxf.o
    $ g++ -c dl_writer_ascii.cpp -o dl_writer_ascii.o
    $ cd ../..
    $ git clone https://github.com/hormigaAzul/dxf2brd.git
    $ cd dxf2brd
    $ g++ dxf2brd.cpp ../dxflib/dxflib/dl_dxf.o ../dxflib/dxflib/dl_writer_ascii.o -o dxf2brd.exe -I ../dxflib/

Depending on the compilation you did, i686 binary or x86_64 binary, you will need to copy certain files to the dxf2brd folder.

### For i686 binary

    $ cp /mingw32/bin/libgcc_s_dw2-1.dll .
    $ cp /mingw32/bin/libstdc++-6.dll .
    $ cp /mingw32/bin/libwinpthread-1.dll .

### For x86_64 binary

    $ cp /mingw64/bin/libgcc_s_seh-1.dll .
    $ cp /mingw64/bin/libstdc++-6.dll .
    $ cp /mingw64/bin/libwinpthread-1.dll .

That leaves a perfectly functional binary for Windows.

## Running the program
To run the program, execute the following command:

```bash
./dxf2brd some_dxf_file.dxf
```

It will produce kicad_pcb code as its output that should be manually added to the kicad_pcb file. To automatically add the generated code to an existing kicad_pcb file, run the following command:

```bash
./dxf2brd some_dxf_file.dxf >> some_brd_file.kicad_pcb
```

## Fixing the results

After that, some editing is needed.

1. Open some_brd_file.kicad_pcb on any text editor.
2. Add a ')' at the last line.
3. Remove the ')' that is before the generated code.
4. Save the file.

## Fixer script

A python script is now included to automatically perform the corrections described in the previous step or run the whole generation process and output a fixed file. To perform a file fix, the script should be run like this:

```bash
python fixer.py --fix board_file.kicad_pcb
```

To run the whole generation process, it should be run with the following arguments:


```bash
python fixer.py --input board_file.kicad_pcb --dxf some_dxf_file.dxf
```

## DXF input file format

The input file must be in in millimeters and all closed polygons **MUST** be exploded to lines.
