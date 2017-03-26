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
tileset_height = max(max_height, len(tiles)//12 * max_height)
        
tileset = Image.new('RGBA', (tileset_width, tileset_height))

print("Tileset width: {}, Tileset height: {}".format(tileset_width, tileset_height))

x = 0
y = 0
for i, tile in enumerate(tiles):                
    tileset.paste(tile, (x, y))
    
    print("x = " + str(x) + ", tile width = " + str(tile.width))
    
    x += 114
    
    
    
    if (i > 0 and i % 12 == 0):
        x = 0
        y += max_height

tileset.save('tileset.png', format='PNG')