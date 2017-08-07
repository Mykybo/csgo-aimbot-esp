#pragma once
#include "Vector.h"
#include "interfaces\InterfaceEngine.h"
#include "interfaces\InterfaceTrace.h"
#include "interfaces\InterfaceModelInfo.h"
#include "interfaces\InterfaceEntityList.h"
#include <random>

struct Color {
	int r, g, b, a;
	Color(int r, int g, int b, int a) : r(r), g(g), b(b), a(a)
	{

	}
};

struct GlowObjectDefinition
{
	CBaseEntity* pEntity;
	float m_flRed;
	float m_flGreen;
	float m_flBlue;
	float m_flAlpha;
	BYTE pad0[16];
	bool m_bRenderWhenOccluded;
	bool m_bRenderWhenUnoccluded;
	bool m_bFullBloomRender;
	BYTE pad1[15];
};

//class CGlowObjectManager
//{
//public:
//	GlowObjectDefinition* m_ppGlowObjectDefinitions;
//	int32_t                m_MaxGlowObjects;
//private:
//	PAD(0x4);
//public:
//	int32_t                m_nGlowObjects;
//};

class CUserCmd
{
private:
	char pad_0x0[0x4];
public:
	int		command_number;		// 0x04 For matching server and client commands for debugging
	int		tick_count;			// 0x08 the tick the client created this command
	Vector	viewangles;			// 0x0C Player instantaneous view angles.
	Vector	aimdirection;		// 0x18
	float	forwardmove;		// 0x24
	float	sidemove;			// 0x28
	float	upmove;				// 0x2C
	int		buttons;			// 0x30 Attack button states
	unsigned char    impulse;			// 0x34
	int		weaponselect;		// 0x38 Current weapon id
	int		weaponsubtype;		// 0x3C
	int		random_seed;		// 0x40 For shared random functions
	short	mousedx;			// 0x44 mouse accum in x from create move
	short	mousedy;			// 0x46 mouse accum in y from create move
	bool	hasbeenpredicted;	// 0x48 Client only, tracks whether we've predicted this command at least once
private:
	char	pad_0x4C[0x18];		// 0x4C Current sizeof( usercmd ) =  100  = 0x64
};

// Player Controls for CS:GO
enum playercontrols
{
	IN_ATTACK = (1 << 0),
	IN_JUMP = (1 << 1),
	IN_DUCK = (1 << 2),
	IN_FORWARD = (1 << 3),
	IN_BACK = (1 << 4),
	IN_USE = (1 << 5),
	IN_CANCEL = (1 << 6),
	IN_LEFT = (1 << 7),
	IN_RIGHT = (1 << 8),
	IN_MOVELEFT = (1 << 9),
	IN_MOVERIGHT = (1 << 10),
	IN_ATTACK2 = (1 << 11),
	IN_RUN = (1 << 12),
	IN_RELOAD = (1 << 13),
	IN_ALT1 = (1 << 14),
	IN_ALT2 = (1 << 15),
	IN_SCORE = (1 << 16),	// Used by client.dll for when scoreboard is held down
	IN_SPEED = (1 << 17),	// Player is holding the speed key
	IN_WALK = (1 << 18),	// Player holding walk key
	IN_ZOOM = (1 << 19),	// Zoom key for HUD zoom
	IN_WEAPON1 = (1 << 20),	// weapon defines these bits
	IN_WEAPON2 = (1 << 21),	// weapon defines these bits
	IN_BULLRUSH = (1 << 22),
};

enum LifeState {
	LIFE_ALIVE = 0,// alive
	LIFE_DYING = 1, // playing death animation or still falling off of a ledge waiting to hit ground
	LIFE_DEAD = 2 // dead. lying still.
};


