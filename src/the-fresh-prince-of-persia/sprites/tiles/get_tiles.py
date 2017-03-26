from PIL import Image, ImageFilter, ImageDraw
import os
import sys

class TileExtractor(object):
    
    def __init__(self, path, x0, y0, tile_width, tile_height):
        self.counter = 1
        
        if (path): self.image = Image.open(path)
        self.x0 = x0
        self.y0 = y0
        self.tile_width = tile_width
        self.tile_height = tile_height
        
        pass
        
    def extract_tiles(self):
        x = self.x0
        y = self.y0
        
        original_widht = self.tile_width
        self.tile_width *= 1.5
        
        i = 0
        while x + self.tile_width <= self.image.width:
            
            while y + self.tile_height <= self.image.height:
                box = (x, y, x+self.tile_width, y+self.tile_height)
                tile = self.image.crop(box)
                print(box)
                tile.save('./tiles/' + str(self.counter) + '.tile.png', format='PNG')
                
                self.counter += 1
                
                y += self.tile_height
            
            x += original_widht//2
            y = self.y0
			
    def merge(self):
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
        tileset_height = len(tiles)//12 * max_height
        
        if (tileset_height == 0): tileset_height = max_height
        
        tileset = Image.new('RGB', (tileset_width, tileset_height))
        
        print("Tileset width: {}, Tileset height: {}".format(tileset_width, tileset_height))

        x = 0
        y = 0
        for i, tile in enumerate(tiles):
            aux = Image.new('RGB', (max_width, max_height))
            aux.paste(tile, ((aux.width-tile.width)//2, 0))
            
            #draw = ImageDraw.Draw(aux)
            #draw.line(((aux.width-1, 0), (aux.width-1, aux.height-1)), 128, 1)
                        
            tileset.paste(aux, (x, y + max_height-tile.height))
            
            x += aux.width
            
            if (i > 0 and i % 12 == 0):
                x = 0
                y += max_height
        
        tileset.save('tileset.png', format='PNG')

    
    
if __name__ == '__main__':
    if len(sys.argv) > 3:
        extractor = None

        a1 = sys.argv[1]
        a2 = sys.argv[2]
        path = sys.argv[3]
        
        if a1 == "tiles":
            extractor = TileExtractor(path, 0, 6, 64, 126)
            if a2 == "extract":
                extractor.extract_tiles()
            elif a2 == "merge":
                extractor.merge()
        elif a1 == "sprites":
            extractor = TileExtractor(None, 0, 6, 64, 126)
            if a2 == "extract":
                pass
            elif a2 == "merge":
                extractor.merge()
            
        
        '''
        if o == 'extract':
            extractor.extract_tiles()
        elif o == 'merge':
            extractor.merge_sprites()
        '''
        
        print("Done")