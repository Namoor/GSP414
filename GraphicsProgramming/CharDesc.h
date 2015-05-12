#pragma once

#include "d3dinclude.h"




// Speichert alle relevanten daten für einen einzelnen Buchstaben
struct CharDesc
{
	// Position und Größe des Buchstabens innerhalb des SpriteSheets
	float m_X;
	float m_Y;
	float m_Width;
	float m_Height;

	// Größe des Buchstabens auf dem Bildschirm als Prozentsatz von Schriftgröße
	float m_RelativeWidth;
	float m_RelativeHeight;

	// Vertikale Versetzung des Buchstabens
	float m_RelativeOffsetY;
};


struct CharDescFF
{
	UINT32 m_ID;
	UINT16 m_X;
	UINT16 m_Y;
	UINT16 m_Width;
	UINT16 m_Height;
	INT16 m_XOffset;
	INT16 m_YOffset;
	INT16 m_XAdvance;
	UINT8 m_Page;
	UINT8 m_Channel;

};