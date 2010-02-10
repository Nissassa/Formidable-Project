//////////////////////////////////////////////////////////////////////////
// File Name:	DirectShow.h
// Author:		Kyle Lauing
// Date:		06/09/2007
// Purpose:		This is where the functionality for streaming audio and 
//				video is created using DirectShow API.
//////////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////////
// Headers and Libraries
//////////////////////////////////////////////////////////////////////////
#include <dshow.h>
#pragma comment(lib, "strmiids.lib")


class CDirectShow
{
	HWND			m_hWnd;
	RECT			m_screenRect;

	IGraphBuilder*	m_graphBuilder;
	IMediaControl*	m_mediaControl;
	IMediaEvent*	m_mediaEvent;
	IVideoWindow*	m_videoWindow;

	bool			m_bVideoPlaying;


public:
	CDirectShow(void);
	~CDirectShow(void);

	bool Init(HWND hWnd);
	int LoadFile(char* Filename);
	void Play(void);
	void Stop(void);
	void Pause(void);
	bool IsFileFinished(void);
	void Update(void);
	void Shutdown(void);

	// Inline
	inline bool IsVideoPlaying(void) { return m_bVideoPlaying;}
};
