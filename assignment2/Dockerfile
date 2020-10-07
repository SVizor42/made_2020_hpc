FROM ubuntu:14.04
RUN mkdir /matmul
WORKDIR /home/matmul
COPY . /home/matmul/
RUN sudo apt-get update && apt-get install -y g++ bash make
