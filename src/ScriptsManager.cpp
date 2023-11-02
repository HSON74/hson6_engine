#include "ScriptsManager.h"
#include "ResourceManager.h"






std::shared_ptr<ResourceManager> script_Path = std::make_shared<ResourceManager>();
void ScriptsManager::StartUp()
{
	lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::table);
	lua.script("math.randomseed(0)");
    lua.new_enum("KEYBOARD", 
        "SPACE", GLFW_KEY_SPACE, 
        "Q", GLFW_KEY_Q,
        "W", GLFW_KEY_W,
        "E", GLFW_KEY_E,
        "R", GLFW_KEY_R,
        "T", GLFW_KEY_T,
        "Y", GLFW_KEY_Y,
        "U", GLFW_KEY_U,
        "I", GLFW_KEY_I,
        "O", GLFW_KEY_P,
        "A", GLFW_KEY_A,
        "S", GLFW_KEY_S,
        "D", GLFW_KEY_D,
        "F", GLFW_KEY_F,
        "G", GLFW_KEY_G,
        "H", GLFW_KEY_H,
        "J", GLFW_KEY_J,
        "K", GLFW_KEY_K,
        "L", GLFW_KEY_L,
        "Z", GLFW_KEY_Z,
        "X", GLFW_KEY_X,
        "C", GLFW_KEY_C,
        "V", GLFW_KEY_V,
        "B", GLFW_KEY_B,
        "N", GLFW_KEY_N,
        "M", GLFW_KEY_M
        );
  
    
    lua.new_usertype<Sprite>("Sprite",
        sol::constructors<Sprite()>(),
        "name", &Sprite::imageName,
        "path", &Sprite::imagePath,
        "active", &Sprite::active,
        "tag", &Sprite::tag,
        "position", &Sprite::position,
        "scale", &Sprite::scale,
        "layer", &Sprite::z_value,
        "isDrawn", &Sprite::in_active
    );
    lua.new_usertype<Script>("Script",
        sol::constructors<Script()>(),
        "name", &Script::name
    );

    lua.new_usertype<m_Types::vec3>("vec3",
        sol::constructors<m_Types::vec3(), m_Types::vec3(float), m_Types::vec3(float, float, float)>(),
        "x", &m_Types::vec3::x,
        "y", &m_Types::vec3::y,
        "z", &m_Types::vec3::z,
        sol::meta_function::addition, sol::overload([](const m_Types::vec3& v1, const m_Types::vec3& v2) -> m_Types::vec3 { return v1 + v2; }),
        sol::meta_function::subtraction, sol::overload([](const m_Types::vec3& v1, const m_Types::vec3& v2) -> m_Types::vec3 { return v1 - v2; }),
        sol::meta_function::multiplication, sol::overload(
            [](const m_Types::vec3& v1, const m_Types::vec3& v2) -> m_Types::vec3 { return v1 * v2; },
            [](const m_Types::vec3& v1, float f) -> m_Types::vec3 { return v1 * f; },
            [](float f, const m_Types::vec3& v1) -> m_Types::vec3 { return f * v1; }
        )
    );

};

void ScriptsManager::Shutdown()
{
    if (!ScriptMap.empty()) {
        ScriptMap.clear();
    }
    
};

bool ScriptsManager::LoadScript(const std::string& name, const std::string& path)
{
   // std::tmpName = name + 
	if (ScriptMap.count(name) != 0) {
		return false;
	}
    
	std::string tmpP = script_Path ->pathToString(script_Path->getCurrentPath()/ script_Path->stringToPath("assets")/ script_Path->stringToPath("Scripts") / script_Path->stringToPath(path + ".lua"));
	ScriptMap[name] = lua.load_file(tmpP);
    if (!ScriptMap[name].valid()) {
        std::cout << tmpP << " is no added" << std::endl;
    }
    else {
        std::cout << tmpP << " is added" << std::endl;
    }
   
	return true;
}
