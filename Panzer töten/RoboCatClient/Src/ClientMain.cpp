#include <RoboCatClientPCH.h>

#if _WIN32
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	Mix_Music* music;

	if (Client::StaticInit())
	{

		Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);

		music = Mix_LoadMUS("Audio/gamemusic.wav");

		Mix_PlayMusic(music, -1);

		return Client::sInstance->Run();
	}
	else
	{
		//Mix_FreeMusic(music);
		//Mix_CloseAudio();
		SDL_Quit();
		//error
		return 1;
	}

}
#else
const char** __argv;
int __argc;
int main(int argc, const char** argv)
{
	__argc = argc;
	__argv = argv;

	if (Client::StaticInit())
	{
		return Client::sInstance->Run();
	}
	else
	{
		SDL_Quit();
		//error
		return 1;
	}
}
#endif