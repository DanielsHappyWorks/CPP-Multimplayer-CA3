#include <RoboCatClientPCH.h>

std::unique_ptr< HUD >	HUD::sInstance;


HUD::HUD() :
mScoreBoardOrigin( 25.f, 10.f, 0.0f ),
mBandwidthOrigin( 25.f, 650.f, 0.0f ),
mRoundTripTimeOrigin( 25.f, 670.f, 0.0f ),
mScoreOffset( 0.f, 25.f, 0.0f ),
mHealthOffset( 1100, 10.f, 0.0f ),
mErrorOrigin(100.f, 670.f, 0.0f),
mHealth( 0 )
{
	TTF_Init();
	mFont = TTF_OpenFont( "../Assets/Carlito-Regular.TTF", 24 );
	if( mFont == nullptr )
	{
		SDL_LogError( SDL_LOG_CATEGORY_ERROR, "Failed to load font." );
	}
}


void HUD::StaticInit()
{
	sInstance.reset( new HUD() );
}

void HUD::Render()
{
	RenderBandWidth();
	RenderRoundTripTime();
	RenderScoreBoard();
	RenderHealth();
}

void HUD::RenderHealth()
{
	if( mHealth > 0 )
	{
		string healthString = "";
		if (mHealth != 5) {
			healthString = StringUtils::Sprintf("Health %d", mHealth);
		}
		else {
			healthString = StringUtils::Sprintf("Max Health %d", mHealth);
		}
		RenderText( healthString, mHealthOffset, Colors::Red );
	}
}

void HUD::RenderBandWidth()
{
	string bandwidth = StringUtils::Sprintf( "In %d  Bps, Out %d Bps",
												static_cast< int >( NetworkManagerClient::sInstance->GetBytesReceivedPerSecond().GetValue() ),
												static_cast< int >( NetworkManagerClient::sInstance->GetBytesSentPerSecond().GetValue() ) );
	RenderText( bandwidth, mBandwidthOrigin, Colors::White );
}

void HUD::RenderRoundTripTime()
{
	float rttMS = NetworkManagerClient::sInstance->GetAvgRoundTripTime().GetValue() * 1000.f;

	string roundTripTime = StringUtils::Sprintf( "RTT %d ms", ( int ) rttMS  );
	RenderText( roundTripTime, mRoundTripTimeOrigin, Colors::White );
}

void HUD::RenderError(string error) {
	string errorMSG = StringUtils::Sprintf("Error %s", error);
	RenderText(errorMSG, mErrorOrigin, Colors::White);
}

void HUD::RenderScoreBoard()
{
	RenderText("Name", mScoreBoardOrigin, Colors::Black);
	mScoreBoardOrigin.mX += 150;
	RenderText("Wins", mScoreBoardOrigin, Colors::Black);
	mScoreBoardOrigin.mX += 100;
	RenderText("Score", mScoreBoardOrigin, Colors::Black);
	mScoreBoardOrigin.mX -= 250;

	const vector< ScoreBoardManager::Entry >& entries = ScoreBoardManager::sInstance->GetEntries();
	Vector3 offset = mScoreBoardOrigin + mScoreOffset;
	
	for( const auto& entry: entries )
	{
		RenderText(entry.GetPlayerName(), offset, entry.GetColor() );
		offset.mX += 150;
		RenderText(std::to_string(entry.GetTotalScore()), offset, entry.GetColor());
		offset.mX += 100;
		RenderText(std::to_string(entry.GetSessionScore()), offset, entry.GetColor());
		offset.mX -= 250;
		offset.mX += mScoreOffset.mX;
		offset.mY += mScoreOffset.mY;
	}

}

void HUD::RenderText( const string& inStr, const Vector3& origin, const Vector3& inColor )
{
	// Convert the color
	SDL_Color color;
	color.r = static_cast<Uint8>( inColor.mX * 255 );
	color.g = static_cast<Uint8>( inColor.mY * 255 );
	color.b = static_cast<Uint8>( inColor.mZ * 255 );
	color.a = 255;

	// Draw to surface and create a texture
	SDL_Surface* surface = TTF_RenderText_Blended( mFont, inStr.c_str(), color );
	SDL_Texture* texture = SDL_CreateTextureFromSurface( GraphicsDriver::sInstance->GetRenderer(), surface );

	// Setup the rect for the texture
	SDL_Rect dstRect;
	dstRect.x = static_cast<int>( origin.mX );
	dstRect.y = static_cast<int>( origin.mY );
	SDL_QueryTexture( texture, nullptr, nullptr, &dstRect.w, &dstRect.h );

	// Draw the texture
	SDL_RenderCopy( GraphicsDriver::sInstance->GetRenderer(), texture, nullptr, &dstRect );

	// Destroy the surface/texture
	SDL_DestroyTexture( texture );
	SDL_FreeSurface( surface );
}
