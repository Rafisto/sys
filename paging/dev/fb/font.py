import subprocess
import shutil
import sys

# to generate font.h file from font.psfu.gz file
# 1. copy font.psfu.gz -- cp /usr/share/kbd/consolefonts/lat1-12.psfu.gz font.psfu.gz
# 2. run this script -- python font.py

# psf2txt font reference example
# %
# // Character 65
# Bitmap: -------- \
#         --###--- \
#         -##-##-- \
#         ##---##- \
#         ##---##- \
#         ##---##- \
#         #######- \
#         ##---##- \
#         ##---##- \
#         ##---##- \
#         -------- \
#         --------
# Unicode: [00000041];
# %

L_WIDTH = 8
L_OFFSET = 8
L_HEIGHT = 12


def txt2carray(txt_file, c_file):
    with open(txt_file, "r") as f:
        lines = f.readlines()

    bitmaps = {}

    for i in range(len(lines)):
        if lines[i].startswith("// Character"):
            bitmap = []
            for j in range(L_HEIGHT):
                line = lines[i+j+1][L_OFFSET:L_OFFSET+L_WIDTH].rstrip('\n')
                bitmap.append(line.replace("#", "1").replace("-", "0"))

            bitmaps[int(lines[i].split()[2])] = bitmap
            i += L_HEIGHT

    with open(c_file, "w") as f:
        f.write("#ifndef FONT_H\n")
        f.write("#define FONT_H\n\n")
        f.write(f"#define FONT_WIDTH  {L_WIDTH}\n")
        f.write(f"#define FONT_HEIGHT {L_HEIGHT}\n\n")
        f.write("unsigned char font_data[256][16] = {\n")

        for char_code in range(256):
            if char_code in bitmaps:
                bitmap = bitmaps[char_code]
                f.write("    { " + ", ".join(f"0x{int(row, 2):02X}" for row in bitmap) +
                        " }, " + f"// Character 0x{char_code:02X}\n")
            else:
                f.write("    { " + ", ".join("0x00" for _ in range(L_HEIGHT)
                                             ) + " }, " + f"// Character 0x{char_code:02X}\n")

        f.write("};\n\n")
        f.write("#endif // FONT_H\n")


def main():
    if not shutil.which("psf2txt"):
        print("psf2txt is not installed. Please install it to proceed.")
        sys.exit(1)

    try:
        with open("font.psfu", "wb") as out_file:
            subprocess.run(["gunzip", "-c", "font.psfu.gz"],
                           stdout=out_file, check=True)

        with open("font.txt", "w") as out_file:
            subprocess.run(["psf2txt", "font.psfu"],
                           stdout=out_file, check=True)

        txt2carray("font.txt", "font.h")

        print("Font conversion completed successfully.")
    except subprocess.CalledProcessError as e:
        print(f"An error occurred: {e}")
        sys.exit(1)


if __name__ == "__main__":
    main()
