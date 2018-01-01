import smopy
from PIL import Image

zoom_level = 9

map = smopy.Map((38.999, -76.999, 39, -77), z=zoom_level)
map.save_png("test.png")

x, y = map.to_pixels(38.999, -76.995)
ax = map.show_mpl(figsize=(8, 6))
ax.plot(x, y, 'or', ms=10, mew=2)

image_file = "test.png"
im = Image.open(image_file)
# print im.size

resolution = smopy.determine_resolution(38.999, zoom_level)
scale = smopy.determine_scale(96, 38.999, zoom_level) / 100

print "resolution:", resolution, "m/p"
print "scale: 1 cm:", scale, "m"
print "size:", int(resolution * im.size[0]), "m by", int(resolution * im.size[1]), "m"
