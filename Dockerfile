# Build Stage
FROM --platform=linux/amd64 ubuntu:20.04 as builder

## Install build dependencies.
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y git clang

## Add source code to the build stage.
WORKDIR /
ADD https://api.github.com/repos/capuanob/blurhash/git/refs/heads/mayhem version.json
RUN git clone -b mayhem https://github.com/capuanob/blurhash.git
WORKDIR blurhash

## Build
RUN clang -fsanitize=fuzzer-no-link -fPIC -shared -o encode.so ext/blurhash/encode.c
RUN clang -fsanitize=fuzzer fuzz/fuzz.c encode.so -I ext/blurhash/ -o blurhash-fuzz

## Consolidate all dynamic libraries used by the fuzzer
RUN mkdir /deps
RUN cp `ldd blurhash-fuzz | grep so | sed -e '/^[^\t]/ d' | sed -e 's/\t//' | sed -e 's/.*=..//' | sed -e 's/ (0.*)//' | sort | uniq` /deps 2>/dev/null || :

## Package Stage
FROM --platform=linux/amd64 ubuntu:20.04
COPY --from=builder /blurhash/blurhash-fuzz /blurhash-fuzz
COPY --from=builder /blurhash/encode.so /usr/lib
COPY --from=builder /deps /usr/lib

## Set up fuzzing!
ENTRYPOINT []
CMD /blurhash-fuzz -close_fd_mask=2
