#pragma once
#include "camera.h"

class cCameraBehindMissile :
  public cCamera
{
public:
  cCameraBehindMissile(void);
  ~cCameraBehindMissile(void);
  void SetCamera(void);
};
