#include <PanzerClientPCH.h>

bool Client::StaticInit( )
{
	// Create the Client pointer first because it initializes SDL
	Client* client = new Client();

	if( WindowManager::StaticInit() == false )
	{
		return false;
	}
	
	if( GraphicsDriver::StaticInit( WindowManager::sInstance->GetMainWindow() ) == false )
	{
		return false;
	}

	TextureManager::StaticInit();
	RenderManager::StaticInit();
	InputManager::StaticInit();

	HUD::StaticInit();

	sInstance.reset( client );

	return true;
}

Client::Client()
{
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'RCAT', PanzerClient::StaticCreate );
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'MOUS', PickupClient::StaticCreate );
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'YARN', BulletClient::StaticCreate );

	string destination;
	string name;
	string password;

	std::ifstream inputFile("ip.txt");
	if (inputFile >> destination && inputFile >> name && inputFile >> password) {
	}
	else {
		// If open/read failed, create new file
		std::ofstream outputFile("ip.txt");
		destination = "127.0.0.1:50001";
		name = "DefName";
		password = "DefPassword";
		outputFile << destination << std::endl;
		outputFile << name << std::endl;
		outputFile << password << std::endl;
	}

	SocketAddressPtr serverAddress = SocketAddressFactory::CreateIPv4FromString( destination );

	NetworkManagerClient::StaticInit( *serverAddress, name , password);

	//NetworkManagerClient::sInstance->SetDropPacketChance( 0.6f );
	//NetworkManagerClient::sInstance->SetSimulatedLatency( 0.25f );
	//NetworkManagerClient::sInstance->SetSimulatedLatency( 0.5f );
	//NetworkManagerClient::sInstance->SetSimulatedLatency( 0.1f );
}



void Client::DoFrame()
{
	InputManager::sInstance->Update();
	Engine::DoFrame();
	if (NetworkManagerClient::sInstance->GetState() == NetworkManagerClient::NetworkClientState::NCS_Menu) {
		RenderManager::sInstance->RenderMenu();
	}
	else {
		NetworkManagerClient::sInstance->ProcessIncomingPackets();

		RenderManager::sInstance->Render();

		NetworkManagerClient::sInstance->SendOutgoingPackets();
	}
}

void Client::HandleEvent( SDL_Event* inEvent )
{
	if (NetworkManagerClient::sInstance->GetState() == NetworkManagerClient::NetworkClientState::NCS_Menu) {
		switch (inEvent->type)
		{
		case SDL_KEYDOWN:
			NetworkManagerClient::sInstance->SetState(NetworkManagerClient::NetworkClientState::NCS_SayingHello);
			break;
		default:
			break;
		}
	}
	if (NetworkManagerClient::sInstance->GetState() == NetworkManagerClient::NetworkClientState::NCS_ErrorRevieved) {
		switch (inEvent->type)
		{
		case SDL_KEYDOWN:
			NetworkManagerClient::sInstance->SetState(NetworkManagerClient::NetworkClientState::NCS_Menu);
			NetworkManagerClient::sInstance->mError = "";
			break;
		default:
			break;
		}
	}
	else {
		switch (inEvent->type)
		{
		case SDL_KEYDOWN:
			InputManager::sInstance->HandleInput(EIA_Pressed, inEvent->key.keysym.sym);
			break;
		case SDL_KEYUP:
			InputManager::sInstance->HandleInput(EIA_Released, inEvent->key.keysym.sym);
			break;
		default:
			break;
		}
	}
}

