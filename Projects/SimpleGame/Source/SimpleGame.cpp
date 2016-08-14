#include "Jobs/Frame.h"
#include "SimpleGame.h"

namespace SimpleGame
{
	VANGUARD_DECLARE_MODULE(SimpleGame)

	void SimpleGame::SetupGame()
	{
	}

	void SimpleGame::UpdateGame(Vanguard::Frame* aFrame)
	{
		DEBUG_LOG("UPDATE GAME");
		DEBUG_LOG("Seconds Elapsed: " + String::FromFloat(aFrame->deltaTime.InSeconds()));
	}

	void SimpleGame::CleanupGame()
	{
	}
}