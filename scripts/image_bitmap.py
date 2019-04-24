import sys
import matplotlib.image as img

image = img.imread(sys.argv[1])

# Convert to Bitmap
bitmap = [['0' for i in range(32)] for j in range(32)]
for x in range(32):
    for y in range(32):
        if(image[x][y][3] > 0):
            bitmap[x][y] = '1'

# print(bitmap)

hexmap = [[0 for i in range(4)] for j in range(32)]
for x in range(32):
    for y in range(0, 32, 8):
        s = ''.join(bitmap[x][y:y+8])
        hexmap[x][int(y / 8)] = int(s, 2)

for x in range(32):
    for y in range(4):
        print('0x%0*X' % (2, hexmap[x][y]), end='')
        if (y != 3):
            print(', ', end='')
    print()
