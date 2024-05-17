// C++ program
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <functional>
#include "Game.hpp"

#define PORT 9999

Value getV(char a){
    if(a == 'A') return Value::Ace;
    else if(a == 'K') return Value::King;
    else if(a == 'Q') return Value::Queen;
    else if(a == 'J') return Value::Jack;
    else if(a == 'D') return Value::Ten;
    else if(a == '9') return Value::Nine;
    else if(a == '8') return Value::Eight;
    else if(a == '7') return Value::Seven;
    else if(a == '6') return Value::Six;
    else if(a == '5') return Value::Five;
    else if(a == '4') return Value::Four;
    else if(a == '3') return Value::Three;
    return Value::Two;
}

Suit getSt(char a){
    if(a == 'c') return Suit::Clubs;
    else if(a == 's') return Suit::Spades;
    else if(a == 'h') return Suit::Hearts;
    return Suit::Diamonds;
}

bool handleRequest(int new_socket, char buffer[1024]){
    long NUMSIMS = (2<<15);
    
    size_t msgLen = (size_t)read(new_socket, buffer, 1024);
    std::string msg = buffer;
    
    std::string card1(msg.substr(0,2)), card2(msg.substr(2,2));

    playerHand user;
    user.first = Card( getV(card1[0]) , getSt(card1[1]) );
    user.second = Card( getV(card2[0]) , getSt(card2[1]) );

    std::vector< Game > vec;
    while(NUMSIMS){
        vec.emplace_back(user, NUMSIMS/(int)std::thread::hardware_concurrency());
        NUMSIMS/=std::thread::hardware_concurrency();
    }

    Threadedsimulate(vec);
    auto d = printResults(vec);
    
    // Process message and generate response
    std::string response = std::to_string(d);
    send(new_socket, response.c_str(), response.length(), 0);

    return msg.substr(0, msgLen)=="kill"?false:true;
}

int main() {
    
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }


    while ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) 
        && handleRequest(new_socket, buffer)
        ) {}

    std::cout << "finished\n";
    
    return 0;
}
