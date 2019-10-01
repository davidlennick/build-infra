import sys

with open(sys.argv[1], "rt") as fin:
    with open(sys.argv[2], "wt") as fout:
        for line in fin:
            fout.write(line.replace(sys.argv[3], sys.argv[4]))