enum ClassID
{
	TestTraceline = 192,
	TEWorldDecal = 193,
	TESpriteSpray = 190,
	TESprite = 189,
	TESparks = 188,
	TESmoke = 187,
	TEShowLine = 185,
	TEProjectedDecal = 182,
	TEPlayerDecal = 181,
	TEPhysicsProp = 178,
	TEParticleSystem = 177,
	TEMuzzleFlash = 176,
	TELargeFunnel = 174,
	TEKillPlayerAttachments = 173,
	TEImpact = 172,
	TEGlowSprite = 171,
	TEShatterSurface = 184,
	TEFootprintDecal = 168,
	TEFizz = 167,
	TEExplosion = 165,
	TEEnergySplash = 164,
	TEEffectDispatch = 163,
	TEDynamicLight = 162,
	TEDecal = 160,
	TEClientProjectile = 159,
	TEBubbleTrail = 158,
	TEBubbles = 157,
	TEBSPDecal = 156,
	TEBreakModel = 155,
	TEBloodStream = 154,
	TEBloodSprite = 153,
	TEBeamSpline = 152,
	TEBeamRingPoint = 151,
	TEBeamRing = 150,
	TEBeamPoints = 149,
	TEBeamLaser = 148,
	TEBeamFollow = 147,
	TEBeamEnts = 146,
	TEBeamEntPoint = 145,
	TEBaseBeam = 144,
	TEArmorRicochet = 143,
	TEMetalSparks = 175,
	SteamJet = 138,
	SmokeStack = 131,
	DustTrail = 242,
	FireTrail = 62,
	SporeTrail = 248,
	SporeExplosion = 247,
	RocketTrail = 245,
	SmokeTrail = 246,
	PropVehicleDriveable = 118,
	ParticleSmokeGrenade = 244,
	ParticleFire = 97,
	MovieExplosion = 243,
	TEGaussExplosion = 170,
	EnvQuadraticBeam = 55,
	Embers = 45,
	EnvWind = 59,
	Precipitation = 112,
	PrecipitationBlocker = 113,
	BaseTempEntity = 18,
	NextBotCombatCharacter = 0,
	BaseAttributableItem = 4,
	EconEntity = 44,
	WeaponXM1014 = 240,
	WeaponTaser = 235,
	SmokeGrenade = 129,
	WeaponSG552 = 232,
	SensorGrenade = 125,
	WeaponSawedoff = 228,
	WeaponNOVA = 224,
	IncendiaryGrenade = 85,
	MolotovGrenade = 94,
	WeaponM3 = 216,
	KnifeGG = 91,
	Knife = 90,
	HEGrenade = 82,
	Flashbang = 64,
	WeaponElite = 207,
	DecoyGrenade = 40,
	DEagle = 39,
	WeaponUSP = 239,
	WeaponM249 = 215,
	WeaponUMP45 = 238,
	WeaponTMP = 237,
	WeaponTec9 = 236,
	WeaponSSG08 = 234,
	WeaponSG556 = 233,
	WeaponSG550 = 231,
	WeaponScout = 230,
	WeaponSCAR20 = 229,
	SCAR17 = 123,
	WeaponP90 = 227,
	WeaponP250 = 226,
	WeaponP228 = 225,
	WeaponNegev = 223,
	WeaponMP9 = 222,
	WeaponMP7 = 221,
	WeaponMP5Navy = 220,
	WeaponMag7 = 219,
	WeaponMAC10 = 218,
	WeaponM4A1 = 217,
	WeaponHKP2000 = 214,
	WeaponGlock = 213,
	WeaponGalilAR = 212,
	WeaponGalil = 211,
	WeaponG3SG1 = 210,
	WeaponFiveSeven = 209,
	WeaponFamas = 208,
	WeaponBizon = 203,
	WeaponAWP = 201,
	WeaponAug = 200,
	AK47 = 1,
	WeaponCSBaseGun = 205,
	WeaponCSBase = 204,
	C4 = 29,
	WeaponBaseItem = 202,
	BaseCSGrenade = 8,
	SmokeGrenadeProjectile = 130,
	SensorGrenadeProjectile = 126,
	MolotovProjectile = 95,
	Item_Healthshot = 89,
	DecoyProjectile = 41,
	FireCrackerBlast = 60,
	Inferno = 86,
	Chicken = 31,
	FootstepControl = 66,
	CSGameRulesProxy = 34,
	WeaponCubemap = 0,
	WeaponCycler = 206,
	TEPlantBomb = 179,
	TEFireBullets = 166,
	TERadioIcon = 183,
	PlantedC4 = 105,	
	CSTeam = 38,
	CSPlayerResource = 36,
	CSPlayer = 35,
	CSRagdoll = 37,
	TEPlayerAnimEvent = 180,
	Hostage = 83,
	HostageCarriableProp = 84,
	BaseCSGrenadeProjectile = 9,
	HandleTest = 81,
	TeamplayRoundBasedRulesProxy = 142,
	SpriteTrail = 136,
	SpriteOriented = 135,
	Sprite = 134,
	RagdollPropAttached = 121,
	RagdollProp = 120,
	PredictedViewModel = 114,
	PoseController = 110,
	GameRulesProxy = 80,
	InfoLadderDismount = 87,
	FuncLadder = 72,
	TEFoundryHelpers = 169,
	EnvDetailController = 51,
	World = 241,
	WaterLODControl = 199,
	WaterBullet = 198,
	VoteController = 197,
	VGuiScreen = 196,
	PropJeep = 117,
	PropVehicleChoreoGeneric = 0,
	TriggerSoundOperator = 195,
	BaseVPhysicsTrigger = 22,
	TriggerPlayerMovement = 194,
	BaseTrigger = 20,
	Test_ProxyToggle_Networkable = 191,
	Tesla = 186,
	BaseTeamObjectiveResource = 17,
	Team = 141,
	SunlightShadowControl = 140,
	Sun = 139,
	ParticlePerformanceMonitor = 98,
	SpotlightEnd = 133,
	SpatialEntity = 132,
	SlideshowDisplay = 128,
	ShadowControl = 127,
	SceneEntity = 124,
	RopeKeyframe = 122,
	RagdollManager = 119,
	PhysicsPropMultiplayer = 103,
	PhysBoxMultiplayer = 101,
	PropDoorRotating = 116,
	BasePropDoor = 16,
	DynamicProp = 43,
	Prop_Hallucination = 115,
	PostProcessController = 111,
	PointCommentaryNode = 109,
	PointCamera = 108,
	PlayerResource = 107,
	Plasma = 106,
	PhysMagnet = 104,
	PhysicsProp = 102,
	StatueProp = 137,
	PhysBox = 100,
	ParticleSystem = 99,
	MovieDisplay = 96,
	MaterialModifyControl = 93,
	LightGlow = 92,
	InfoOverlayAccessor = 88,
	FuncTrackTrain = 79,
	FuncSmokeVolume = 78,
	FuncRotating = 77,
	FuncReflectiveGlass = 76,
	FuncOccluder = 75,
	FuncMoveLinear = 74,
	FuncMonitor = 73,
	Func_LOD = 68,
	TEDust = 161,
	Func_Dust = 67,
	FuncConveyor = 71,
	FuncBrush = 70,
	BreakableSurface = 28,
	FuncAreaPortalWindow = 69,
	Fish = 63,
	FireSmoke = 61,
	EnvTonemapController = 58,
	EnvScreenEffect = 56,
	EnvScreenOverlay = 57,
	EnvProjectedTexture = 54,
	EnvParticleScript = 53,
	FogController = 65,
	EnvDOFController = 52,
	CascadeLight = 30,
	EnvAmbientLight = 50,
	EntityParticleTrail = 49,
	EntityFreezing = 48,
	EntityFlame = 47,
	EntityDissolve = 46,
	DynamicLight = 42,
	ColorCorrectionVolume = 33,
	ColorCorrection = 32,
	BreakableProp = 27,
	BeamSpotlight = 25,
	BaseButton = 5,
	BaseToggle = 19,
	BasePlayer = 15,
	BaseFlex = 12,
	BaseEntity = 11,
	BaseDoor = 10,
	BaseCombatCharacter = 6,
	BaseAnimatingOverlay = 3,
	BoneFollower = 26,
	BaseAnimating = 2,
	AI_BaseNPC = 0,
	Beam = 24,
	BaseViewModel = 21,
	BaseParticleEntity = 14,
	BaseGrenade = 13,
	BaseCombatWeapon = 7,
	BaseWeaponWorldModel = 23
};

