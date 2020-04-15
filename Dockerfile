# Container image that runs your code
FROM archlinux:latest

RUN pacman -Syu --noconfirm ca-certificates-mozilla  && pacman -Syu --noconfirm base-devel cmake fmt libffi gtest

run mkdir -p /root/katla
COPY . /root/katla/

run rm -Rf /root/katla/build
run mkdir -p /root/katla/build
WORKDIR /root/katla/build

run cmake ..
run make

# Code file to execute when the docker container starts up (`entrypoint.sh`)
