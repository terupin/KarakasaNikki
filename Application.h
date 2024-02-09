#pragma once
#define NOMINMAX
#include <Windows.h>
#include <cstdint>

//アプリケーションのclass
class Application
{
public:
    Application(uint32_t width, uint32_t height);
    ~Application();
    void Run();

    // 幅を取得
    uint32_t GetWidth() { return m_Width;}

    // 高さを取得
    uint32_t GetHeight() { return m_Height;}

    // ウインドウハンドルを返す
    HWND GetWindow() { return m_hWnd;}

private:
    HINSTANCE   m_hInst;        // インスタンスハンドルです.
    HWND        m_hWnd;         // ウィンドウハンドルです.
    uint32_t    m_Width;        // ウィンドウの横幅です.
    uint32_t    m_Height;       // ウィンドウの縦幅です.

    bool InitApp();
    void TermApp();
    bool InitWnd();
    void TermWnd();
    void MainLoop();

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
};