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
	NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), EMRS_Pose);
	inCat->HandleHealthIncrease(1);
	NetworkManagerServer::sInstance->SetStateDirty(inCat->GetNetworkId(), inCat->ECRS_Health);

	

	return false;
}


