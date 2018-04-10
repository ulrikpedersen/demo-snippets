FROM alpine:3.7

WORKDIR /root
ADD . /root/demo-snippets/

RUN apk add --no-cache g++ musl-dev boost-dev cmake make
RUN mkdir build && cd build &&\
    cmake /root/demo-snippets &&\
    make &&\
    ./bin/cli

