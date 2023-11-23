#pragma 

#include "Types.h"

class AnimatorManager {
	public:
		std::vector<int64_t> EntityWithAnimator;
		AnimatorManager() = default;
		void CreateAniamtor(int64_t a, std::string fileofAnimation);
		void PlayAnimator(int64_t a, int typesofAnimation);
		void StopAnimator(int64_t a, bool activecode);
		void LoopAnimator(int64_t a, bool activecode);
		void SetSpeed(int64_t a, float speed);
		void NextFrame(int64_t a);
		void shutdown();

};