import argparse
import subprocess as sub

parser = argparse.ArgumentParser(description="Integrate dxf2brd generated code into a Kicad board file")
parser.add_argument('-f', '--fix', dest='fileFix', action='store', help='Kicad file that should be fixed')
parser.add_argument('-i', '--input', dest='fileIn', action='store', help='Kicad file where the edge should be inserted')
parser.add_argument('-d', '--dxf', dest='fileDxf', action='store', help='Board edge DXF file')
args = parser.parse_args()

def analyze(line):
    count = 0
    for c in line:
        if c == '(':
            count = count + 1
        elif c == ')':
            count = count - 1
    return count

def fix(arch):
    f = open(arch)
    lineas = f.readlines();
    f.close()

    f = open(arch, 'w')
    unclosed = 0
    for l in lineas:
        ans = analyze(l)
        if(ans == 0):
            f.write(l)
        else:
            unclosed = unclosed + ans
            if unclosed != 0:
                f.write(l)
    f.write(')\n\n')
    f.close()

def generate(inFile, dxf):
    sub.check_call(["./dxf2brd " + dxf + " >> " + inFile], shell = True)
    fix(inFile)

if(args.fileFix!=None):
    fix(args.fileFix)
elif(args.fileIn!=None and args.fileDxf!=None):
    generate(args.fileIn, args.fileDxf)
else:
    sub.call(["python fixer.py -h"], shell = True)
