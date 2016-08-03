// The MIT License (MIT)
//
// Copyright (c) 2013 Dan Ginsburg, Budirijanto Purnomo
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

//
// Book:      OpenGL(R) ES 3.0 Programming Guide, 2nd Edition
// Authors:   Dan Ginsburg, Budirijanto Purnomo, Dave Shreiner, Aaftab Munshi
// ISBN-10:   0-321-93388-5
// ISBN-13:   978-0-321-93388-1
// Publisher: Addison-Wesley Professional
// URLs:      http://www.opengles-book.com
//            http://my.safaribooksonline.com/book/animation-and-3d/9780133440133
//
//    This file contains the Win32 implementation of the windowing functions.


#include <stdlib.h>
#include <stdio.h>

#include "esUtil.h"

//Global use
ESContext _esContext;

extern "C" {
	extern int esMain(ESContext *esContext);
}

///
// Includes
//
//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include "esUtil.h"

#ifdef _WIN64
#define GWL_USERDATA GWLP_USERDATA
#endif

///
//  ESWindowProc()
//
//      Main window procedure
//
LRESULT WINAPI ESWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT  lRet = 1;

	switch (uMsg)
	{
	case WM_CREATE:
		break;

	case WM_PAINT:
	{
//		ESContext *esContext = (ESContext *)(LONG_PTR)GetWindowLongPtr(hWnd, GWL_USERDATA);
		if (_esContext.drawFunc)
		{
			_esContext.drawFunc(&_esContext);
			eglSwapBuffers(_esContext.eglDisplay, _esContext.eglSurface);
		}

		ValidateRect(_esContext.eglNativeWindow, NULL);
	}
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CHAR:
	{
		POINT      point;
//		ESContext *esContext = (ESContext *)(LONG_PTR)GetWindowLongPtr(hWnd, GWL_USERDATA);

		GetCursorPos(&point);

		if (_esContext.keyFunc)
			_esContext.keyFunc(&_esContext, (unsigned char)wParam,(int)point.x, (int)point.y);
	}
	break;

	default:
		lRet = DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return lRet;
}

