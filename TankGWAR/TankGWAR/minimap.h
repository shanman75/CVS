#ifndef _minimap_h
#define _minimap_h

#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>
#include "D3DObject.h"
#include "3DObject.h"
#include "3DObjectTank.h"
#include "gamestate.h"
#include "Texture.h"

class cGameState;
class c3DObjectTank;

class minimap
{  friend class cGameState;
   friend class c3DObjectTank;
   friend class c3DObject;
   public:	
      minimap();
			void drawmap(float x=0,float y=0);
			~minimap();
   private: 
      CTexture *tex_map;
			float xt,yt;
			void findtankcoor(float,float);
};


#endif