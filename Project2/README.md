# CS118 Project 2

Name: Taykhoom Dalal & Nevin Liang \
Email: tdalal@ucla.edu & nliang868@ucla.edu \
UID: 305303123 & 705575353

## Basic Testing
### Case 1: No loss

Open 2 windows and follow the following instructions

Compile and start the server in one of the windows:

`$ make server` \
`$ ./server [server_port]`

Compile and start the client in the other window:

`$ make client` \
`$ ./client [server_hostname] [server_port] [file_name]`

### Case 2: No loss

Open 3 windows and follow the following instructions

Compile and start the server in one of the windows:

`$ make server` \
`$ ./server [server_port]`

Start the proxy server which will drop the packets in another window:

`$ python3 rdproxy.py [server_port] [server_hostname] [decimal_loss_rate]`

Compile and start the client in the other window:

`$ make client` \
`$ ./client [server_hostname] [server_port] [file_name]`


## High Level Design

At a high level, the way our code works is that on the client side, after the connection setup is completed, we send the first 10 packets (or the first X packets, where X < 10 if the file is small), and then we wait for ACKs from the server in a while loop. As we get each ACK, we send the next packet, advanced our last unacked packet depending on the ack number from the server. If we get an ACK that is further down the line than our last unacked packet, we move our last unacked packet index up to the correct packet, and then send out X packets, where X represents how many packets with skipped with the ACK. We also restart the timer for the oldest unacked packet sent out, and at the end of the while loop, we check whether we have timed out on the last unacked packet, and if so, we resend the packets in the current window, from the last unacked to the last packet sent out. On the server side, our logic is very simple, we simply loop, receiving ACKs from the client, and if the sequence number matches with the last ACK the server sent, then we send a new ACK for the next expected packet, otherwise we send a duplicate ACK to the client with the sequence number of the expected packet. After the packets are done being sent out from the client side, and it has recieved all the necessary ACKs, the client initiates the shutdown procedure and the server does the same using the code provided.

## Problems and Solutions

One of the main problems we ran into while finishing this project was figuring out the logic for how to keep track of the last unacked packet. At a high level, the concept of using an index into our buffer which tells us the location of the last unacked packet is very intuitive, but the majority of our issues come into play when we run into the situation where the server sends multiple ACKs back to the client, but one of the ACKs in the middle is lost. We initially didn't know that in this case, as long as the client receives a new ACK, even if the ACK number for this is greater than the sequence number + length of the last unacked packet, that we should still send the next packet. Initially, our understanding was that we should keep waiting for the ACK that is right after our last unacked packet, so we kept timing out and our code didn't work. After we found out that we should just advance our last unacked packet by the number of packets skipped, our code started working partially correctly. We solved this by simply looping through all of the packets in our buffer and finding the index of the packet whose sequence number + length was equal to our acknumber, and then setting the last unacked packet index to the spot after that. 

However, another misunderstanding that we had was that if we get an ACK that is 5 packets ahead of our last unacked packet, that we should NOT only advanced our last unacked packet, but also SEND OUT 5 more new packets. This seems logical now, but at the time we didn't realize this, which caused us many issues. We then just simply looped by the number of packets skipped and sent out the new packets. Another annoying general problem we had while coding this project was dealing with the circular buffer. We used two "pointers" to keep track of the start of the buffer (I.E. the last unacked packet) and another to keep track of the end of the buffer (I.E. the spot in the buffer where we will write the next new packet). Although this seemed simple enough, it was easy to run into weird cases such as when the end pointer is located physically behind the start pointer, and so we had to make sure we never check anything like buffer [start] < buffer [end]. Overall, we feel as though this project should have been a lot more straightforward then it ended up being, although that was more due to the fact that GBN was more nuanced than we realized from just learning about the logic. 

## Acknowledgement of Sources

Besides the TA and lecture slides, we didn't use any other sources to help us with our project.