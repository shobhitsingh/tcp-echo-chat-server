## tcp-echo-chat-server
A multithreading epoll based simple TCP chatserver. It uses a simple binary protocol. When it receives a message, it echos to all connected clients.
#Description
The main idea is to use epoll with std threads and use c++11 features. I created 3 IO threads which listens on incoming connection on listening socket and each has their own epoll event loop. Another thread is created at the start called the message processor which waits on std queue. When IO threads receives connection request, they add the session details to the session manager contaning std map. When IO threads receives message, they parse the binary packet and adds the username to the connected socket key in std mao through session manager and adds the message to the MessageQueue. This triggers the waiting MessageProcessor which reads from the queue and fetches all the sessions from the std map and sends the message to all the connected clients.
#Protocol
It uses a simple binary protocol
10/3/Tom/Hello
[ {TotalLength=2},{UserNameLength=2},{< UserName >},{Variable Message} ]
#Usage Example
```bash
> ./EchoServer/bin/echoserver -p 3000
```
## Echo Chat Client
#Usage Example
```bash
>./EchoClient/bin/echoclient -p 3000 -u Tom -d localhost (Client 1)
>./EchoClient/bin/echoclient -p 3000 -u Shobhit -d localhost (Client 2)
>./EchoClient/bin/echoclient -p 3000 -u Sam -d localhost (Client 3)
