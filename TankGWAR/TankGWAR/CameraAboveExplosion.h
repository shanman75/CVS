#pragma once
#include "camera.h"
#include "Timer.h"

class cCameraAboveExplosion :
  public cCamera
{
public:
  cCameraAboveExplosion(void);
  ~cCameraAboveExplosion(void);
  void SetCamera(void);

  CTimer m_camtime;
};
