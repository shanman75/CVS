#pragma once
#include "camera.h"
#include "GameState.h"

class cCameraAboveHit :
  public cCamera
{
public:
  cCameraAboveHit(void);
  ~cCameraAboveHit(void);
  void SetCamera(void);
};
