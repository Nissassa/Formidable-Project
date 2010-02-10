#include "CameraManager.h"

CCameraManager::CCameraManager(void)
{
	m_cameraIndex = 0;
}

CCameraManager::~CCameraManager(void)
{
}

void CCameraManager::SetProjectionMatrix(float fovDegrees, float aspectRatio, float nearPlane, float farPlane)
{
	m_fov = D3DXToRadian(fovDegrees);
	m_aspectRatio = aspectRatio;
	m_nearPlane = nearPlane;
	m_farPlane = farPlane;

//	D3DXMatrixPerspectiveFovLH(&m_projMat, m_fov, m_aspectRatio, m_nearPlane, m_farPlane);
	RecalculateProjectionMatrix();
}

void CCameraManager::RecalculateProjectionMatrix()
{
	D3DXMatrixPerspectiveFovLH(&m_projMat, m_fov, m_aspectRatio, m_nearPlane, m_farPlane);
}

void CCameraManager::RecalculateViewMatrix()
{
	D3DXMatrixLookAtLH(&m_cameraList[m_activeCamera]->viewMat, &m_cameraList[m_activeCamera]->eye, &m_cameraList[m_activeCamera]->lookAt, &m_cameraList[m_activeCamera]->up);

}

int CCameraManager::CreateCamera(D3DXVECTOR3 eyePos, D3DXVECTOR3 lookAt, D3DXVECTOR3 up)
{
	Camera* cam = new Camera;

	cam->eye = eyePos;
	cam->lookAt = lookAt;
	cam->up	= up;
	D3DXMatrixLookAtLH(&cam->viewMat, &cam->eye, &cam->lookAt, &cam->up);
	m_cameraList.push_back(cam);

	// Set our new camera to the active
	m_activeCamera = m_cameraIndex;
	
	return m_cameraIndex++;
}
void CCameraManager::RemoveCamera(int cameraIndex)
{
	if(m_cameraList[cameraIndex])
	{
		delete m_cameraList[cameraIndex];
		m_cameraList[cameraIndex] = 0;
	}
}