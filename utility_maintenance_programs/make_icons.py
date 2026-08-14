#!/usr/bin/env python3
"""Regenerate every derived BASIC256 icon from the master logo.

    python utility_maintenance_programs/make_icons.py     (needs Pillow)

Master   resources/images/logo_300x300.png     square, any size, white background

Derived
    resources/images/logo_300x300.ico   master ICO, 16..256, kept for reference
    resources/icons/basic256.ico        Windows .exe icon (resources/*.rc)
    resources/icons/basic256.png        Qt resource :icons/basic256.png -- window,
                                        taskbar and About-dialog title-bar icon,
                                        and the AppImage/.desktop icon
    resources/icons/basic256_256.png    the same icon at 256, so the About dialog
                                        and a HiDPI taskbar have something to
                                        scale down from rather than up
    wasm-deploy/favicon.png             browser favicon for the web IDE
    wasm-deploy/logo.png                splash image on the WASM loading screen
    resources/basic256.icns             macOS bundle icon
    resources/images/basic256.bmp       NSIS installer welcome-page bitmap

The master is drawn on an opaque white page.  An icon has to sit on whatever
the taskbar, Dock or dark-mode page behind it happens to be, so the white is
flood-filled away from the edges and the one- or two-pixel anti-aliased rim is
un-blended from white back to a real alpha ramp.  Everything is then cropped to
the artwork, so the badge fills its frame instead of floating in a margin.

The .bmp is the exception: NSIS statics cannot do alpha, so it is flattened
onto the Windows dialog face colour instead.
"""

import io
import os
import struct
from collections import deque

from PIL import Image, ImageChops, ImageFilter

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
MASTER = os.path.join(ROOT, "resources", "images", "logo_300x300.png")

# Anything at or above this on all three channels counts as page white.
WHITE = 240
# Width in pixels of the anti-aliased rim that gets un-blended from white.
RIM = 2

# Sizes in the .ico.  Vista and later read PNG-compressed entries, but only the
# 256 one is stored that way -- the smaller entries stay classic DIBs, which is
# what every Windows resource compiler and shell version understands.
ICO_SIZES = [16, 24, 32, 48, 64, 72, 96, 128, 256]

# PNG element types macOS reads, minus ic10 (1024): the artwork has no detail
# that survives only at 1024, and it alone was over half the file.
ICNS_TYPES = [
    (b"icp4", 16),    # 16x16
    (b"icp5", 32),    # 32x32
    (b"ic11", 32),    # 16x16@2x
    (b"ic12", 64),    # 32x32@2x
    (b"ic07", 128),   # 128x128
    (b"ic13", 256),   # 128x128@2x
    (b"ic08", 256),   # 256x256
    (b"ic14", 512),   # 256x256@2x
    (b"ic09", 512),   # 512x512
]

# macOS draws app icons inside a margin rather than edge to edge.
ICNS_FILL = 0.82

# COLOR_BTNFACE on Windows 10/11 -- the NSIS custom page behind the bitmap.
NSIS_FACE = (240, 240, 240)
NSIS_SIZE = 128


def out(*parts):
    return os.path.join(ROOT, *parts)


