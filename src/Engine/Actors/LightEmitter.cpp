#include "LightEmitter.h"


// ## This is automatic generated text. Pleace do not change it.
// ## Registration in ActorFactory
#include "../../Engine/Modules/ActorFactory.h"
// unnamed namespase to hide from another places
namespace
{
	// specific factory
	IActor* CreateLightEmitter(World *world, const Vector2D location, const Vector2D scale, const Rotator rotation)
	{
		return new LightEmitter(world, location, scale, rotation);
	}

	const std::string CLASS_ID = "LightEmitter";

	// register specific factory in actor factory
	const bool registered = ActorFactory::Factory().registerActor(CLASS_ID, CreateLightEmitter);
}
// ## End of automatic generated text


LightEmitter::LightEmitter(World *world, Vector2D location, Vector2D scale, Rotator rotation) : Actor(world, location, rotation)
{
	this->type = AT_Light;
	this->brightness = 1.f;
	this->color = 0xFFFFFFFF;

	this->classID = CLASS_ID;
}


LightEmitter::~LightEmitter(void)
{
}

void LightEmitter::update(float deltatime)
{
	Actor::update(deltatime);
}

void LightEmitter::updateCollision()
{
}

void LightEmitter::takeDamage(float damageValue,Vector2D impulse)
{
}