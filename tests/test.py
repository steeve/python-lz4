import lz4
import sys

DATA = open("/dev/urandom", "rb").read(128 * 1024)  # Read 128kb
if DATA != lz4.loads(lz4.dumps(DATA)):
    sys.exit(1)

# max size
DATA = "x" * 2139095020
if DATA != lz4.loads(lz4.dumps(DATA)):
    sys.exit(1)

# max size + 1
DATA = DATA + "x"
try:
    lz4.dumps(DATA)
    sys.exit(1)
except ValueError:
    pass

sys.exit(0)
