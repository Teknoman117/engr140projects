#ifndef __ANIMATION_CLIP_HPP__
#define __ANIMATION_CLIP_HPP__

#include <project/common.hpp>
#include <project/animation.hpp>
#include <project/animationsource.hpp>

/**
 * AnimationClip encapsulates the animation of a skeleton via
 * an Animation object loaded from a model
 */
class AnimationClip : public AnimationSource
{
    /** The keyframe animation that drives the animation clip */
    const Animation *animation;
    
    /** The time at which the animation began playing */
    float animationStartTime;
    
    /** The time multiplier of the animation */
    float speed;
    
    /** Flag indicating whether the animation is playing */
    bool playing;
    
    /** Flag indicating whether the animation should play one-shot */
    bool loop;
public:
    /**
     * Standard constructor.  Initializes everything and stores the animation
     */
    AnimationClip(const Animation *animation = NULL);
    
    /**
     * Copy constructor - duplicate another animation clip
     * @param animationClip the animation clip to duplicate
     */
    AnimationClip(const AnimationClip& animationClip);
    
    /**
     * Update the animation clip
     * @param delta time since last frame in seconds
     * @param now the current time
     */
    void Update(double delta, double now);
    
    /**
     * Setter method for the animation
     * @param animation the animation to play
     */
    void SetAnimation(const Animation *animation);
    
    /**
     * Setter method for the animation playback speed
     * @param playback speed of the animation (1.0 for normal)
     */
    void SetPlaybackSpeed(float speed);
    
    /**
     * Begin playing the animation
     * @param loop should the animation play on a loop versus one shot
     * @param now the current simulated game time
     */
    void Play(bool loop, double now);
    
    /**
     * Stop the animation
     */
    void Stop();
    
    /**
     * Accessor method for the playing flag
     * @return true if the animation is playing
     */
    bool IsPlaying();
    
    /**
     * Accessor method for the loop flag
     * @return true if the animation is playing on a loop
     */
    bool IsLooping();
    
    /**
     * Method to get current animation progress
     * @return animation progress (0.0 -> 1.0)
     */
    const float GetProgress(double now) const;
};

#endif
