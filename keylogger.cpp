#include "main.h"
#include "keylogger.hpp"
#include "debugprintf.h"
#include <vector>

#define LOG_FRAME (10) //Min 2

bool pressedKey[MAX_INPUT];
bool keyLog[LOG_FRAME][MAX_INPUT];

void StoreKeyLog(UINT uMsg, WPARAM keys)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
   		pressedKey[keys] = true;
	break;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		pressedKey[keys] = false;
		break;
	}
}

void UpdateKeyLog()
{
	for (int i = LOG_FRAME - 1; i > 0; i--)
	{
		memcpy_s(keyLog[i], MAX_INPUT, keyLog[i - 1], sizeof(bool) * MAX_INPUT);
	}

	memcpy_s(keyLog[0], MAX_INPUT, pressedKey, sizeof(bool) * MAX_INPUT);

}

//キーが押されたか
bool GetKeyTrigger(WPARAM keyType)
{
	//前フレームに押されているか
	if (keyLog[1][keyType] == true)
	{
		//押されていたらfalseを返す
		return false;
	}
	//現在フレームに押されているか
	else if (keyLog[0][keyType] == true)
	{
		//押されていなかったらfalseを返す
		return true;
	}

	////前フレームに押されているか
	//for (int i = 0; i < keyLog[1].size(); i++)
	//{
	//	//押されていたらfalseを返す
	//	if (keyType == keyLog[1][i])
	//	{
	//		return false;
	//	}
	//}

	////現在のフレームで押されているか
	//for (int i = 0; i < keyLog[1].size(); i++)
	//{
	//	//押されていたらtrueを返す
	//	if (keyType == keyLog[1][i])
	//	{
	//		return true;
	//	}
	//}

	//押されていなかったらfalseを返す
	return false;
}

//キーが離されたか
bool GetKeyUp(WPARAM keyType)
{
	//前フレームに押されているか
	if (keyLog[1][keyType] == true)
	{
		//現在のフレームで押されているか
		if (keyLog[0][keyType] == true)
		{
			//押されていたらfalseを返す
			return false;
		}
		else
		{
			//押されていなかったらtrueを返す
			return true;
		}
	}
	//押されていなかったらfalseを返す
	else
	{
		return false;
	}

	////前フレームに押されているか
	//for (int i = 0; i < keyLog[1].size(); i++)
	//{
	//	//押されていたら現在フレーム判定へ
	//	if (keyType == keyLog[1][i])
	//	{
	//		//現在のフレームで押されているか
	//		for (int i = 0; i < keyLog[0].size(); i++)
	//		{
	//			//押されていたらfalseを返す
	//			if (keyType == keyLog[0][i])
	//			{
	//				return false;
	//			}
	//		}

	//		//押されていなかったらtrueを返す
	//		return true;
	//	}
	//}

	////押されていなかったらfalseを返す
	//return false;
}

//キーが押し続けられているか(今押されているか)
bool GetKeyDown(WPARAM keyType, unsigned int frame)
{
	//遡れるフレームより値が大きいなら最大値に
	if (frame > LOG_FRAME - 1)
	{
		frame = LOG_FRAME - 1;
	}

	//任意フレーム前に押されているか
	for (int i = 0; i < frame + 1; i++)
	{
		if (keyLog[i][keyType] != true)
		{
			//押されていなかったらfalseを返す
			return false;
		}
	}

	//押されていたらtrueを返す
	return true;


	////前フレームに押されているか
	//for (int i = 0; i < keyLog[1].size(); i++)
	//{
	//	//押されていたら現在フレーム判定へ
	//	if (keyType == keyLog[1][i])
	//	{
	//		//現在のフレームで押されているか
	//		for (int i = 0; i < keyLog[0].size(); i++)
	//		{
	//			//押されていたらfalseを返す
	//			if (keyType == keyLog[0][i])
	//			{
	//				return false;
	//			}
	//		}

	//		//押されていなかったらtrueを返す
	//		return true;
	//	}
	//}

	////押されていなかったらfalseを返す
	//return false;
}