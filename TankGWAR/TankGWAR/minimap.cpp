#include "stdafx.h"
#include "minimap.h"

extern cGameState *g_GameState;
extern D3DObject *g_D3DObject;							// Main D3D Object

minimap::minimap()
{	map=NULL;   
	D3DXCreateSprite(g_D3DObject->m_d3ddevice9,&map);
}

void minimap::findtankcoor(float x,float y)
{	char tankcoordinates[65];
    sprintf(tankcoordinates,"Tanks in squares:\n");
	int ycoord=(int)y+220;
	g_D3DObject->DrawTextStr((int)x,ycoord,D3DCOLOR_XRGB(240,240,50),tankcoordinates);
	for(int i=0;i<(g_GameState->returnplayers());i++)
	{	if(((g_GameState->m_PlayerState[i].object)->m_position.x)<=-90.0f)
	    { xt=0;
	      if((g_GameState->m_PlayerState[i].object)->m_position.z>=90.0f)
			  yt=0;
	      else if((g_GameState->m_PlayerState[i].object)->m_position.z<90.0f&&
			 (g_GameState->m_PlayerState[i].object)->m_position.z>=30.0f)
			  yt=1;
	      else if((g_GameState->m_PlayerState[i].object)->m_position.z<30.0f&&
			 (g_GameState->m_PlayerState[i].object)->m_position.z>=-30.0f)
			  yt=2;
	      else if((g_GameState->m_PlayerState[i].object)->m_position.z<-30.0f&&
			  (g_GameState->m_PlayerState[i].object)->m_position.z>=-90.0f)
			  yt=3;
	      else if((g_GameState->m_PlayerState[i].object)->m_position.z<-90.0f)
			  yt=4;
		}

		else if((g_GameState->m_PlayerState[i].object)->m_position.x>-90.0f&&
		   (g_GameState->m_PlayerState[i].object)->m_position.x<=-30.0f)
	    { xt=1;
	      if((g_GameState->m_PlayerState[i].object)->m_position.z>=90.0f)
			  yt=0;
	      else if((g_GameState->m_PlayerState[i].object)->m_position.z<90.0f&&
			 (g_GameState->m_PlayerState[i].object)->m_position.z>=30.0f)
			  yt=1;
	      else if((g_GameState->m_PlayerState[i].object)->m_position.z<30.0f&&
			 (g_GameState->m_PlayerState[i].object)->m_position.z>=-30.0f)
			  yt=2;
	      else if((g_GameState->m_PlayerState[i].object)->m_position.z<-30.0f&&
			 (g_GameState->m_PlayerState[i].object)->m_position.z>=-90.0f)
			  yt=3;
	      else if((g_GameState->m_PlayerState[i].object)->m_position.z<-90.0f)
			  yt=4;
		}

		else if((g_GameState->m_PlayerState[i].object)->m_position.x>-30.0f&&
		   (g_GameState->m_PlayerState[i].object)->m_position.x<=30.0f)
	    { xt=2;
	      if((g_GameState->m_PlayerState[i].object)->m_position.z>=90.0f)
			  yt=0;
	      else if((g_GameState->m_PlayerState[i].object)->m_position.z<90.0f&&
			 (g_GameState->m_PlayerState[i].object)->m_position.z>=30.0f)
			  yt=1;
	      else if((g_GameState->m_PlayerState[i].object)->m_position.z<30.0f&&
			 (g_GameState->m_PlayerState[i].object)->m_position.z>=-30.0f)
			  yt=2;
	      else if((g_GameState->m_PlayerState[i].object)->m_position.z<-30.0f&&
			 (g_GameState->m_PlayerState[i].object)->m_position.z>=-90.0f)
			  yt=3;
	      else if((g_GameState->m_PlayerState[i].object)->m_position.z<-90.0f)
			  yt=4;
		}

		else if((g_GameState->m_PlayerState[i].object)->m_position.x>30.0f&&
		   (g_GameState->m_PlayerState[i].object)->m_position.x<=90.0f)
	    { xt=3;
	      if((g_GameState->m_PlayerState[i].object)->m_position.z>=90.0f)
			  yt=0;
	      if((g_GameState->m_PlayerState[i].object)->m_position.z<90.0f&&
			 (g_GameState->m_PlayerState[i].object)->m_position.z>=30.0f)
			  yt=1;
	      if((g_GameState->m_PlayerState[i].object)->m_position.z<30.0f&&
			 (g_GameState->m_PlayerState[i].object)->m_position.z>=-30.0f)
			  yt=2;
	      if((g_GameState->m_PlayerState[i].object)->m_position.z<-30.0f&&
			 (g_GameState->m_PlayerState[i].object)->m_position.z>=-90.0f)
			  yt=3;
	      if((g_GameState->m_PlayerState[i].object)->m_position.z<-90.0f)
			  yt=4;
		}

		else if((g_GameState->m_PlayerState[i].object)->m_position.x>90.0f)
	    { xt=4;
	      if((g_GameState->m_PlayerState[i].object)->m_position.z>=90.0f)
			  yt=0;
	      if((g_GameState->m_PlayerState[i].object)->m_position.z<90.0f&&
			 (g_GameState->m_PlayerState[i].object)->m_position.z>=30.0f)
			  yt=1;
	      if((g_GameState->m_PlayerState[i].object)->m_position.z<30.0f&&
			 (g_GameState->m_PlayerState[i].object)->m_position.z>=-30.0f)
			  yt=2;
	      if((g_GameState->m_PlayerState[i].object)->m_position.z<-30.0f&&
			 (g_GameState->m_PlayerState[i].object)->m_position.z>=-90.0f)
			  yt=3;
	      if((g_GameState->m_PlayerState[i].object)->m_position.z<-90.0f)
			  yt=4;
		}
		sprintf(tankcoordinates,"(col:%d,row:%d)\n",(int)xt,(int)yt);
		ycoord+=25;
		g_D3DObject->DrawTextStr((int)x,ycoord,D3DCOLOR_XRGB(240,240,50),tankcoordinates);
	}
}

void minimap::drawmap(float x,float y)
{	D3DXIMAGE_INFO structImageInfo; //image information
    LPDIRECT3DTEXTURE9 maptex;

    HRESULT hres=D3DXCreateTextureFromFileEx(g_D3DObject->m_d3ddevice9,
	"resource\\tankmap.jpg",0,0,1,0,D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,
		D3DX_FILTER_NONE,D3DX_DEFAULT,0,&structImageInfo,NULL,&maptex);

	D3DXVECTOR3 mapcoordinates;
	mapcoordinates.x=x;
	mapcoordinates.y=y;
	mapcoordinates.z=0;

	map->Begin(NULL);
	map->Draw(maptex,NULL,NULL,&mapcoordinates,0xFFFFFFFF);
	map->End();

	findtankcoor(x,y);

	maptex->Release();
}

minimap::~minimap()
{   map->Release();
}
