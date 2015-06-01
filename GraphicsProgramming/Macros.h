#pragma once



#define DX_RELEASE(x) if(x) \
{ \
	x->Release(); \
	x = nullptr; \
	}



//#define DX_RELEASE(x) if(x) { x->Release(); x = nullptr; }