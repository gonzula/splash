#!/usr/bin/env python3

from PIL import Image
import os

files = os.listdir()
images = [f for f in files if f.endswith('.png')]

target_width = 750//2

for file_name in images:
    image = Image.open(file_name)
    size = image.size
    original_aspect_ratio = size[0]/size[1]
    size = target_width, int(target_width / original_aspect_ratio)
    image = image.resize(size, Image.ANTIALIAS)
    image.save(file_name)

# a = w/h
# h = w/a
