#!/usr/bin/env python3
import math, struct, sys, zlib

W = H = 256
rows = []
for y in range(H):
    row = bytearray([0])
    for x in range(W):
        r = 48
        cx = min(max(x, r), W - 1 - r)
        cy = min(max(y, r), H - 1 - r)
        if (x - cx) ** 2 + (y - cy) ** 2 > r * r:
            row += bytes((0, 0, 0, 0))
            continue
        t = y / (H - 1)
        R, G, B = int(40 + 30 * t), int(60 + 50 * t), int(120 + 60 * t)
        d = math.hypot(x - 128, y - 128)
        if 36 <= d < 70:
            R, G, B = 240, 244, 250
        row += bytes((R, G, B, 255))
    rows.append(bytes(row))

def chunk(tag, data):
    c = struct.pack(">I", len(data)) + tag + data
    return c + struct.pack(">I", zlib.crc32(tag + data) & 0xFFFFFFFF)

png = (b"\x89PNG\r\n\x1a\n"
       + chunk(b"IHDR", struct.pack(">IIBBBBB", W, H, 8, 6, 0, 0, 0))
       + chunk(b"IDAT", zlib.compress(b"".join(rows), 9))
       + chunk(b"IEND", b""))
open(sys.argv[1] if len(sys.argv) > 1 else "icon.png", "wb").write(png)
