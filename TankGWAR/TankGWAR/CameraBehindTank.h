#pragma once
#include "camera.h"

class cCameraBehindTank :
  public cCamera
{
public:
  cCameraBehindTank(void);
  ~cCameraBehindTank(void);
  void SetCamera(void);
};
