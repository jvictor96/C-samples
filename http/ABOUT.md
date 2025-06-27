docker build . --tag product
docker network create CS
docker run --name mypg --network CS -e POSTGRES_DB=jose -e POSTGRES_USER=jose -e POSTGRES_HOST_AUTH_METHOD=trust postgres -d 2
docker run --network CS -it -p 12913:12913 product
curl -X POST localhost:12913/product --data '{"name":"coca", "price":"5" }' 
curl localhost:12913/product;