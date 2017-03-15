from PIL import Image, ImageFilter
import sys

class TileExtractor(object):
    
    def __init__(self, path, x0, y0, tile_width, tile_height):
        self.counter = 1
        
        self.image = Image.open(path)
        self.x0 = x0
        self.y0 = y0
        self.tile_width = tile_width
        self.tile_height = tile_height
        
        #if self.image.width % tile_width:
            #raise "Can't extract an exact number of tiles from this image"
        
        pass
        
    def extract(self):
        x = self.x0
        y = self.y0
        
        print('img width: ' + str(self.image.width))
        print('img height: ' + str(self.image.height))
        
        
        i = 0
        while x + self.tile_width <= self.image.width:
            print('trying xmax: ' + str(x + self.tile_width))
            
            while y + self.tile_height <= self.image.height:
                print('trying ymax: ' + str(y + self.tile_height))
                box = (x, y, x+self.tile_width, y+self.tile_height)
                tile = self.image.crop(box)
                print("box")
                print(box)
                tile.save('./tiles/' + str(self.counter) + '.png', format='PNG')
                
                self.counter += 1
                
                y += self.tile_height
            
            x += self.tile_width
            y = self.y0
            
if __name__ == '__main__':
    extractor = TileExtractor(sys.argv[1], 0, 6, 64, 126)
    extractor.extract()
    print("Done")