//#ifndef ESP_H
//#define ESP_H
//
//#include "Engine.h"
//#include "Settings.h"
//
//struct GlowObjectDefinition_t
//{
//	DWORD dwBase;
//	float r;
//	float g;
//	float b;
//	float a;
//	bool m_bRenderWhenOccluded;
//	bool m_bRenderWhenUnoccluded;
//	bool m_bFullBloom;
//};
//
//class ESP {
//
//public:
//	void run() {
//		int objectCount;
//		DWORD pointerGlow;
//		float playerhp;
//		bool enemyHealthBased = true;
//		bool friendHealthBased = true;
//		float alphaFriend = 120;
//		float alphaEnemy = 120;
//		float alphaInX = 255;
//		//if (!ENABLED) {
//		//	return;
//		//}
//		while (true) {
//			//if (!Settings()->espEnabled) {
//			//	Sleep(3000);
//			//	continue;
//			//}
//			pointerGlow = Engine()->Memory()->Read<DWORD>(Engine()->getClientModule()->dwBase + Engine()->Offsets()->dwGlow);
//			objectCount = Engine()->Memory()->Read<DWORD>(Engine()->getClientModule()->dwBase + Engine()->Offsets()->dwGlow + 0x4);
//			if (!(pointerGlow != NULL && objectCount > 0)) {
//				Sleep(3000);
//				continue;
//			}
//			for (int i = 0; i < objectCount; i++) {
//				DWORD mObj = pointerGlow + (i * sizeof(GlowObjectDefinition_t));
//				GlowObjectDefinition_t glowObj = Engine()->Memory()->Read<GlowObjectDefinition_t>(mObj);
//				Entity* Player = Engine()->getEntityByBase(glowObj.dwBase);
//				if (!(glowObj.dwBase && Player && !Player->is_dormant && Player->is_alive)) {
//					continue;
//				}
//				// Player->hp can be greater than 100.0f on custom servers.
//				playerhp = Player->hp >= 100 ? 100.0f : (float)Player->hp / 100.0f;
//
//				glowPlayer(glowObj, 1, 1, 0, alphaEnemy);
//				
//				Engine()->Memory()->Write<GlowObjectDefinition_t>(mObj, glowObj);
//			}
//			Sleep(50);
//		}
//	}
//
//private:
//	void glowPlayer(GlowObjectDefinition_t &glowObj, float r, float g, float b, float a) {
//		glowObj.r = r;
//		glowObj.g = g;
//		glowObj.b = b;
//		glowObj.a = a;
//		glowObj.m_bRenderWhenOccluded = true;
//		//glowObj.m_bRenderWhenUnoccluded = false;
//		//glowObj.m_bFullBloom = false;
//	}
//
//
//};
//
//#endif