#pragma 

#include "Types.h"

class AnimatorManager {
	public:
		std::vector<int64_t> EntityWithAnimator;
		AnimatorManager() = default;
		void CreateAniamtor(int64_t a, EntityAnimator& animator, std::string fileofAnimation);
		void PlayAnimator(EntityAnimator& animator, int typesofAnimation);
		void StopAnimator(EntityAnimator& animator, bool activecode);
		void LoopAnimator(EntityAnimator& animator, bool activecode);
		void SetSpeed(EntityAnimator& animator, float speed);
		void NextFrame(EntityAnimator& animator);
		void shutdown();

};