#include <project/animationtransition.hpp>

AnimationTransition::AnimationTransition()
{
    
}

/**
 * Start the animation transition
 * @param from the animation source we are transitioning from
 * @param to the animation source we are transitioning to
 * @param duration the amount of time to transition should take
 * @param callback a closure to execute upon completion of the transition
 */
void AnimationTransition::Start(AnimationSource *from, AnimationSource *to, double duration, std::function<void (AnimationTransition *, AnimationSource *, AnimationSource *)> callback )
{
    sourceFrom = from;
    sourceTo = to;
    transitionDuration = duration;
    transitionCallback = callback;
    transitionProgress = 0.0;
    triggered = false;
    
    Update(0,0);
}

/**
 * Update the animation transition
 * @param delta time since last frame in seconds
 * @param now the current time
 */
void AnimationTransition::Update(double delta, double now)
{
    // Update both animations
    sourceFrom->Update(delta, now);
    sourceTo->Update(delta, now);
    transitionProgress += delta / transitionDuration;
    
    // If the animation is complete, just use the new animation
    if(transitionProgress >= 1.0)
    {
        // Copy the from source's skeleton into the local skeleton
        for(Node::flattreemap::iterator it = skeletonTable.begin(); it != skeletonTable.end(); it++)
        {
            it->second->LocalTransform() = sourceTo->Bones().find(it->first)->second->LocalTransform();
        }
        
        if(!triggered)
        {
            triggered = true;
            transitionCallback(this, sourceFrom, sourceTo);
        }
        
        return;
    }
    
    // Set the skeleton as a blend between the two
    for(Node::flattreemap::iterator it = skeletonTable.begin(); it != skeletonTable.end(); it++)
    {
        // Get the local transform of the cooresponding bone in the transition layer
        const Transform& a = sourceFrom->Bones().find(it->first)->second->LocalTransform();
        const Transform& b = sourceTo->Bones().find(it->first)->second->LocalTransform();
        
        // Store the interpolated transform
        it->second->LocalTransform() = Transform::Interpolate(a, b, (float) transitionProgress);
    }
}

