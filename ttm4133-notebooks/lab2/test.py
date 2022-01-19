import sys

args = sys.argv

print("running test.py", args)
a = int(args[1])
build = bool(args[2])
dist = float(args[3])

if args[2] == 'True':
    build = True
elif args[2] == 'False':
    build = False
        
    

print(a, build, dist)