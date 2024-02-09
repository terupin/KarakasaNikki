﻿#include <chrono>
#include <thread>
#include "Application.h"
#include "fpscontrol.h"
#include "renderer.h"
#include "manager.h"

const auto ClassName = TEXT("唐傘日記");     // ウィンドウクラス名
const auto WindowName = TEXT("唐傘日記");    // ウィンドウ名

//コンストラクタ
Application::Application(uint32_t width, uint32_t height)
: m_hInst   (nullptr)
, m_hWnd    (nullptr)
, m_Width   (width)
, m_Height  (height)
{ /* DO_NOTHING */ }


//デストラクタ
Application::~Application()
{ /* DO_NOTHING */ }

//アプリの実行
void Application::Run()
{
    if (InitApp())
    { 
        MainLoop(); 
    }

    TermApp();
}

//初期化処理
bool Application::InitApp()
{
    // ウィンドウの初期化.
    if (!InitWnd())
    { 
        return false; 
    }

    // 正常終了.
    return true;
}

//終了処理
void Application::TermApp()
{
    // ウィンドウの終了処理.
    TermWnd();
}

//ウィンドウの初期化処理
bool Application::InitWnd()
{
    // インスタンスハンドルを取得.
    auto hInst = GetModuleHandle(nullptr);
    if (hInst == nullptr)
    { 
        return false; 
    }

    // ウィンドウの設定.
    WNDCLASSEX wc = {};
    wc.cbSize           = sizeof(WNDCLASSEX);
    wc.style            = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc      = WndProc;
    wc.hIcon            = LoadIcon(hInst, IDI_APPLICATION);
    wc.hCursor          = LoadCursor(hInst, IDC_ARROW);
    wc.hbrBackground    = GetSysColorBrush(COLOR_BACKGROUND);
    wc.lpszMenuName     = nullptr;
    wc.lpszClassName    = ClassName;
    wc.hIconSm          = LoadIcon(hInst, IDI_APPLICATION);

    // ウィンドウの登録.
    if (!RegisterClassEx(&wc))
    { return false; }

    // インスタンスハンドル設定.
    m_hInst = hInst;

    // ウィンドウのサイズを設定.
    RECT rc = {};
    rc.right  = static_cast<LONG>(m_Width);
    rc.bottom = static_cast<LONG>(m_Height);

    // ウィンドウサイズを調整.
    auto style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
    AdjustWindowRect(&rc, style, FALSE);

    // ウィンドウを生成.
    m_hWnd = CreateWindowEx(
        0,
        ClassName,
        WindowName,
        style,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rc.right  - rc.left,
        rc.bottom - rc.top,
        nullptr,
        nullptr,
        m_hInst,
        nullptr);

    if (m_hWnd == nullptr)
    { return false; }
 
    // ウィンドウを表示.
    ShowWindow(m_hWnd, SW_SHOWNORMAL);

    // ウィンドウを更新.
    UpdateWindow(m_hWnd);

    // ウィンドウにフォーカスを設定.
    SetFocus(m_hWnd);

    // 正常終了.
    return true;
}

//ウィンドウの終了処理
void Application::TermWnd()
{
    // ウィンドウの登録を解除.
    if (m_hInst != nullptr)
    { UnregisterClass(ClassName, m_hInst); }

    m_hInst = nullptr;
    m_hWnd  = nullptr;
}

//メインループ
void Application::MainLoop()
{
    MSG msg = {};

    // FPS調整クラス
    FPS fpsrate(60);

    // ゲームの初期処理
    Manager::Init(this);

    while(WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) == TRUE)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            uint64_t delta_time = 0;

            // デルタタイムを計算
            delta_time = fpsrate.CalcDelta();

            Manager::Update(delta_time);        // ゲーム更新
            Manager::Draw(delta_time);		    // ゲーム描画

            // 規定時間まで時間調整
            fpsrate.Wait();
        }
    }

    // ゲームの終了処理
    Manager::Uninit();

}

//ウィンドウプロシージャ
LRESULT CALLBACK Application::WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch(msg)
    {
        case WM_DESTROY:
            { PostQuitMessage(0); }
            break;

        default:
            { /* DO_NOTHING */ }
            break;
    }

    return DefWindowProc(hWnd, msg, wp, lp);
}
