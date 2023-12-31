#pragma once

#define SOL_ALL_SAFETIES_ON 1

#include "GLFW/glfw3.h"
#include "sokol_time.h"
#include "spdlog/spdlog.h"
#include "soloud.h"
#include "soloud_wav.h"
#include "webgpu/webgpu.h"
#include "glfw3webgpu.h"
#include "glm/glm.hpp"


#include <iostream>
#include <cstdint>
#include <chrono>
#include <thread>
#include <climits>
#include <string>
#include <memory>
#include <functional>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <sstream>
#include <math.h>
#include "sol/sol.hpp"
#include <imgui.h>
#include <backends/imgui_impl_wgpu.h>
#include <backends/imgui_impl_glfw.h>



namespace m_Types
{
    typedef double real;
    typedef std::string string;
    typedef glm::vec2 vec2;
    typedef glm::vec3 vec3;
    typedef glm::vec4 vec4;
    //...
    
}
struct Position : m_Types::vec3 {}; // or: struct Position : public vec2 {};
struct Health { m_Types::real percent; };
struct Script { std::string name; };
struct Sprite {
    std::string imageName;
    int64_t EntityN;
    std::string imagePath;
    m_Types::vec3 position; 
    m_Types::vec3 rotation;
    m_Types:: vec3 scale;
    m_Types::real z_value;
    m_Types::vec2 size;
    int UI_location;
    bool active;
    bool in_active;
    std::string tag;
};
struct UI {
    std::string text;
    std::string tex_text;
};
struct Rigidbody {
    bool active;
    bool gravity;
    long mass;
    float f_weight;
    float toggle;
    bool canToggle;
    m_Types::vec3 force;
    m_Types::real meters_per_second = -9.8;
    m_Types::vec3 velocity;
    m_Types::vec3 AngularVelocity;
    
};
struct Camera {
    bool active;
    double fovy;
    double aspect;
    float c_near;
    float c_far;
    float r_near;
    float r_far;
    float left;
    float right;
    float bottom; 
    float top; 
};
struct Collider {
    bool active;
    bool IsStatis = true;
    bool IsTrigger = false;
    m_Types::vec3 offset; //Center
};
struct BoxCollider : Collider {
    m_Types::vec3 size;
};
struct Animation {
    bool isLooping;
    std::string animation_name;
    float speed;
    std::vector<std::string> animation_path;
    void destoryit() {
        if (animation_path.size() != 0) {
            animation_path.clear();
        }
    };
};
struct EntityAnimator {
    bool isPlaying;
    int current_animation;
    int current_frame;
    float frame_count;
    std::string path;
    std::vector<Animation> e_Animator_Frame;
    void destoryit() {
        if (e_Animator_Frame.size() != 0) {
            e_Animator_Frame.clear();
        }
    };
};

