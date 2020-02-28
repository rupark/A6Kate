#include "serialize.h"
//#include "list.h"
#include "string.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "string.h"
#include <arpa/inet.h>
#include <assert.h>

#pragma once

class Node {
public:
    //FIRST NODE IS THE SERVER
    size_t nodes;
    size_t *ports;  // owned
    String **addresses;  // owned; strings owned

    //Personal Stuff
    String *server_addr;
    String *ip_addr;
    size_t port;
    size_t server_port;

    int sock_listen;
    int sock_send;

    Node(String *server_addr, size_t server_port, String *ip_addr, size_t port) {
        close(sock_send);

        this->server_addr = server_addr;
        this->server_port = server_port;

        this->ip_addr = ip_addr;
        this->port = port;

        this->addresses = new String *[1000];
        addresses[0] = this->server_addr;
        this->ports = new size_t[1000];
        ports[0] = this->server_port;
        this->nodes = 1;

        // needs to be listening for connections
        //this->sock_listen = init_server(this->ip_addr->cstr_, this->port);
        printf("NODE: creating sock_send");
        this->sock_send = init_client();
        printf("NODE: done creating sock_send...");
        printf("NODE: sending reg");
        this->send_reg();
        printf("NODE: DONE!");
        close(sock_send);

        this->sock_send = init_client();
        printf("sending status");
        this->send_status();
        printf("done sending");
        cout << nodes << endl;
        //close(sock_send);
         while (1) {
             handle_packet();
         }
    }

    ~Node() {
        delete server_addr;
        delete ip_addr;
        delete[] ports;
        delete[] addresses;
        close(sock_listen);
        close(sock_send);
    }

    void send_reg() {
        send_data(new Register(-1, 0, this->port, this->ip_addr));
    }

    void send_status() {
        send_data(new Status(-1, 0, new String("hi")));
    }

    sockaddr_in create_sockaddr(String *ip_address, size_t port) {
        struct sockaddr_in our_sockaddr;

        our_sockaddr.sin_family = AF_INET;
        our_sockaddr.sin_port = htons(port);

        // Convert IPv4 and IPv6 addresses from text to binary form
        if (inet_pton(AF_INET, ip_address->cstr_, &our_sockaddr.sin_addr) <= 0) {
            printf("\nInvalid address/ Address not supported");
            printf("%s", ip_address->cstr_);
            printf("\n");
        }

        return our_sockaddr;
    }

    void send_data(Message *m) {

        // create socket
        if ((sock_send = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("\n Socket creation error \n");
        }

        // connect socket
        struct sockaddr_in our_sockaddr;

        // if message does not have a sockaddr, build one.
        //if (m.kind_ == MsgKind::Register) {
        our_sockaddr = create_sockaddr(this->server_addr, this->server_port);
        //} else {
        //    String *ip = addresses[m.target_];
        //     size_t port = ports[m.target_];
        //     our_sockaddr = create_sockaddr(ip, port);
        // }

        // bind?

        if (connect(sock_send, (struct sockaddr *) &our_sockaddr, sizeof(our_sockaddr)) < 0) {
            printf("\nConnection Failed \n");

        }

        String *serial;

        // send data
        if (m->kind_ == MsgKind::Register) {
            serial = dynamic_cast<Register *>(m)->serialize();
        } else if (m->kind_ == MsgKind::Status) {
            serial = dynamic_cast<Status *>(m)->serialize();
            cout << serial->cstr_ << endl;
        } else {
            serial = dynamic_cast<Ack *>(m)->serialize();
        }

        //String* serial = new String("1?-1?0?f.0.0.1?8080");

        //printf("NODE: Message send = %s",serial->cstr_);
        send(sock_send, serial->cstr_, 10000, 0);
        printf("NODE: message sent");

        handle_packet();

//         cout << this->nodes << endl;
//         char buffer[1024] = {0};
//         int valread = read( sock_send , buffer, 1024);
//         printf("%s\n",buffer );

        printf("got message");

        // close socket
        //close(sock_send);

    }

    void handle_directory(Directory *d) {
        this->ports = d->ports;
        this->addresses = d->addresses;
        this->nodes = d->nodes;
        printf("Directory handled");
    }

    // Receives type Message -- has MsgKind field
    void handle_packet() {
        char *buffer = new char[10000];
        read(sock_send, buffer, 10000);
        //cout << buffer << endl;

        char *msg_kind = new char[10000];
        *msg_kind = buffer[0];

        switch (atoi(msg_kind)) {
            case 1:
                //TODO error?
                break;
            case 2: //ack
                cout << "Ack recieved in NODE";
                break;
            case 3:

                break;
            case 4: //Directory
                printf("Found directory");
                Directory *d = new Directory(buffer);
                handle_directory(d);
                break;
        }
    }

    String *get_address() {
        return this->ip_addr;
    }

    int init_client() {

        struct sockaddr_in server_addr;
        //char buffer[1024] = {0};
        printf("In client\n");
        printf("creating socket\n");
        int tmpSocket = 0;
        if ((tmpSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("\n Socket creation error \n");
            return -1;
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(this->port);

        // Convert IPv4 and IPv6 addresses from text to binary form
        if (inet_pton(AF_INET, this->ip_addr->cstr_, &server_addr.sin_addr) <= 0) {
            printf("\nInvalid address/ Address not supported");
            printf("%s", ip_addr);
            printf("\n");
            return -1;
        }

        if (connect(tmpSocket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
            printf("\nConnection Failed \n");
            return -1;
        }
        return tmpSocket;
    }
};