#ifndef _minimap_h
#define _minimap_h

#include <d3d9.h>
#include <d3dx9.h>
#include "D3DObject.h"

class minimap
{  public:	minimap();
			void drawmap(float x=0,float y=0);
			~minimap();
   private: LPD3DXSPRITE map;
};
#endif