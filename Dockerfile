# Use a base image with C++ tools pre-installed
FROM ubuntu:22.04

# Set non-interactive mode for apt to avoid prompts
ENV DEBIAN_FRONTEND=noninteractive

# Update the package
RUN apt-get update
# C++ tools
RUN apt-get install -y build-essential 
RUN apt-get install -y cmake 
# Install Flex and Bison
RUN apt-get install -y flex 
RUN apt-get install -y bison
# Install OpenEXR + X11
RUN apt-get install -y libopenexr-dev
RUN apt-get install -y libx11-dev
# Tiff not needed
#RUN apt-get install -y libtiff-dev
# Install scons
RUN apt-get install -y scons
# Additional OpenEXR tools
RUN apt-get install -y openexr

# Script to patch openexr installation
# (Replaces __declspec(dllimport) with __attribute__((visibility("default")))
RUN mkdir -p /mypatch
COPY patch.py /mypatch
RUN python3 /mypatch/patch.py

# Install openomp
RUN apt-get install libomp-dev -y

# Set the working directory
WORKDIR /workspace

# Default command: run bash
CMD ["/bin/bash"]