def cut_out_background(im):
    """White page -> alpha, cropped to the artwork and re-centred on a square."""
    w, h = im.size
    px = im.load()

    # Flood fill the page white inwards from the border.  A fill (rather than
    # "every white pixel") keeps white *inside* the artwork -- the lettering --
    # fully opaque.
    outside = bytearray(w * h)
    queue = deque()

    def seed(x, y):
        r, g, b, _ = px[x, y]
        if r >= WHITE and g >= WHITE and b >= WHITE and not outside[y * w + x]:
            outside[y * w + x] = 1
            queue.append((x, y))

    for x in range(w):
        seed(x, 0)
        seed(x, h - 1)
    for y in range(h):
        seed(0, y)
        seed(w - 1, y)
    while queue:
        x, y = queue.popleft()
        for nx, ny in ((x - 1, y), (x + 1, y), (x, y - 1), (x, y + 1)):
            if 0 <= nx < w and 0 <= ny < h:
                seed(nx, ny)

    mask = Image.frombytes("L", (w, h), bytes(bytearray(v * 255 for v in outside)))
    # The rim is the artwork-side neighbourhood of the page: those pixels are
    # blends of edge colour and white, so their whiteness *is* their alpha.
    rim = ImageChops.subtract(mask.filter(ImageFilter.MaxFilter(2 * RIM + 1)), mask)

    result = im.copy()
    rpx = result.load()
    for i, (m, edge) in enumerate(zip(mask.tobytes(), rim.tobytes())):
        x, y = i % w, i // w
        if m:
            rpx[x, y] = (0, 0, 0, 0)
        elif edge:
            r, g, b, _ = rpx[x, y]
            lum = (r * 299 + g * 587 + b * 114) // 1000
            a = 255 - lum
            if a <= 0:
                rpx[x, y] = (0, 0, 0, 0)
            elif a < 255:
                # Undo the blend against white: observed = c*a + 255*(1-a).
                un = lambda c: max(0, min(255, ((c - (255 - a)) * 255) // a))
                rpx[x, y] = (un(r), un(g), un(b), a)

    result = result.crop(result.getchannel("A").getbbox())
    side = max(result.size)
    square = Image.new("RGBA", (side, side), (0, 0, 0, 0))
    square.paste(result, ((side - result.width) // 2, (side - result.height) // 2))
    return square


def scaled(logo, size, fill=1.0):
    inner = max(1, round(size * fill))
    canvas = Image.new("RGBA", (size, size), (0, 0, 0, 0))
    canvas.paste(logo.resize((inner, inner), Image.LANCZOS),
                 ((size - inner) // 2, (size - inner) // 2))
    return canvas


def png_bytes(im):
    buf = io.BytesIO()
    im.save(buf, format="PNG", optimize=True)
    return buf.getvalue()


def dib_bytes(im):
    """One classic 32-bit BITMAPINFOHEADER icon image: bottom-up BGRA + AND mask."""
    w, h = im.size
    px = im.load()
    xor = bytearray()
    for y in range(h - 1, -1, -1):
        for x in range(w):
            r, g, b, a = px[x, y]
            xor += bytes((b, g, r, a))
    stride = ((w + 31) // 32) * 4
    and_mask = bytearray()
    for y in range(h - 1, -1, -1):
        bits = bytearray(stride)
        for x in range(w):
            if px[x, y][3] == 0:
                bits[x >> 3] |= 0x80 >> (x & 7)
        and_mask += bits
    header = struct.pack("<IiiHHIIiiII", 40, w, h * 2, 1, 32, 0,
                         len(xor) + len(and_mask), 0, 0, 0, 0)
    return header + bytes(xor) + bytes(and_mask)


def write_ico(logo, path):
    images = [(s, scaled(logo, s)) for s in ICO_SIZES]
    blobs = [(s, png_bytes(im) if s >= 256 else dib_bytes(im)) for s, im in images]
    offset = 6 + 16 * len(blobs)
    directory = b""
    for s, blob in blobs:
        directory += struct.pack("<BBBBHHII", s % 256, s % 256, 0, 0, 1, 32,
                                 len(blob), offset)
        offset += len(blob)
    write(path, struct.pack("<HHH", 0, 1, len(blobs)) + directory
          + b"".join(b for _, b in blobs))


def write_icns(logo, path):
    elements = b""
    for tag, size in ICNS_TYPES:
        blob = png_bytes(scaled(logo, size, ICNS_FILL))
        elements += struct.pack(">4sI", tag, 8 + len(blob)) + blob
    write(path, struct.pack(">4sI", b"icns", 8 + len(elements)) + elements)


def write_png(im, path):
    write(path, png_bytes(im))


def write_bmp(logo, path):
    flat = Image.new("RGB", (NSIS_SIZE, NSIS_SIZE), NSIS_FACE)
    icon = scaled(logo, NSIS_SIZE)
    flat.paste(icon, mask=icon.getchannel("A"))
    buf = io.BytesIO()
    flat.save(buf, format="BMP")
    write(path, buf.getvalue())


def write(path, data):
    with open(path, "wb") as f:
        f.write(data)
    print("  {:<38} {:>9,} bytes".format(os.path.relpath(path, ROOT), len(data)))


def main():
    master = Image.open(MASTER).convert("RGBA")
    print("master {} {}x{}".format(os.path.relpath(MASTER, ROOT), *master.size))
    logo = cut_out_background(master)
    print("artwork cropped to {}x{}, background transparent".format(*logo.size))

    write_ico(logo, out("resources", "images", "logo_300x300.ico"))
    write_ico(logo, out("resources", "icons", "basic256.ico"))
    write_png(scaled(logo, 64), out("resources", "icons", "basic256.png"))
    write_png(scaled(logo, 256), out("resources", "icons", "basic256_256.png"))
    write_png(scaled(logo, 64), out("wasm-deploy", "favicon.png"))
    write_png(scaled(logo, 300), out("wasm-deploy", "logo.png"))
    write_icns(logo, out("resources", "basic256.icns"))
    write_bmp(logo, out("resources", "images", "basic256.bmp"))


if __name__ == "__main__":
    main()
