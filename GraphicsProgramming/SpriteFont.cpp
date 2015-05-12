#include "SpriteFont.h"

#include <fstream>


SpriteFont::SpriteFont(char* p_pFileName, ID3D11Device* p_pDevice)
{
	std::ifstream _FileStream(p_pFileName, std::ios::binary);

	int VersionID;

	_FileStream.read((char*)&VersionID, sizeof(INT32));

	// Fehler: Falsche Version
	if ((VersionID & 0xFF000000) != 0x03000000)
		return;

	// Fehler: File Corrupted oder File ist kein BMFont
	if ((VersionID & 0x00FFFFFF) != 0x00464d42)
		return;

	char _CurrentBlockType;
	int _CurrentBlockSize;

	char _Trash[1024];
	char _Pages[256];

	int _CharCount;
	CharDescFF _Desc;


	while (!_FileStream.eof())
	{
		// Schauen welcher Block gerade bearbeitet wird
		_CurrentBlockType = _FileStream.get();
		_FileStream.read((char*)&_CurrentBlockSize, sizeof(int));

		// Je nach Block Daten verarbeiten
		switch (_CurrentBlockType)
		{
		case 1:
			_FileStream.read(_Trash, _CurrentBlockSize);
			break;
		case 2:
			_FileStream.read(_Trash, _CurrentBlockSize);
			break;
		case 3:
			_FileStream.read(_Pages, _CurrentBlockSize);
			m_pSpriteSheet = new Texture(_Pages, p_pDevice);
			break;
		case 4:
			_CharCount = _CurrentBlockSize / 20;

			for (int x = 0; x < _CharCount; x++)
			{
				

				_FileStream.read((char*)&_Desc, sizeof(CharDescFF));

				m_Chars[_Desc.m_ID].m_X = _Desc.m_X;
				m_Chars[_Desc.m_ID].m_Y = _Desc.m_Y;
				m_Chars[_Desc.m_ID].m_Width = _Desc.m_Width;
				m_Chars[_Desc.m_ID].m_Height = _Desc.m_Height;

				m_Chars[_Desc.m_ID].m_RelativeWidth = _Desc.m_Width / 48.0f;
				m_Chars[_Desc.m_ID].m_RelativeHeight = _Desc.m_Height / 48.0f;

				m_Chars[_Desc.m_ID].m_RelativeOffsetY = _Desc.m_YOffset / 48.0f;

			}
			break;
		case 5:
			_FileStream.read(_Trash, _CurrentBlockSize);
			break;
		}

	}
}