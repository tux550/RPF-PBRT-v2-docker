# Utilities for visualizing and converting the generated .exr
# Required the ImageMagick package to be installed
# > sudo apt-get install imagemagick

TARGET_FOLDER = rpf


convert: # Convert .exr to .png
	@echo "Converting .exr to .png"
	for file in $(TARGET_FOLDER)/*.exr; do \
		convert $$file $${file%.exr}.png; \
	done

clean: # Remove .png files
	@echo "Removing .png files"
	rm -f $(TARGET_FOLDER)/*.png