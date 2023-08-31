
#include <iostream>
#include <memory>
#include <functional>
#include "Engine.h"

int main(int argc, const char *argv[])
{
    std::cout << "Hello, World!\n";
    std::shared_ptr<engine::Engine> h = std::make_shared<engine::Engine>();
    h->e_StartUp(h);
    std::cout << "Engine is created\n";
    h->e_ReunGameLoop(h->e_UpdateCallBack());
    h->e_ShutDown();
    return 0;
};