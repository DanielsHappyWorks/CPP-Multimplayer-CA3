#include <RoboCatServerPCH.h>

RoboCatServer::RoboCatServer() :
	mCatControlType( ESCT_Human ),
	mTimeOfNextShot( 0.f ),
	mTimeBetweenShots( 0.5f )
{}

void RoboCatServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject( this );
}

void RoboCatServer::Update()
{
	RoboCat::Update();
	
	Vector3 oldLocation = GetLocation();
	Vector3 oldVelocity = GetVelocity();
	float oldRotation = GetRotation();

	//are you controlled by a player?
	//if so, is there a move we haven't processed yet?
	if( mCatControlType == ESCT_Human )
	{
		ClientProxyPtr client = NetworkManagerServer::sInstance->GetClientProxy( GetPlayerId() );
		if( client )
		{
			MoveList& moveList = client->GetUnprocessedMoveList();
			for( const Move& unprocessedMove : moveList )
			{
				const InputState& currentState = unprocessedMove.GetInputState();

				float deltaTime = unprocessedMove.GetDeltaTime();

				ProcessInput( deltaTime, currentState );
				SimulateMovement( deltaTime );

				//LOG( "Server Move Time: %3.4f deltaTime: %3.4f left rot at %3.4f", unprocessedMove.GetTimestamp(), deltaTime, GetRotation() );

			}

			moveList.Clear();
		}
	}
	else
	{
		//do some AI stuff
		SimulateMovement( Timing::sInstance.GetDeltaTime() );
	}

	HandleShooting();

	if( !RoboMath::Is2DVectorEqual( oldLocation, GetLocation() ) ||
		!RoboMath::Is2DVectorEqual( oldVelocity, GetVelocity() ) ||
		oldRotation != GetRotation() )
	{
		NetworkManagerServer::sInstance->SetStateDirty( GetNetworkId(), ECRS_Pose );
	}

}

void RoboCatServer::HandleShooting()
{
	float time = Timing::sInstance.GetFrameStartTime();
	if( mIsShooting && Timing::sInstance.GetFrameStartTime() > mTimeOfNextShot )
	{
		//not exact, but okay
		mTimeOfNextShot = time + mTimeBetweenShots;

		//fire!
		YarnPtr yarn = std::static_pointer_cast< Yarn >( GameObjectRegistry::sInstance->CreateGameObject( 'YARN' ) );
		yarn->InitFromShooter( this );
	}
}

void RoboCatServer::TakeDamage( int inDamagingPlayerId )
{
	mHealth--;
	if( mHealth <= 0.f )
	{
		//score one for damaging player...
		ScoreBoardManager::sInstance->IncSessionScore( inDamagingPlayerId, 1 );

		if (ScoreBoardManager::sInstance->GetEntry(inDamagingPlayerId)->GetSessionScore() >= 3) {
			ScoreBoardManager::sInstance->SetSessionWinner(inDamagingPlayerId);

			std::string output = "";
			//coutput to file
			std::ifstream inputFile("scores.txt");
			if (inputFile.good()) {
				std::string savedName, savedScore;
				while (std::getline(inputFile, savedName)) {
					output += savedName;
					std::getline(inputFile, savedScore);
					output += "\n";
					if (savedName == ScoreBoardManager::sInstance->GetEntry(inDamagingPlayerId)->GetPlayerName()) {
						output += std::to_string(ScoreBoardManager::sInstance->GetEntry(inDamagingPlayerId)->GetTotalScore());
						std::string msg = StringUtils::Sprintf("%s won the round", savedName.c_str());
						NetworkManagerServer::sInstance->SendBroadcastMessage(msg);
					}
					else {
						output += savedScore;
					}
					output += "\n";
				}
				inputFile.close();
			}

			std::ofstream outputFile("scores.txt");
			outputFile << output;
			outputFile.close();
		}

		//and you want to die
		SetDoesWantToDie( true );

		//tell the client proxy to make you a new cat
		ClientProxyPtr clientProxy = NetworkManagerServer::sInstance->GetClientProxy( GetPlayerId() );
		if( clientProxy )
		{
			clientProxy->HandleCatDied();
		}
	}

	//tell the world our health dropped
	NetworkManagerServer::sInstance->SetStateDirty( GetNetworkId(), ECRS_Health );
}

void RoboCatServer::TakeDamage()
{
	mHealth--;
	if (mHealth <= 0.f)
	{
		//and you want to die
		SetDoesWantToDie(true);

		//tell the client proxy to make you a new cat
		ClientProxyPtr clientProxy = NetworkManagerServer::sInstance->GetClientProxy(GetPlayerId());
		if (clientProxy)
		{
			clientProxy->HandleCatDied();
		}
	}

	//tell the world our health dropped
	NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), ECRS_Health);
}