#include <PanzerServerPCH.h>


PickupServer::PickupServer()
{
}

void PickupServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject( this );
}


bool PickupServer::HandleCollisionWithCat( Panzer* inCat )
{
	//kill yourself!

	//SetDoesWantToDie( true );
	if (type == PickupType::health) {
		if (inCat->GetHealth() < inCat->GetMaxHealth()) {
			Vector3 mouseMin(-5.f, -3.f, 0.f);
			Vector3 mouseMax(5.f, 3.f, 0.f);
			Vector3 mouseLocation = RoboMath::GetRandomVector(mouseMin, mouseMax);
			SetLocation(mouseLocation);
			NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), EMRS_Pose);
			inCat->HandleHealthIncrease(1);
			NetworkManagerServer::sInstance->SetStateDirty(inCat->GetNetworkId(), inCat->ECRS_Health);
		}
	}
	if (type == PickupType::mine) {
		Vector3 mouseMin(-5.f, -3.f, 0.f);
		Vector3 mouseMax(5.f, 3.f, 0.f);
		Vector3 mouseLocation = RoboMath::GetRandomVector(mouseMin, mouseMax);
		SetLocation(mouseLocation);
		NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), EMRS_Pose);

		static_cast<PanzerServer*>(inCat)->TakeDamage();
	}

	return false;
}


