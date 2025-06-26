#!/bin/bash

export POSTGRES_DB=jose
export POSTGRES_USER=jose
export POSTGRES_HOST_AUTH_METHOD=trust

docker-entrypoint.sh postgres &
sleep 8
http/server