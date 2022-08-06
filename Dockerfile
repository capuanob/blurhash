# Build Stage
FROM --platform=linux/amd64 ubuntu:20.04 as builder

## Install build dependencies.
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y clang gcc ruby-dev make

## Add source code to the build stage.
ADD . /blurhash
WORKDIR blurhash

## Build
WORKDIR ext/blurhash
RUN ruby extconf.rb
RUN make
RUN cp encode.so /usr/lib
WORKDIR /blurhash
RUN clang -fsanitize=fuzzer fuzz/fuzz_blur_pixels.c -Lencode -I ext/blurhash/ -L/usr/lib -l:encode.so -o blurhash-fuzz

## Package Stage
FROM --platform=linux/amd64 ubuntu:20.04
COPY --from=builder /blurhash/blurhash-fuzz /blurhash-fuzz
COPY --from=builder /blurhash/ext/blurhash/encode.so /usr/lib

## Set up fuzzing!
ENTRYPOINT []
CMD /blurhash-fuzz
