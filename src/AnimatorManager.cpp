
#include "AnimatorManager.h"
#include "ResourceManager.h"

std::shared_ptr<ResourceManager> a_resources = std::shared_ptr<ResourceManager>();

void AnimatorManager::CreateAniamtor(int64_t a, EntityAnimator& animator,std::string fileofAnimation)
{
        std::string tmpP = a_resources->pathToString(a_resources->getCurrentPath() / a_resources->stringToPath("assets") / a_resources->stringToPath(fileofAnimation));

        for (const auto& entry :
           std::filesystem::directory_iterator(a_resources->stringToPath(tmpP))) {
            // Output the path of the file or subdirectory
            std::string t = a_resources->pathToString(entry.path());
            std::string new_t = t.substr(tmpP.size()+1, t.size() - tmpP.size());
            std::string new_name = new_t.substr(0, new_t.size()-4);
            //Look fileofAnimation in 
            //Vector Array of <>
        }
        bool EntityExist = false;
        for (int i = 0; i < EntityWithAnimator.size(); i++) {
            if (EntityWithAnimator.at(i) == a) {
                EntityExist = true;
            }
        }
        if (!EntityExist) {
            EntityWithAnimator.push_back(a);
        }
       
}

void AnimatorManager::PlayAnimator(EntityAnimator& animator, int typesofAnimation)
{
    animator.frame_count = animator.speed;
    animator.isPlaying = true;
    animator.current_animation = typesofAnimation;
    animator.current_frame = 0;
    animator.isLooping = false;
    std::string path_c = a_resources->pathToString(a_resources->getCurrentPath() / a_resources->stringToPath("assets") / a_resources->stringToPath(animator.e_Animator_Frame.at(animator.current_animation).at(animator.current_frame)));
    animator.path = path_c;

}

void AnimatorManager::StopAnimator(EntityAnimator& animator, bool activecode)
{
    animator.isPlaying = false;
}

void AnimatorManager::LoopAnimator(EntityAnimator& animator, bool activecode)
{
    animator.isLooping = activecode;
}

void AnimatorManager::SetSpeed(EntityAnimator& animator, float speed)
{
    animator.speed = speed;
}

void AnimatorManager::NextFrame(EntityAnimator &animator)
{
    if (animator.isPlaying) {
        if (animator.frame_count > 0) {
            animator.frame_count = animator.frame_count - DeltaTime;
            return;
        }
        animator.current_frame++;
        if (animator.isLooping) {
            if (animator.current_frame >= animator.e_Animator_Frame.at(animator.current_animation).size()) {
                animator.current_frame = 0;
            }
        }
        std::string path_c = a_resources->pathToString(a_resources->getCurrentPath() / a_resources->stringToPath("assets") / a_resources->stringToPath(animator.e_Animator_Frame.at(animator.current_animation).at(animator.current_frame)));
        animator.path = path_c;
        animator.frame_count = animator.speed;
    }
}

void AnimatorManager::shutdown()
{
    if (this->EntityWithAnimator.size() != 0) {
        for (int i = 0; i < this->EntityWithAnimator.size(); i++) {
        }
        EntityWithAnimator.clear();
    }
    
}
