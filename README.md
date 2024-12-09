### About
Super simple program that uses TCP connection. Basically server listens on certain port and client sends data on certain IP:PORT(1 sec period)
### Building
```shell
#building server
g++ server.cxx -o server
#building client
g++ client.cxx -o client
# don't forget to give permissions
chmod u+rx server | chmod u+rx client
```
