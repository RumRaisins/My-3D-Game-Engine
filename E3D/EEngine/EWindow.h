#pragma once
#include"ECommon.h"
#include<Windows.h>
namespace E3D {
	class EInputListener;
	class ECanvas;
	//游戏窗口管理类
	class EGameWindow {
	public:
		//全局类指针，用于在WinProc中回调
		static EGameWindow *GWindow;
		//Window事件处理
		void startLoop();

		EGameWindow(const EString &windowTitle, HINSTANCE hinstance);
		~EGameWindow();

		//显示窗口

		void showWindow(EBool show);
		void updateWindow();

		void quiteApplication();

		HWND getHwWnd()const { return mHwnd; }
		HINSTANCE getHInstance()const { return mHinstance; }

		//注册、移除I/O时间监听

		void addInputListener(EInputListener *linstener);
		void removeInputLinstener(EInputListener *linstener);
		//设置绘制监听器，同时只能有一个绘制Cavas
		void setCanvasListener(ECanvas *canvasListener);
	protected:
		//全局Win时间回调函数
		static LRESULT WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM IParam);

		//窗口绘制函数
		virtual void onPaint(HDC hdc);
		virtual void onUpdate();
		virtual void onKeyDown(EInt msg);
		virtual void onKeyRelease(EInt msg);
		virtual void onMousePress(bool rightPress);
		virtual void onMouseMove(EInt x, EInt y);
		virtual void onMouseWheel(EInt delta);
	protected:
		//注册窗口
		bool registerWindow();
		//创建窗口
		bool createWindow();
	protected:
		HWND mHwnd;
		EString mWindowTitle;
		HINSTANCE mHinstance;
		std::vector<EInputListener*> mInputListeners;
		ECanvas	*mCanvasLinster;
		EBool mQuite;
		typedef std::vector<EInputListener*>::iterator InputItr;

	};
}