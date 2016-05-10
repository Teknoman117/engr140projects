#include <project/animationsource.hpp>

/**
 * Empty Constructor, initializes everything to a default state
 */
AnimationSource::AnimationSource()
    : skeleton(new Node()), skeletonTable(Node::flattreemap()), initialSkeleton(new Node()), initialSkeletonTable(Node::const_flattreemap())
{
    
}

/**
 * Base copy constructor.  Copies the skeleton
 * @param source AnimationSource to duplicate
 */
AnimationSource::AnimationSource(const AnimationSource& source)
    : skeletonTable(Node::flattreemap()), initialSkeleton(source.initialSkeleton), initialSkeletonTable(source.initialSkeletonTable)
{
    // If there is a skeleton to duplicate
    if(source.skeleton)
    {
        // Duplicate the existing skeleton
        skeleton = new Node(*(source.skeleton));
    
        // Build the lookup table
        skeleton->GetFlatNodeTree(skeletonTable);
    }
}

/**
 * Base destructor, cleans up the local skeleton storage
 */
AnimationSource::~AnimationSource()
{
    // Cleanup the skeleton
    delete skeleton;
}

/**
 * Base assignment operator.  Animation source assumes the state of the other
 * @param controller Source to set this source to
 */
AnimationSource& AnimationSource::operator= (const AnimationSource& source)
{
    // Store the initial skeleton and store the lookup map
    initialSkeleton = source.initialSkeleton;
    initialSkeletonTable = source.initialSkeletonTable;
    skeletonTable = Node::flattreemap();
    
    // Purge previous state
    delete skeleton;
    
    // If we have a new skeleton, duplicate it
    if(source.skeleton)
    {
        // Create the local skeleton (a copy)
        skeleton = new Node(*(source.skeleton));
    
        // Generate the quick lookup tables of the skeletal components
        skeleton->GetFlatNodeTree(skeletonTable);
    }
    
    // Return a this pointer
    return *this;
}

/**
 * Binds the animation source to a skeleton
 * @param root the root node of the transform tree of the skeleton
 */
void AnimationSource::Bind(const Node* root)
{
    // Purge previous state
    initialSkeletonTable = Node::const_flattreemap();
    skeletonTable = Node::flattreemap();
    delete skeleton;
    
    // Store the initial skeleton
    initialSkeleton = root;
    
    // Create the local skeleton (a copy)
    skeleton = new Node(*root);
    
    // Generate the quick lookup tables of the skeletal components
    initialSkeleton->GetFlatNodeTree(initialSkeletonTable);
    skeleton->GetFlatNodeTree(skeletonTable);
}

/**
 * Performs an update of the animation source
 * @param delta the elapsed time since the previous frame
 * @param now the current simulated time
 */
void AnimationSource::Update(double delta, double now)
{
    // Copy the from source's skeleton into the local skeleton
    for(Node::flattreemap::iterator it = skeletonTable.begin(); it != skeletonTable.end(); it++)
    {
        it->second->LocalTransform() = InitialBones().find(it->first)->second->LocalTransform();
    }
}

/**
 * Sets the local skeleton's transforms back to the initial pose
 */
void AnimationSource::Reset()
{
    // Copy the initial pose bones back into the local skeleton
    for(Node::const_flattreemap::const_iterator it = InitialBones().begin(); it != InitialBones().end(); it++)
    {
        skeletonTable[it->first]->LocalTransform() = it->second->LocalTransform();
    }
}

/**
 * Get a pointer to the local skeleton
 * @return pointer to the local skeleton
 */
Node* AnimationSource::Skeleton()
{
    return skeleton;
}

/**
 * Get a reference to the local skeleton bone lookup map
 * @return reference to the local skeleton bone lookup map
 */
Node::flattreemap& AnimationSource::Bones()
{
    return skeletonTable;
}

/**
 * Get a const pointer to the local skeleton
 * @return const pointer to the local skeleton
 */
const Node* AnimationSource::Skeleton() const
{
    return skeleton;
}

/**
 * Get a const reference to the local skeleton bone lookup map
 * @return const reference to the local skeleton bone lookup map
 */
const Node::flattreemap& AnimationSource::Bones() const
{
    return skeletonTable;
}

/**
 * Get a const pointer to the local skeleton
 * @return const pointer to the local skeleton
 */
const Node* AnimationSource::InitialSkeleton() const
{
    return initialSkeleton;
}

/**
 * Get a const reference to the initial skeleton bone lookup map
 * @return const reference to the initial skeleton bone lookup map
 */
const Node::const_flattreemap& AnimationSource::InitialBones() const
{
    return initialSkeletonTable;
}