const float DeltaTime = 1.0f / 60.0f;
enum INPUTVALUE {
#define GLFW_RELEASE  0

#define GLFW_PRESS   1

#define GLFW_REPEAT   2

#define 	GLFW_KEY_UNKNOWN   -1

#define 	GLFW_KEY_SPACE   32

#define 	GLFW_KEY_APOSTROPHE   39 /* ' */

#define 	GLFW_KEY_COMMA   44 /* , */

#define 	GLFW_KEY_MINUS   45 /* - */

#define 	GLFW_KEY_PERIOD   46 /* . */

#define 	GLFW_KEY_SLASH   47 /* / */

#define 	GLFW_KEY_0   48

#define 	GLFW_KEY_1   49

#define 	GLFW_KEY_2   50

#define 	GLFW_KEY_3   51

#define 	GLFW_KEY_4   52

#define 	GLFW_KEY_5   53

#define 	GLFW_KEY_6   54

#define 	GLFW_KEY_7   55

#define 	GLFW_KEY_8   56

#define 	GLFW_KEY_9   57

#define 	GLFW_KEY_SEMICOLON   59 /* ; */

#define 	GLFW_KEY_EQUAL   61 /* = */

#define 	GLFW_KEY_A   65

#define 	GLFW_KEY_B   66

#define 	GLFW_KEY_C   67

#define 	GLFW_KEY_D   68

#define 	GLFW_KEY_E   69

#define 	GLFW_KEY_F   70

#define 	GLFW_KEY_G   71

#define 	GLFW_KEY_H   72

#define 	GLFW_KEY_I   73

#define 	GLFW_KEY_J   74

#define 	GLFW_KEY_K   75

#define 	GLFW_KEY_L   76

#define 	GLFW_KEY_M   77

#define 	GLFW_KEY_N   78

#define 	GLFW_KEY_O   79

#define 	GLFW_KEY_P   80

#define 	GLFW_KEY_Q   81

#define 	GLFW_KEY_R   82

#define 	GLFW_KEY_S   83

#define 	GLFW_KEY_T   84

#define 	GLFW_KEY_U   85

#define 	GLFW_KEY_V   86

#define 	GLFW_KEY_W   87

#define 	GLFW_KEY_X   88

#define 	GLFW_KEY_Y   89

#define 	GLFW_KEY_Z   90

#define 	GLFW_KEY_LEFT_BRACKET   91 /* [ */

#define 	GLFW_KEY_BACKSLASH   92 /* \ */

#define 	GLFW_KEY_RIGHT_BRACKET   93 /* ] */

#define 	GLFW_KEY_GRAVE_ACCENT   96 /* ` */

#define 	GLFW_KEY_WORLD_1   161 /* non-US #1 */

#define 	GLFW_KEY_WORLD_2   162 /* non-US #2 */

#define 	GLFW_KEY_ESCAPE   256

#define 	GLFW_KEY_ENTER   257

#define 	GLFW_KEY_TAB   258

#define 	GLFW_KEY_BACKSPACE   259

#define 	GLFW_KEY_INSERT   260

#define 	GLFW_KEY_DELETE   261

#define 	GLFW_KEY_RIGHT   262

#define 	GLFW_KEY_LEFT   263

#define 	GLFW_KEY_DOWN   264

#define 	GLFW_KEY_UP   265

#define 	GLFW_KEY_PAGE_UP   266

#define 	GLFW_KEY_PAGE_DOWN   267

#define 	GLFW_KEY_HOME   268

#define 	GLFW_KEY_END   269

#define 	GLFW_KEY_CAPS_LOCK   280

#define 	GLFW_KEY_SCROLL_LOCK   281

#define 	GLFW_KEY_NUM_LOCK   282

#define 	GLFW_KEY_PRINT_SCREEN   283

#define 	GLFW_KEY_PAUSE   284

#define 	GLFW_KEY_F1   290

#define 	GLFW_KEY_F2   291

#define 	GLFW_KEY_F3   292

#define 	GLFW_KEY_F4   293

#define 	GLFW_KEY_F5   294

#define 	GLFW_KEY_F6   295

#define 	GLFW_KEY_F7   296

#define 	GLFW_KEY_F8   297

#define 	GLFW_KEY_F9   298

#define 	GLFW_KEY_F10   299

#define 	GLFW_KEY_F11   300

#define 	GLFW_KEY_F12   301

#define 	GLFW_KEY_F13   302

#define 	GLFW_KEY_F14   303

#define 	GLFW_KEY_F15   304

#define 	GLFW_KEY_F16   305

#define 	GLFW_KEY_F17   306

#define 	GLFW_KEY_F18   307

#define 	GLFW_KEY_F19   308

#define 	GLFW_KEY_F20   309

#define 	GLFW_KEY_F21   310

#define 	GLFW_KEY_F22   311

#define 	GLFW_KEY_F23   312

#define 	GLFW_KEY_F24   313

#define 	GLFW_KEY_F25   314

#define 	GLFW_KEY_KP_0   320

#define 	GLFW_KEY_KP_1   321

#define 	GLFW_KEY_KP_2   322

#define 	GLFW_KEY_KP_3   323

#define 	GLFW_KEY_KP_4   324

#define 	GLFW_KEY_KP_5   325

#define 	GLFW_KEY_KP_6   326

#define 	GLFW_KEY_KP_7   327

#define 	GLFW_KEY_KP_8   328

#define 	GLFW_KEY_KP_9   329

#define 	GLFW_KEY_KP_DECIMAL   330

#define 	GLFW_KEY_KP_DIVIDE   331

#define 	GLFW_KEY_KP_MULTIPLY   332

#define 	GLFW_KEY_KP_SUBTRACT   333

#define 	GLFW_KEY_KP_ADD   334

#define 	GLFW_KEY_KP_ENTER   335

#define 	GLFW_KEY_KP_EQUAL   336

#define 	GLFW_KEY_LEFT_SHIFT   340

#define 	GLFW_KEY_LEFT_CONTROL   341

#define 	GLFW_KEY_LEFT_ALT   342

#define 	GLFW_KEY_LEFT_SUPER   343

#define 	GLFW_KEY_RIGHT_SHIFT   344

#define 	GLFW_KEY_RIGHT_CONTROL   345

#define 	GLFW_KEY_RIGHT_ALT   346

#define 	GLFW_KEY_RIGHT_SUPER   347

#define 	GLFW_KEY_MENU   348

#define 	GLFW_KEY_LAST   GLFW_KEY_MENU
};
/*float distancePointtoPoint(const m_Types::vec2& a, const m_Types::vec2& b) {
    return std::hypot(a.x - b.x, a.y - b.y);
};
float distancePointtoCircle(const m_Types::vec2& a, const m_Types::vec2& center, float radius) {
    return distancePointtoPoint(a, center) - radius;
};
float dotproduct(const m_Types::vec2& a) {
    return (a.x * a.x) + (a.y * a.y);
};
float distancePointtoLine(const m_Types::vec2& a, const m_Types::vec2& b, m_Types::vec2& normal) {
    m_Types::vec2 c;
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    return dotproduct(c) * normal.x  + dotproduct(c) * normal.y;
};*/