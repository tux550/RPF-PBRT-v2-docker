DOCKER_IMAGE_NAME = pbrt-compile-env

shell:
	docker run -it --rm -v $(PWD):/workspace $(DOCKER_IMAGE_NAME) /bin/bash

docker:
	docker build -t $(DOCKER_IMAGE_NAME) .