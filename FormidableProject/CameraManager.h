#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <DxErr.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dxerr.lib")

#ifdef _DEBUG
#pragma comment(lib, "d3dx9d.lib")
#else
#pragma comment(lib, "d3dx9.lib")
#endif



#include <vector>
using namespace std;


#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)													\
{																\
	HRESULT hr = (x);											\
	if(FAILED(hr))												\
{															\
	DXTrace __FILE__, (DWORD)__LINE__, hr, L#x, true);		\
}															\
}																
#endif
#else
#ifndef HR
#define HR(x) (x)
#endif
#endif

class CCameraManager
{
	// Projection Matrix Variables
	float		m_fov;
	float		m_aspectRatio;
	float		m_nearPlane;
	float		m_farPlane;

	// Projection Matrix
	D3DXMATRIX	m_projMat;
	//////////////////////////////////////////////////////////////////////////
	struct Camera
	{
		D3DXVECTOR3			eye;
		D3DXVECTOR3			lookAt;
		D3DXVECTOR3			up;
		D3DXMATRIX			viewMat;
	};

	// List of all cameras in a scene
	vector<Camera*>		m_cameraList;
	// Number of cameras in a scene
	int					m_cameraIndex;
	// Index to the active camera, uses last created camera unless specified
	int					m_activeCamera;

public:
	CCameraManager(void);
	~CCameraManager(void);
	
	void SetProjectionMatrix(float fovDegrees, float aspectRatio, float nearPlane, float farPlane);
	void RecalculateProjectionMatrix();
	void RecalculateViewMatrix();
	int CreateCamera(D3DXVECTOR3 eyePos, D3DXVECTOR3 lookAt, D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	void RemoveCamera(int cameraIndex);
	void SetActiveCamera(int cameraIndex) {m_activeCamera = cameraIndex;  RecalculateViewMatrix();}
	
	//////////////////////////////////////////////////////////////////////////
	// Accessors and Modifiers
	//////////////////////////////////////////////////////////////////////////
	D3DXMATRIX GetViewMat() {return m_cameraList[m_activeCamera]->viewMat;}
	D3DXMATRIX GetProjMat() {return m_projMat;}

	float GetFOV() {return m_fov;}
	void SetFOVDegrees(float fovDegrees) {m_fov = D3DXToDegree(fovDegrees);  RecalculateProjectionMatrix();}

	float GetAspectRatio() {return m_aspectRatio;}
	void SetAspectRatio(float aspectRatio) {m_aspectRatio = aspectRatio;  RecalculateProjectionMatrix();}
	void SetAspectRatio(float screenWidth, float screenHeight) {m_aspectRatio = screenWidth / screenHeight;  RecalculateProjectionMatrix();}

	float GetNearPlane() {return m_nearPlane;}
	void SetNearPlane(float nearPlane) {m_nearPlane = nearPlane; RecalculateProjectionMatrix();}

	float GetFarPlane() {return m_farPlane;}
	void SetFarPlane(float farPlane) {m_farPlane = farPlane; RecalculateProjectionMatrix();}

	//////////////////////////////////////////////////////////////////////////
	D3DXVECTOR3 GetEyePos() {return m_cameraList[m_activeCamera]->eye;}
	void SetEyePos(D3DXVECTOR3 eye) {m_cameraList[m_activeCamera]->eye = eye;  RecalculateViewMatrix();}

	D3DXVECTOR3 GetLookAtPos() {return m_cameraList[m_activeCamera]->lookAt;}
	void SetLookAtPos(D3DXVECTOR3 lookAt) {m_cameraList[m_activeCamera]->lookAt = lookAt;  RecalculateViewMatrix();}

	D3DXVECTOR3 GetUpVec() {return m_cameraList[m_activeCamera]->up;}
	void SetUpVec(D3DXVECTOR3 up) {m_cameraList[m_activeCamera]->up = up;  RecalculateViewMatrix();}
};
