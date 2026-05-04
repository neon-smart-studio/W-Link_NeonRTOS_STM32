import sys

bin_path = sys.argv[1].replace("\\", "/")
out_path = sys.argv[2]

with open(out_path, "w", encoding="utf-8") as f:
    f.write('.section .boot2, "ax"\n')
    f.write('.balign 4\n')
    f.write('.global __boot2_start__\n')
    f.write('__boot2_start__:\n')
    f.write(f'.incbin "{bin_path}"\n')
    f.write('.global __boot2_end__\n')
    f.write('__boot2_end__:\n')