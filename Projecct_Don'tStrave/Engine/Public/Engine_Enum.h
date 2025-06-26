#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum class WINMODE { FULL, WIN, END };	
	enum class PROTOTYPE { GAMEOBJECT, COMPONENT };
	enum class WORLDSTATE { RIGHT, UP, LOOK, POSITION, END };
	enum class RENDER { PRIORITY, NONBLEND, BLEND, PROJECTION_UI, ORTTHO_UI, END};
	enum class TEXTURE { PLANE, CUBE, END };
	enum class RAY { VIEW, WORLD, END };

	enum CHANNELID { SOUND_EFFECT, SOUND_BGM, SOUND_END };
}
#endif // Engine_Enum_h__
