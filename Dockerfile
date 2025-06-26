FROM debian AS builder

ADD http http

RUN apt update && apt install -y make gcc
RUN cd http && make

FROM postgres

COPY --from=builder http/server http/server
COPY --from=builder http/json.o http/json.o
COPY --from=builder http/main.o http/main.o
COPY --from=builder http/repository.o http/repository.o
COPY --from=builder http/sql.o http/sql.o
COPY --from=builder http/httpd.o http/httpd.o

ADD composite-entrypoint.sh /usr/bin/composite-entrypoint.sh

ENTRYPOINT [ "composite-entrypoint.sh" ]