class CBaseEntity;
class Utils {
public:
	static void Warning(char*, ...);
	static void Msg(char*, ...);
	static void Error(char*, ...);
	static void InitConsole(char* name);
	static trace_t TraceRay(Vector start, Vector end, CBaseEntity* skipent);
	static Vector GetHitboxPosition(int Hitbox, CBaseEntity* ent);
	static bool GetBonePosition(CBaseEntity* pEntity, Vector &Hitbox, int Bone);
	static bool IsVisible(Vector start, Vector end, CBaseEntity* skipent, CBaseEntity* target);
	static float RandomFloat(float a, float b);
	static bool IsKeyDown(UINT key);
	static void StartShooting();
	static void StopShooting();
	static bool ValidTarget(CBaseEntity* ent);
	static bool HitboxVisible(int hitbox, CBaseEntity* ent);
	static bool WeaponCheck(int id);
	static DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char * szMask)
	{
		for (DWORD i = 0; i < dwLen; i++)
			if (bDataCompare((BYTE*)(dwAddress + i), bMask, szMask))
				return (DWORD)(dwAddress + i);
		return 0;
	}

private:
	__forceinline static float DotProduct(const Vector v1, const float* v2)
	{
		return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
	}

	static void VectorTransform(const Vector in1, const VMatrix& in2, Vector& out)
	{
		out[0] = DotProduct(in1, in2[0]) + in2[0][3];
		out[1] = DotProduct(in1, in2[1]) + in2[1][3];
		out[2] = DotProduct(in1, in2[2]) + in2[2][3];
	}

	static bool bDataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
	{
		for (; *szMask; ++szMask, ++pData, ++bMask)
			if (*szMask == 'x' && *pData != *bMask)
				return false;
		return (*szMask) == NULL;
	}
	
};
