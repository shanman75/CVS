#pragma once
#include "camera.h"

class cCameraAboveTerrain :
  public cCamera
{
public:
  cCameraAboveTerrain(void);
  ~cCameraAboveTerrain(void);
  void SetCamera();
};
