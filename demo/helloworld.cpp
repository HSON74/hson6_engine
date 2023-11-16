
#include "Engine.h"


int main(int argc, const char *argv[])
{
    std::cout << "Hello, World!\n";
    std::shared_ptr<engine::Engine> h = std::make_shared<engine::Engine>();
    h->e_StartUp(h);
    std::cout << "Engine is created\n";

    h->e_ReunGameLoop([&]() {
        // code that will run inside the game loop
        h->UserInput(h);
        h->graphics->resize();
        
    });

    //std::cout << std::systemfile::<<"\n";
    h->e_ShutDown();
    return 0;
};
