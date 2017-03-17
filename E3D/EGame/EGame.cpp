#include"EGame.h"
#include"ECamera.h"
#include"ELight.h"
#include"EMaterial.h"
#include"ESceneManager.h"
#include"EPlayerController.h"
#include"EGameManager.h"

namespace E3D {

	const static EString map001 = "map001.map";
	const static EString map002 = "mao002.map";

	EGame::EGame() :plane(NULL), showHelp(false), scene(NULL), player(NULL), gameMgr(NULL), mLogicX(-1), mLogicZ(-1) {

	}
}