#pragma
#include <string>
#include <iostream>

class GraphicsManager
{
    public:

        void g_StartUp(int p_width, int p_height, std::string window_name, bool window_fullscreen);
        void g_Shutdown();
        void Draw();

        GraphicsManager() = default;
};