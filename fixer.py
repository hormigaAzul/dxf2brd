import argparse
import subprocess as sub

parser = argparse.ArgumentParser(
    description="Integrate dxf2brd generated code into a Kicad board file")
parser.add_argument('-f', '--fix', dest='fileFix',
                    action='store', help='Kicad file that should be fixed')
parser.add_argument('-i', '--input', dest='fileIn', action='store',
                    help='Kicad file where the edge should be inserted')
parser.add_argument('-d', '--dxf', dest='fileDxf',
                    action='store', help='Board edge DXF file')
args = parser.parse_args()


def fix(arch):
    f = open(arch, encoding='utf-8')
    lines = f.readlines()
    f.close()

    f = open(arch,  mode='w', encoding='utf-8')
    line_buffer = None
    skip_line = False
    for l in lines:
        if "(embedded_fonts" in l and line_buffer is None:
            line_buffer = l
            skip_line = True
        else:
            if not skip_line:
                f.write(l)
            else:
                skip_line = False
    f.write(line_buffer)
    f.write(')\n')
    f.close()


def generate(inFile, dxf):
    sub.check_call(["./dxf2brd " + dxf + " >> " + inFile], shell=True)
    fix(inFile)


if args.fileFix is not None:
    fix(args.fileFix)
elif (args.fileIn is not None and args.fileDxf is not None):
    generate(args.fileIn, args.fileDxf)
else:
    sub.call(["python3 fixer.py -h"], shell=True)
