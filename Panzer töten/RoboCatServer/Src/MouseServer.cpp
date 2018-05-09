#include <RoboCatServerPCH.h>


MouseServer::MouseServer()
{
}

void MouseServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject( this );
}


bool MouseServer::HandleCollisionWithCat( RoboCat* inCat )
{
	//kill yourself!

	//SetDoesWantToDie( true );

	Vector3 mouseMin(-5.f, -3.f, 0.f);
	Vector3 mouseMax(5.f, 3.f, 0.f);
	Vector3 mouseLocation = RoboMath::GetRandomVector(mouseMin, mouseMax);
	SetLocation(mouseLocation);
	ScoreBoardManager::sInstance->IncScore( inCat->GetPlayerId(), 1 );

	

	return false;
}


