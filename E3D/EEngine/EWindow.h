#pragma once
#include"ECommon.h"
#include<Windows.h>
namespace E3D {
	class EInputListener;
	class ECanvas;
	//��Ϸ���ڹ�����
	class EGameWindow {
	public:
		//ȫ����ָ�룬������WinProc�лص�
		static EGameWindow *GWindow;
		//Window�¼�����
		void startLoop();

		EGameWindow(const EString &windowTitle, HINSTANCE hinstance);
		~EGameWindow();

		//��ʾ����

		void showWindow(EBool show);
		void updateWindow();

		void quiteApplication();

		HWND getHwWnd()const { return mHwnd; }
		HINSTANCE getHInstance()const { return mHinstance; }

		//ע�ᡢ�Ƴ�I/Oʱ�����

		void addInputListener(EInputListener *linstener);
		void removeInputLinstener(EInputListener *linstener);
		//���û��Ƽ�������ͬʱֻ����һ������Cavas
		void setCanvasListener(ECanvas *canvasListener);
	protected:
		//ȫ��Winʱ��ص�����
		static LRESULT WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM IParam);

		//���ڻ��ƺ���
		virtual void onPaint(HDC hdc);
		virtual void onUpdate();
		virtual void onKeyDown(EInt msg);
		virtual void onKeyRelease(EInt msg);
		virtual void onMousePress(bool rightPress);
		virtual void onMouseMove(EInt x, EInt y);
		virtual void onMouseWheel(EInt delta);
	protected:
		//ע�ᴰ��
		bool registerWindow();
		//��������
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