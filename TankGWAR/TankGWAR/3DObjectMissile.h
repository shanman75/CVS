#pragma once
#include "3dobject.h"

//enum MSLTYPE {SHELL,ATOMBOMB,SCUD,AMRAM,FUNKIEBOMB};

class c3DObjectMissile :
	public c3DObject
{
public:
  enum MSLTYPE {SHELL,ATOMBOMB,SCUD,AMRAM,FUNKIEBOMB};
  static const int MSLNUM = 5;
	c3DObjectMissile(MSLTYPE missile_type = SCUD);
	virtual ~c3DObjectMissile(void);
  void move();
  static char *GetMissileStr (enum MSLTYPE m);
  static D3DCOLORVALUE GetMissileExpColor(enum MSLTYPE m);
  static float GetMissileExpRadius(enum MSLTYPE m);
  static float GetMissileCost(enum MSLTYPE m);
  static float GetMissileLength(enum MSLTYPE m);
protected:
	static LPD3DXMESH			      m_missilemesh;
  static LPDIRECT3DTEXTURE9*	m_missiletex;
  static D3DMATERIAL9*		    m_missilemat;
  static DWORD                m_missileNmat;

  //enum MSLTYPE { SHELL, ATOMBOMB, SCUD, AMRAM, FUNKIEBOMB };
  // FUNKIEBOMB
	static LPD3DXMESH			      m_FUNKIEBOMB_mesh;
  static LPDIRECT3DTEXTURE9*	m_FUNKIEBOMB_tex;
  static D3DMATERIAL9*		    m_FUNKIEBOMB_mat;
  static DWORD                m_FUNKIEBOMB_Nmat;

  // Am-Ram
	static LPD3DXMESH			      m_AMRAM_mesh;
  static LPDIRECT3DTEXTURE9*	m_AMRAM_tex;
  static D3DMATERIAL9*		    m_AMRAM_mat;
  static DWORD                m_AMRAM_Nmat;

  // Scud
	static LPD3DXMESH			      m_SCUD_mesh;
  static LPDIRECT3DTEXTURE9*	m_SCUD_tex;
  static D3DMATERIAL9*		    m_SCUD_mat;
  static DWORD                m_SCUD_Nmat;

  // Shell
	static LPD3DXMESH			      m_SHELL_mesh;
  static LPDIRECT3DTEXTURE9*	m_SHELL_tex;
  static D3DMATERIAL9*		    m_SHELL_mat;
  static DWORD                m_SHELL_Nmat;

  // Atom Bomb
	static LPD3DXMESH			      m_ATOMBOMB_mesh;
  static LPDIRECT3DTEXTURE9*	m_ATOMBOMB_tex;
  static D3DMATERIAL9*		    m_ATOMBOMB_mat;
  static DWORD                m_ATOMBOMB_Nmat;


	static int m_graph_init;

	void _LoadGraphics(void);
	void _UnloadGraphics();

  float m_initYvelocity;

};