///
//  WinCreate()
//
//      Create Win32 instance and window
//
GLboolean WinCreate(ESContext *esContext, const char *title)
{
	WNDCLASS wndclass = { 0 };
	DWORD    wStyle = 0;
	RECT     windowRect;
	HINSTANCE hInstance = GetModuleHandle(NULL);

	wndclass.style = CS_OWNDC;
	wndclass.lpfnWndProc = (WNDPROC)ESWindowProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszClassName = "opengles3.0";

	if (!RegisterClass(&wndclass))
	{
		return FALSE;
	}

	wStyle = WS_VISIBLE | WS_POPUP | WS_BORDER | WS_SYSMENU | WS_CAPTION;

	// Adjust the window rectangle so that the client area has
	// the correct number of pixels
	windowRect.left = 0;
	windowRect.top = 0;
	windowRect.right = esContext->width;
	windowRect.bottom = esContext->height;

	AdjustWindowRect(&windowRect, wStyle, FALSE);

	esContext->eglNativeWindow = CreateWindow(
		"opengles3.0",
		title,
		wStyle,
		0,
		0,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	// Set the ESContext* to the GWL_USERDATA so that it is available to the
	// ESWindowProc
	SetWindowLongPtr(esContext->eglNativeWindow, GWL_USERDATA, (LONG)(LONG_PTR)esContext);

	if (esContext->eglNativeWindow == NULL)
	{
		return GL_FALSE;
	}

	ShowWindow(esContext->eglNativeWindow, TRUE);

	return GL_TRUE;
}

///
//  WinLoop()
//
//      Start main windows loop
//
void WinLoop(ESContext *esContext)
{
	MSG msg = { 0 };
	int done = 0;
	DWORD lastTime = GetTickCount();

	while (!done)
	{
		int gotMsg = (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0);
		DWORD curTime = GetTickCount();
		float deltaTime = (float)(curTime - lastTime) / 1000.0f;
		lastTime = curTime;

		if (gotMsg)
		{
			if (msg.message == WM_QUIT)
			{
				done = 1;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			SendMessage(esContext->eglNativeWindow, WM_PAINT, 0, 0);
		}

		// Call update function if registered
		if (esContext->updateFunc != NULL)
		{
			esContext->updateFunc(esContext, deltaTime);
		}
	}
}

EGLint GetContextRenderableType(EGLDisplay eglDisplay)
{
#ifdef EGL_KHR_create_context
	const char *extensions = eglQueryString(eglDisplay, EGL_EXTENSIONS);

	// check whether EGL_KHR_create_context is in the extension string
	if (extensions != NULL && strstr(extensions, "EGL_KHR_create_context"))
	{
		// extension is supported
		return EGL_OPENGL_ES3_BIT_KHR;
	}
#endif
	// extension is not supported
	return EGL_OPENGL_ES2_BIT;
}

GLboolean ESUTIL_API esCreateWindow(ESContext *esContext, const char *title, GLint width, GLint height, GLuint flags)
{
#ifndef __APPLE__

	EGLConfig config;
	EGLint majorVersion;
	EGLint minorVersion;
	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };

	if (esContext == NULL)
		return GL_FALSE;

	esContext->width = width;
	esContext->height = height;

	if (!WinCreate(esContext, title))
	{
		return GL_FALSE;
	}

	esContext->eglDisplay = eglGetDisplay(esContext->eglNativeDisplay);
	if (esContext->eglDisplay == EGL_NO_DISPLAY)
	{
		return GL_FALSE;
	}

	// Initialize EGL
	if (!eglInitialize(esContext->eglDisplay, &majorVersion, &minorVersion))
	{
		return GL_FALSE;
	}

	{
		EGLint numConfigs = 0;
		EGLint attribList[] =
		{
			EGL_RED_SIZE,       5,
			EGL_GREEN_SIZE,     6,
			EGL_BLUE_SIZE,      5,
			EGL_ALPHA_SIZE,     (flags & ES_WINDOW_ALPHA) ? 8 : EGL_DONT_CARE,
			EGL_DEPTH_SIZE,     (flags & ES_WINDOW_DEPTH) ? 8 : EGL_DONT_CARE,
			EGL_STENCIL_SIZE,   (flags & ES_WINDOW_STENCIL) ? 8 : EGL_DONT_CARE,
			EGL_SAMPLE_BUFFERS, (flags & ES_WINDOW_MULTISAMPLE) ? 1 : 0,
			// if EGL_KHR_create_context extension is supported, then we will use
			// EGL_OPENGL_ES3_BIT_KHR instead of EGL_OPENGL_ES2_BIT in the attribute list
			EGL_RENDERABLE_TYPE, GetContextRenderableType(esContext->eglDisplay),
			EGL_NONE
		};

		// Choose config
		if (!eglChooseConfig(esContext->eglDisplay, attribList, &config, 1, &numConfigs))
		{
			return GL_FALSE;
		}

		if (numConfigs < 1)
		{
			return GL_FALSE;
		}
	}

	// Create a surface
	esContext->eglSurface = eglCreateWindowSurface(esContext->eglDisplay, config,
		esContext->eglNativeWindow, NULL);

	if (esContext->eglSurface == EGL_NO_SURFACE)
	{
		return GL_FALSE;
	}

	// Create a GL context
	esContext->eglContext = eglCreateContext(esContext->eglDisplay, config,
		EGL_NO_CONTEXT, contextAttribs);

	if (esContext->eglContext == EGL_NO_CONTEXT)
	{
		return GL_FALSE;
	}

	// Make the context current
	if (!eglMakeCurrent(esContext->eglDisplay, esContext->eglSurface,
		esContext->eglSurface, esContext->eglContext))
	{
		return GL_FALSE;
	}

#endif // #ifndef __APPLE__

	return GL_TRUE;
}


int main(int argc, char *argv[])
{
	memset(&_esContext, 0, sizeof(ESContext));

	_esContext.height = 600;
	_esContext.width = 800;

	if (esMain(&_esContext) != GL_TRUE)
	{
		return 1;
	}

	WinLoop(&_esContext);

	if (_esContext.shutdownFunc != NULL)
	{
		_esContext.shutdownFunc(&_esContext);
	}

	if (_esContext.userData != NULL)
	{
		free(_esContext.userData);
	}

	return 0;
}
