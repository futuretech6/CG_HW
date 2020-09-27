from PIL import Image
img = Image.open("./pic/test.ppm")
img.show()
img.save("./pic/test.png")
