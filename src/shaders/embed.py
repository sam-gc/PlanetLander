import sys

fname = sys.argv[1]

f = open(fname, 'r')
lines = f.readlines()
f.close()

o = open(fname.split('.')[0] + '__gen.h', 'w')
for line in lines:
    o.write('"' + line.strip() + '\\n"\n')

o.close()
