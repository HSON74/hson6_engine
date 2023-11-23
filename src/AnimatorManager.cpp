#include "AnimatorManager.h"
#include "ResourceManager.h"

std::shared_ptr<ResourceManager> a_resources = std::shared_ptr<ResourceManager>();

void AnimatorManager::CreateAniamtor(int64_t a, std::string fileofAnimation)
{
        std::string tmpP = a_resource->pathtoString(a_resources->getCurrentPath());

        for (const auto& entry :
           std::filesystem::directory_iterator(script_Path->stringToPath(tmpP))) {
            // Output the path of the file or subdirectory
            std::string t = script_Path->pathToString(entry.path());
            std::string new_t = t.substr(tmpP.size()+1, t.size() - tmpP.size());
            std::string new_name = new_t.substr(0, new_t.size()-4);
            LoadScript(new_name, new_t);
        }
}

void AnimatorManager::PlayAnimator(int64_t a, int typesofAnimation)
{
}

void AnimatorManager::StopAnimator(int64_t a, bool activecode)
{
}

void AnimatorManager::LoopAnimator(int64_t a, bool activecode)
{
}

void AnimatorManager::SetSpeed(int64_t a, float speed)
{
}

void AnimatorManager::NextFrame(int64_t a)
{
}

void AnimatorManager::shutdown()
{
    if (this->EntityWithAnimator.size() != 0) {
        for (int i = 0; i < this->EntityWithAnimator.size(); i++) {
            std::cout <<"" << std::endl;
        }
        EntityWithAnimator.clear();
    }
    
}
