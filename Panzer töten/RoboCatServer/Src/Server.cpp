
#include <RoboCatServerPCH.h>



//uncomment this when you begin working on the server

bool Server::StaticInit()
{
	sInstance.reset( new Server() );

	return true;
}

Server::Server()
{

	GameObjectRegistry::sInstance->RegisterCreationFunction( 'RCAT', RoboCatServer::StaticCreate );
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'MOUS', MouseServer::StaticCreate );
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'YARN', YarnServer::StaticCreate );

	InitNetworkManager();
	
	//NetworkManagerServer::sInstance->SetDropPacketChance( 0.8f );
	//NetworkManagerServer::sInstance->SetSimulatedLatency( 0.25f );
	//NetworkManagerServer::sInstance->SetSimulatedLatency( 0.5f );
	//NetworkManagerServer::sInstance->SetSimulatedLatency( 0.1f );

}


int Server::Run()
{
	SetupWorld();

	return Engine::Run();
}

bool Server::InitNetworkManager()
{
	string portString;

	std::ifstream inputFile("port.txt");
	if (inputFile >> portString) {
	}
	else {
		// If open/read failed, create new file
		std::ofstream outputFile("port.txt");
		portString = "50001";
		outputFile << portString << std::endl;
	}

	uint16_t port = stoi( portString );

	return NetworkManagerServer::StaticInit( port );
}


namespace
{
	
	void CreateRandomMice( int inMouseCount, Mouse::PickupType type)
	{
		Vector3 mouseMin( -5.f, -3.f, 0.f );
		Vector3 mouseMax( 5.f, 3.f, 0.f );
		GameObjectPtr go;

		//make a mouse somewhere- where will these come from?
		for( int i = 0; i < inMouseCount; ++i )
		{
			go = GameObjectRegistry::sInstance->CreateGameObject( 'MOUS' );
			go->setType(type);
			Vector3 mouseLocation = RoboMath::GetRandomVector( mouseMin, mouseMax );
			go->SetLocation( mouseLocation );
		}
	}


}


void Server::SetupWorld()
{
	//spawn some health
	CreateRandomMice(3, Mouse::PickupType::health);
	//spawn some mines
	CreateRandomMice(3, Mouse::PickupType::mine);
}

void Server::DoFrame()
{
	NetworkManagerServer::sInstance->ProcessIncomingPackets();

	NetworkManagerServer::sInstance->CheckForDisconnects();

	NetworkManagerServer::sInstance->RespawnCats();

	Engine::DoFrame();

	NetworkManagerServer::sInstance->SendOutgoingPackets();

}

void Server::HandleNewClient( ClientProxyPtr inClientProxy )
{
	
	int playerId = inClientProxy->GetPlayerId();
	
	//check to see if player has a score
	bool nameFound = false;
	int totalScore = 0;

	std::ifstream inputFile("scores.txt");
	if (inputFile.good()) {
		std::string savedName, savedScore;
		while (std::getline(inputFile, savedName)) {
			std::getline(inputFile, savedScore);
			if (savedName == inClientProxy->GetName()) {
				totalScore = stoi(savedScore);
				nameFound = true;
				inputFile.close();
				break;
			}
		}
		inputFile.close();
	}


	if (!nameFound) {
		//create entry
		std::ofstream outputFile("scores.txt", std::ios::app);
		outputFile << inClientProxy->GetName() << std::endl;
		outputFile << 0 << std::endl;
		outputFile.close();
	}

	ScoreBoardManager::sInstance->AddEntry( playerId, inClientProxy->GetName(), totalScore);
	SpawnCatForPlayer( playerId );
}

void Server::SpawnCatForPlayer( int inPlayerId )
{
	RoboCatPtr cat = std::static_pointer_cast< RoboCat >( GameObjectRegistry::sInstance->CreateGameObject( 'RCAT' ) );
	cat->SetColor( ScoreBoardManager::sInstance->GetEntry( inPlayerId )->GetColor() );
	cat->SetPlayerId( inPlayerId );
	//gotta pick a better spawn location than this...
	Vector3 mouseMin(-5.f, -3.f, 0.f);
	Vector3 mouseMax(5.f, 3.f, 0.f);
	Vector3 mouseLocation = RoboMath::GetRandomVector(mouseMin, mouseMax);
	cat->SetLocation(mouseLocation);

}

void Server::HandleLostClient( ClientProxyPtr inClientProxy )
{
	//kill client's cat
	//remove client from scoreboard
	int playerId = inClientProxy->GetPlayerId();

	ScoreBoardManager::sInstance->RemoveEntry( playerId );
	RoboCatPtr cat = GetCatForPlayer( playerId );
	if( cat )
	{
		cat->SetDoesWantToDie( true );
	}
}

RoboCatPtr Server::GetCatForPlayer( int inPlayerId )
{
	//run through the objects till we find the cat...
	//it would be nice if we kept a pointer to the cat on the clientproxy
	//but then we'd have to clean it up when the cat died, etc.
	//this will work for now until it's a perf issue
	const auto& gameObjects = World::sInstance->GetGameObjects();
	for( int i = 0, c = gameObjects.size(); i < c; ++i )
	{
		GameObjectPtr go = gameObjects[ i ];
		RoboCat* cat = go->GetAsCat();
		if( cat && cat->GetPlayerId() == inPlayerId )
		{
			return std::static_pointer_cast< RoboCat >( go );
		}
	}

	return nullptr;

}