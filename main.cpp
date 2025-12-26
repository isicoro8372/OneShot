#include <string>

//システム系
#include "main.h"
#include "renderer.h"
#include "keylogger.hpp"
#include "keyboard.h"
#include "debugPrintf.h"
#include "texture.h"
#include "Audio.h"

//ポリゴン系
#include "sprite.h"
#include "line.h"
#include "cube.hpp"
#include "light.h"

//シーン系
#include "scenemanager.h"
#include "title.h"
#include "background.h"
////#include "result.h"
#include "fade.hpp"

//ゲームプレイ
////#include "blockpiece.hpp"
////#include "blockplace.hpp"
////#include "effect.h"
#include "game.h"

//UI系
////#include "score.h"

//==============================================
// ライブラリのリンク
//==============================================
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")


//==============================================
// マクロ定義
//==============================================
#define CLASS_NAME "DX21 Window"
#define WINDOW_CAPTION "AT12B173-17-桜井優輝"

//vectorの境界チェックの無効化
#ifdef _ITERATOR_DEBUG_LEVEL
#undef _ITERATOR_DEBUG_LEVEL
#define _ITERATOR_DEBUG_LEVEL (0)
#endif 

//==============================================
// グローバル変数
//==============================================
#ifdef _DEBUG

int g_CountFPS;
char g_DebugStr[2048] = WINDOW_CAPTION;

#endif // _DEBUG

//==============================================
// プロトタイプ宣言
//==============================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit();
void Update();
void Draw();

//==============================================
// Main関数
//==============================================
int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR lParam, _In_ int nCmdShow)
{
	//乱数初期化
#ifdef _DEBUG
	srand(0);
#else
	srand((unsigned int)timeGetTime());
#endif // _DEBUG


	//フレームレート計測用変数
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	//COMコンポーネントの準備(機能を部品化して外部のプログラムから共有利用する)
	HRESULT dummy = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);

	//ウィンドウクラスの登録(ウィンドウの仕様を決め、Windowsにセットする)
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(WNDCLASS));

	wc.lpfnWndProc = WndProc; //コールバック関数のポインタ
	wc.lpszClassName = CLASS_NAME; //クラス名設定
	wc.hInstance = hInstance; //アプリケーションのインスタンス指定
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); //カーソルの種類
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1); //ウィンドウの背景色
	RegisterClass(&wc); //Windowsへセット

	//ウィンドウサイズの調整
	RECT rc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };	//left top right bottom

	//描画領域がHDになるように調節
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, FALSE);

	//ウィンドウの作成
	HWND hWnd = CreateWindow(CLASS_NAME, WINDOW_CAPTION, WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX), 0, 0, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);
	//ウィンドウ識別名 タイトル名 ウィンドウの形状 配置座標x 配置座標y ウィンドウの幅 ウィンドウの高さ 親指定 メニュー? アプリケーションインスタンス lpParam

	//初期化
	if (FAILED(Init(hInstance, hWnd, true)))
	{
		return -1;
	}

	//作成したウィンドウの表示
	ShowWindow(hWnd, nCmdShow);

	//ウィンドウの内容を強制表示
	UpdateWindow(hWnd);

	//メッセージループ
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	//フレームレート計測初期化
	timeBeginPeriod(1);
	dwExecLastTime = dwFPSLastTime = timeGetTime();
	dwCurrentTime = dwFrameCount = 0;


	//ゲームループ
	while (1) //GetMessage(&msg, NULL, 0, 0) <= メッセージが来るまで待機
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) //メッセージが来ていた場合
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg); // WndProcが呼び出される
			}
		}
		else //メッセージが来てないため、更新処理
		{
			dwCurrentTime = timeGetTime(); //現在のタイマー値を取得
			if ((dwCurrentTime - dwFPSLastTime) >= 1000) //一秒経過したか
			{
#ifdef _DEBUG
				g_CountFPS = dwFrameCount;
#endif // _DEBUG

				dwFPSLastTime = dwCurrentTime; //現在のタイマー値を保存
				dwFrameCount = 0; //フレームカウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= ((float)1000 / 60)) //1/60秒経過したか(60fps)
			{
				dwExecLastTime = dwCurrentTime; //現在の時間を保存
#ifdef _DEBUG
				wsprintf(g_DebugStr, WINDOW_CAPTION); //配列にキャプション文字列を格納
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " FPS:%d", g_CountFPS);
				SetWindowText(hWnd, g_DebugStr); //キャプションの書き換え
#endif // _DEBUG


				Update();
				Draw();

				keycopy();
				//キーの初期化(最後にやる)

				dwFrameCount++;
			}
		}
	}

	Uninit();

	return (int)msg.wParam;
}

//==============================================
// ウィンドウプロシージャ(ウィンドウ枠の処理)
//==============================================
//コールバック関数 メッセージを受け取った場合実行される
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) //uMsgにメッセージが届く
	{
	 case WM_ACTIVATEAPP:
     case WM_SYSKEYDOWN:
     case WM_KEYUP:
     case WM_SYSKEYUP:
         Keyboard_ProcessMessage(uMsg, wParam, lParam);
		 StoreKeyLog(uMsg, wParam);
         break;
	 case WM_KEYDOWN: //何かしらのキーが押された
		 if (wParam == VK_ESCAPE) //ESCだった場合
		 {
			 //ウィンドウを閉じたいメッセージを送信
			 SendMessage(hWnd, WM_CLOSE, 0, 0);
		 }

		 Keyboard_ProcessMessage(uMsg, wParam, lParam);
		 StoreKeyLog(uMsg, wParam);
		break;

	case WM_CLOSE: //ウィンドウを閉じる
		if (MessageBox(hWnd, "本当に終了してよろしいですか？", "確認", MB_OKCANCEL | MB_DEFBUTTON2 | MB_ICONEXCLAMATION) == IDOK)
		{
			DestroyWindow(hWnd); //終了リクエスト(WM_DESTROY)
		}
		else
		{
			return 0;
		}

		break;

	case WM_DESTROY: //終了命令
		PostQuitMessage(0);
		break;
	}

	//必要のないメッセージはデフォルトプロシージャで適当に返す
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//==============================================
// 初期化
//==============================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	Keyboard_Initialize();

	//DirectX関連
	InitRenderer(hInstance, hWnd, bWindow);
	SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	InitTexture();
	InitLight();
	LoadTexture(L"asset/texture/blank.png");//white

	InitSprite();
	InitLine();

	InitAudio();
	InitFade();
	InitBackground();

	InitManager();

	////InitScore();

	return S_OK;
}

//==============================================
// 更新
//==============================================
void Update()
{
	UpdateKeyLog();

	UpdateBackground();
	UpdateManager();
}

//==============================================
// 描画
//==============================================
void Draw()
{
	//バックバッファのクリア
	Clear(0.5f, 0.5f, 0.5f);
	//2D描画なので奥行き処理をオフ
	SetDepthEnable(true);

	//描画処理
	//DrawBackground();
	DrawManager();

	//バックバッファをフロントバッファへコピー
	Present();
}

//==============================================
// 終了
//==============================================
void Uninit()
{
	//DirectX関連
	UninitLight();
	UninitTexture();
	UninitRenderer();
	UninitSprite();
	UninitLine();

	UninitManager();

	UninitAudio();
	UninitFade();
	UninitBackground();
}