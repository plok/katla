# Container image that runs your code
FROM archlinux:latest

RUN pacman -Sy --noconfirm ca-certificates-mozilla  && pacman -Sy --noconfirm base-devel cmake


COPY . .

run mkdir build
WORKDIR build
run cmake ..
run make

# Code file to execute when the docker container starts up (`entrypoint.sh`)
