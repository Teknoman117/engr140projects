#ifndef __ANIMATION_TRANSITION_HPP__
#define __ANIMATION_TRANSITION_HPP__

#include <project/common.hpp>
#include <project/animationsource.hpp>

class AnimationTransition : public AnimationSource
{
    AnimationSource *sourceFrom;
    AnimationSource *sourceTo;
    
    double           transitionProgress;
    double           transitionDuration;
    
    std::function<void (AnimationTransition *, AnimationSource *, AnimationSource *)> transitionCallback;
    bool             triggered;
    
public:
    
    AnimationTransition();
    
    /**
     * Start the animation transition
     * @param from the animation source we are transitioning from
     * @param to the animation source we are transitioning to
     * @param duration the amount of time to transition should take
     * @param callback a closure to execute upon completion of the transition
     */
    void Start(AnimationSource *from, AnimationSource *to, double duration, std::function<void (AnimationTransition *, AnimationSource *, AnimationSource *)> callback );
    
    /**
     * Update the animation transition
     * @param delta time since last frame in seconds
     * @param now the current time
     */
    virtual void Update(double delta, double now) override;
};

#endif 
