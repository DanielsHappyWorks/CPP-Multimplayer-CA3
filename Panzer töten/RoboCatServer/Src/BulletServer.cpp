#include <PanzerServerPCH.h>


BulletServer::BulletServer()
{
	//yarn lives a second...
	mTimeToDie = Timing::sInstance.GetFrameStartTime() + 1.5f;
}

void BulletServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject( this );
}


void BulletServer::Update()
{
	Bullet::Update();

	if( Timing::sInstance.GetFrameStartTime() > mTimeToDie )
	{
		SetDoesWantToDie( true );
	}

}

bool BulletServer::HandleCollisionWithCat( Panzer* inCat )
{
	if( inCat->GetPlayerId() != GetPlayerId() )
	{
		//kill yourself!
		SetDoesWantToDie( true );

		static_cast< PanzerServer* >( inCat )->TakeDamage(GetPlayerId());

	}

	return false;
}


