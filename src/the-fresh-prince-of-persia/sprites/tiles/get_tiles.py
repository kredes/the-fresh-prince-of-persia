from PIL import Image, ImageFilter
import os
import sys

class TileExtractor(object):
    
    def __init__(self, path, x0, y0, tile_width, tile_height):
        self.counter = 1
        
        self.image = Image.open(path)
        self.x0 = x0
        self.y0 = y0
        self.tile_width = tile_width
        self.tile_height = tile_height
        
        pass
        
    def extract(self):
        x = self.x0
        y = self.y0
        
        i = 0
        while x + self.tile_width <= self.image.width:
            
            while y + self.tile_height <= self.image.height:
                box = (x, y, x+self.tile_width, y+self.tile_height)
                tile = self.image.crop(box)
                print(box)
                tile.save('./tiles/' + str(self.counter) + '.tile.png', format='PNG')
                
                self.counter += 1
                
                y += self.tile_height
            
            x += self.tile_width
            y = self.y0
			
	def merge(self):
		tiles = []
		for filename in os.listdir(self.path):
			if filename.endswith('.tile.png'):
				tile = Image.open(self.path + '/' + filename)
				tiles.append(tile)
				
		tileset = Image.new('RGB', (len(tiles) * self.tile_width, self.tile_height))
		
		i = 0;
		for x in range(0, tileset.width, self.tile_width):
			for y in range(0, tileset.height, self.tile_height):
				tileset.paste(tiles[i])
				i += 1
				
		tileset.show();
		
		
if __name__ == '__main__':
    if sys.argc > 2:
		extractor = TileExtractor(sys.argv[1], 0, 6, 64, 126)
	
		o = sys.argv[2]
		if o == 'extract':
			extractor.extract()
		elif o == 'merge':
			extractor.merge()
		
		print("Done")