//main.cpp
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp> // for MouseButtonEvent
#include <iostream>
#include <vector>
#include "grid.h"
#include <fstream>
#include <filesystem>
#include "crossplatform.h"

void outputMouseInput(sf::RenderWindow& screen);
void checkKey();
using namespace sf;


int main()
{
    MAKE_FIFO("backend/mouseOutput.txt");
    run_backend();
    while(std::filesystem::exists("backend/run"))
    if(std::filesystem::exists("backend/render"))
    {
    RenderWindow screen(VideoMode({800, 800}), "Chess Game");
    Grid guiGrid;
    guiGrid.update();
    guiGrid.draw(screen);
    while (std::filesystem::exists("backend/render"))
         {
            auto event=screen.pollEvent();
            if(event)
              {
               if (event->is<Event::Closed>())break;  
               if (event->is<Event::MouseButtonPressed>())outputMouseInput(screen);
               if (event->is<Event::KeyPressed>())checkKey();
              }                       
         }

    screen.close();   
    
    }

    UNLINK_FIFO("mouseOutput.txt");
    return 0;
}


void checkKey()
{
if(Keyboard::isKeyPressed(Keyboard::Key::X)&&Keyboard::isKeyPressed(Keyboard::Key::Escape))
   {
    sleep(seconds(0.5));
    if(Keyboard::isKeyPressed(Keyboard::Key::X)&&Keyboard::isKeyPressed(Keyboard::Key::Escape))
    std::filesystem::remove("backend/run");
   }
}

void outputMouseInput(RenderWindow& screen)
{
 auto coordinates= Mouse::getPosition(screen);
 char x='a'+coordinates.x/100;
 char y='8'-coordinates.y/100;
 std::string a={x,y};
 std::ofstream out("backend/mouseOutput.txt",std::ios::app);
 out<<a<<std::endl;
 out.close();
}
