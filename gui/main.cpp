#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <cstdio>
#include <SFML/Network/UdpSocket.hpp>
#include <thread>
#include<windows.h>




using namespace sf;

void receiver(void*);

char status='d';    // the status for fill and drain button
String volume ="0"; // the volume percentage for tank

int main()
{
    char ip[50] ;
    int port;
    int port2;
    // Initialization settings 
    std::cout << "Tank's IP(e.g. PC's IP) is : ";
    std::cin >> ip;
    std::cout << "Tank's port(1024 ~ 65535)(e.g. 55002) is : " ;
    std::cin >> port;
    std::cout << "The port number(1024 ~ 65535)(e.g. 55001) which you wanna bind with is : ";
    std::cin >> port2;
    
    if (port < 1024 && port>65535 && port2 < 1024 && port2 > 65535 ) {
        system("Ple try again");
        return 0;
    }

    //GUI Setup
    RenderWindow window(VideoMode(600, 600), "Drain!");

    Texture t1, t2, t3,t4,t5, t6;
    t1.loadFromFile("images/fillButton.png");
    t2.loadFromFile("images/backgr.png");
    t4.loadFromFile("images/drainButton.png");
    t5.loadFromFile("images/stopButton.png");
    t3.loadFromFile("images/fillButtonClicked.png");
    t6.loadFromFile("images/drainButtonClicked.png");
    
    Sprite full(t1), background(t2), drain(t4), stop(t5),fillClicked(t3), drainClicked(t6);
 

    Vector2f  newPos;

    int offset[2]  = { 150,300 };
    sf::Font font;
    sf::Text text("", font, 40);
    font.loadFromFile("sansation.ttf");
    text.setFillColor(Color::Red);
    text.setPosition(offset[0], offset[1]);

    newPos = Vector2f(0 , 0);
    full.setPosition(newPos);
    newPos = Vector2f(0, 80);
    drain.setPosition(newPos);
    drainClicked.setPosition(newPos);
    newPos = Vector2f(0, 160);
    stop.setPosition(newPos);


    //UDPSocket Setup
    sf::UdpSocket socket;
    socket.bind(port2);
    sf::Thread t(receiver, &socket);
    t.launch();
    socket.send("", 1,ip, port);

    while (window.isOpen())
    {

        // while User lick a button
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::MouseButtonPressed) {
                Vector2i pos = Mouse::getPosition(window);

                if (pos.x < 100 &&pos.x>0) {
                    if (pos.y > 0 && pos.y < 70) {                      
                        std::string message = "0";
                        socket.send(message.c_str(), message.size()+1 , ip, port);
                    
                    }
                    else if (pos.y > 80 && pos.y < 150) {
                        std::string message = "1";
                        socket.send(message.c_str(), message.size() + 1, ip, port);

                    }
                    else if (pos.y > 160 && pos.y < 230) {
                        std::string message = "2";
                        socket.send(message.c_str(), message.size() + 1, ip, port);
                    }

                }
            }
        }
        
        //GUI Settup
        window.draw(background);
        if (status == 'a') {
            window.draw(fillClicked);
            window.draw(drainClicked);
        }
        else if (status == 'b') {
            window.draw(full);
            window.draw(drainClicked);
        }
        else if (status == 'c') {
            window.draw(fillClicked);
            window.draw(drain);
        }
        else if (status == 'd') {
            window.draw(full);
            window.draw(drain);
        }
        

        window.draw(stop);
        text.setString("Volume Percentage:" + volume);
        window.draw(text);
        window.display();
    }

    return 0;
}

//receiv message from Tank
void receiver(void* socket1 )
{
    while (true) {
        sf::UdpSocket* socket = (sf::UdpSocket*) socket1;

        char buffer[1024];
        std::size_t received = 0;
        sf::IpAddress sender;
        unsigned short port;
        socket->receive(buffer, sizeof(buffer), received, sender, port);
        if (buffer[0] == 'a' || buffer[0] == 'b' || buffer[0] == 'c' || buffer[0] == 'd') {
            status = buffer[0];
        }
        else {
            volume = buffer;
        }
        

    }
}



