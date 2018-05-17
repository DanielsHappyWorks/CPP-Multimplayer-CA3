//I take care of rendering things!

#include <SDL_ttf.h>

class HUD
{
public:

	static void StaticInit();
	static std::unique_ptr< HUD >	sInstance;

	void Render();
	void	RenderError(string error);
	void	RenderBroadcast(string broadcast);
	void	RenderMenu();
	void			SetPlayerHealth( int inHealth )	{ mHealth = inHealth; }

private:

	HUD();

	void	RenderBandWidth();
	void	RenderRoundTripTime();
	void	RenderScoreBoard();
	void	RenderHealth();
	void	RenderText(const string& inStr, const Vector3& origin, const Vector3& inColor, int font = 0);

	Vector3										mBandwidthOrigin;
	Vector3										mRoundTripTimeOrigin;
	Vector3										mScoreBoardOrigin;
	Vector3										mScoreOffset;
	Vector3										mHealthOffset;
	Vector3										mErrorOrigin, mBroadcastOrigin;
	SDL_Rect									mViewTransform;

	TTF_Font*									mFontAlpha;
	TTF_Font*									mFontBravo;
	TTF_Font*									mFontCharlie;
	int											mHealth;
};

