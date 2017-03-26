from PIL import Image, ImageFilter, ImageDraw
import os



tiles = []
for filename in os.listdir():
    #if filename.endswith('.tile.png'):
    if (filename.endswith('.png') and filename != "tileset.png"):
        tile = Image.open(filename)
        tiles.append(tile)


max_height = 0
max_width = 0
for tile in tiles:
    if (tile.height > max_height): max_height = tile.height
    if (tile.width > max_width): max_width = tile.width
    

print("max_width: {}, max_height: {}".format(max_width, max_height))


tileset_width = 12*max_width
tileset_height = max(max_height, (int(round(len(tiles)/12)) + 1) * max_height)
        
tileset = Image.new('RGBA', (tileset_width, tileset_height))

print("Tileset width: {}, Tileset height: {}".format(tileset_width, tileset_height))

print(int(round(len(tiles)/12)))

x = 0
y = 0
for i, tile in enumerate(tiles):                
    if (x + max_width > tileset.width):
        x = 0
        y += max_height
    
    tileset.paste(tile, (x, y))
    
    print("x = " + str(x) + ", tile width = " + str(tile.width))
    
    x += max_width
    
    

tileset.save('tileset.png', format='PNG')