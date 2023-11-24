#pragma 

#include "Types.h"

class AnimatorManager {
	public:
		std::vector<int64_t> EntityWithAnimator;
		AnimatorManager() = default;
		void CreateAnimator(int64_t a, EntityAnimator& animator, std::string name, std::string fileofAnimation);
		void CreateAnimatorIdle(int64_t a, EntityAnimator& animator, std::string name, std::string fileofAnimation);
		void AddAnimation(EntityAnimator& animator, std::string name, std::string fileofAnimation);
		void AddFrame(EntityAnimator& animator, std::string name, std::string fileofAnimation);
		void PlayAnimator(EntityAnimator& animator, std::string name);
		void StopAnimator(EntityAnimator& animator, bool activecode);
		void LoopAnimator(EntityAnimator& animator, bool activecode, std::string name);
		void SetSpeed(EntityAnimator& animator, std::string name, float speed);
		void NextFrame(EntityAnimator& animator);
		void shutdown();

};