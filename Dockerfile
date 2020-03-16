# docker build -t allangcruz/ubuntu:latest -f Dockerfile .
# docker push allangcruz/ubuntu:latest
# docker run -it --rm --name my-container-ubuntu --volume $(pwd):/workspace allangcruz/ubuntu:latest /bin/sh
FROM ubuntu:14.04

RUN alias ll='ls -lha'

RUN apt-get update && apt-get install -y gcc make 