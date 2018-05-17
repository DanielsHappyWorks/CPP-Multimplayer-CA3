#include <PanzerClientPCH.h>

PickupClient::PickupClient()
{
	mSpriteComponent.reset( new SpriteComponent( this ) );
	mSpriteComponent->SetTexture( TextureManager::sInstance->GetTexture( "pickup" ) );
}

void PickupClient::setType(int t) { 
	type = static_cast<PickupType>(t);
	if (type == PickupType::health) {
		mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("pickup"));
	}
	else if (type == PickupType::mine) {
		mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("mine"));
	}
}

void PickupClient::Read(InputMemoryBitStream& inInputStream)
{
	bool stateBit;

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		int type;
		inInputStream.Read(type);
		setType(type);
	}

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		Vector3 location;
		inInputStream.Read(location.mX);
		inInputStream.Read(location.mY);

		SetLocation(location);


		float rotation;
		inInputStream.Read(rotation);
		SetRotation(rotation);
	}


	inInputStream.Read(stateBit);
	if (stateBit)
	{
		Vector3 color;
		inInputStream.Read(color);
		SetColor(color);
	}
}