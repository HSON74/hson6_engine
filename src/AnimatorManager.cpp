
#include "AnimatorManager.h"
#include "ResourceManager.h"

std::shared_ptr<ResourceManager> a_resources = std::shared_ptr<ResourceManager>();

void AnimatorManager::CreateAniamtor(int64_t a, EntityAnimator& animator, std::string name,std::string fileofAnimation)
{
    //Progress need Implementation
        std::string tmpP = a_resources->pathToString(a_resources->getCurrentPath() / a_resources->stringToPath("assets") / a_resources->stringToPath(fileofAnimation));
        //Vector Array of <>
        std::vector<std::string> tmp_vector_animator;
        for (const auto& entry :
            std::filesystem::directory_iterator(a_resources->stringToPath(tmpP))) {
            // Output the path of the file or subdirectory
            std::string t = a_resources->pathToString(entry.path());
            std::string new_t = t.substr(tmpP.size() + 1, t.size() - tmpP.size());
            std::string new_name = new_t.substr(0, new_t.size() - 4);
            std::cout << "t name: " << t << std::endl;
            std::cout << "new_t name: " << new_t << std::endl;
            std::cout << "new_name name: " << new_name << std::endl;
            //Look fileofAnimation in 
            tmp_vector_animator.push_back(t);
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
        animator.frame_count = 0;
        animator.isPlaying = false;
        animator.current_animation = 0;
        animator.current_frame = 0;
        animator.isLooping = false;
        std::string path_c = a_resources->pathToString(a_resources->getCurrentPath() / a_resources->stringToPath("assets") / a_resources->stringToPath(animator.e_Animator_Frame.at(animator.current_animation).at(animator.current_frame)));
        animator.path = path_c;
}

void AnimatorManager::PlayAnimator(EntityAnimator& animator, std::string name)
{
    bool nameexist = false;
    int index_animation = -1;
    for (int i = 0; i < animator.e_Animator_Frame.size(); i++) {
        if (name == animator.e_Animator_Frame.at(i).animation_name) {
            nameexist = true;
            index_animation = i;
            break;
        }
    }
    if (!nameexist) { return; }
    animator.frame_count = animator.e_Animator_Frame.at(index_animation).speed;
    animator.isPlaying = true;
    animator.current_animation = index_animation;
    animator.current_frame = 0;
    animator.e_Animator_Frame.at(index_animation).isLooping = false;
    std::string path_c = a_resources->pathToString(a_resources->getCurrentPath() / a_resources->stringToPath("assets") / a_resources->stringToPath(animator.e_Animator_Frame.at(animator.current_animation).at(animator.current_frame)));
    animator.path = path_c;

}

void AnimatorManager::StopAnimator(EntityAnimator& animator, bool activecode)
{
    animator.isPlaying = false;
}

void AnimatorManager::LoopAnimator(EntityAnimator& animator, bool activecode, std::string name)
{
    bool nameexist = false;
    int index_animation = -1;
    for (int i = 0; i < animator.e_Animator_Frame.size(); i++) {
        if (name == animator.e_Animator_Frame.at(i).animation_name) {
            nameexist = true;
            index_animation = i;
            break;
        }
    }
    if (!nameexist) { return; }
    animator.e_Animator_Frame.at(index_animation).isLooping = activecode;
}

void AnimatorManager::SetSpeed(EntityAnimator& animator, std::string name, float speed)
{
    bool nameexist = false;
    int index_animation = -1;
    for (int i = 0; i < animator.e_Animator_Frame.size(); i++) {
        if (name == animator.e_Animator_Frame.at(i).animation_name) {
            nameexist = true;
            index_animation = i;
            break;
        }
    }
    if (!nameexist) { return; }
    animator.e_Animator_Frame.at(index_animation).speed = speed;
}

void AnimatorManager::NextFrame(EntityAnimator &animator)
{
    if (animator.isPlaying) {
        if (animator.frame_count > 0) {
            animator.frame_count = animator.frame_count - DeltaTime;
            return;
        }
        animator.current_frame++;
        if (animator.e_Animator_Frame.at(animator.current_animation).isLooping) {
            if (animator.current_frame >= animator.e_Animator_Frame.at(animator.current_animation).animation_path.size()) {
                animator.current_frame = 0;
            }
        }
        else {
            if (animator.current_frame >= animator.e_Animator_Frame.at(animator.current_animation).animation_path.size()) {
                animator.current_frame--;
            }
        }
        std::string path_c = a_resources->pathToString(a_resources->getCurrentPath() / a_resources->stringToPath("assets") / a_resources->stringToPath(animator.e_Animator_Frame.at(animator.current_animation).animation_path.at(animator.current_animation)));
        animator.path = path_c;
        animator.frame_count = animator.e_Animator_Frame.at(animator.current_animation).speed;
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
