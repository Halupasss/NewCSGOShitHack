#include <Windows.h>
#include "constvars.hpp"

#define LOCALPLAYER *(uintptr_t*)(CLIENT + offsets::dwLocalPlayer)
#define LOCALPLAYERADDRESS = (CLIENT + offsets::dwLocalPlayer)

static class LocalPlayer
{
public:
	VOID Shoot()
	{
		uintptr_t client = CLIENT;

		*(int*)(client + offsets::dwForceAttack) = cVars::Flags::Active;
		Sleep(20);
		*(int*)(client + offsets::dwForceAttack) = cVars::Flags::InActive;
	}

	VOID Jump()
	{
		uintptr_t client = CLIENT;

		*(int*)(client + offsets::dwForceJump) = cVars::Flags::Active;
		Sleep(17);
		*(int*)(client + offsets::dwForceJump) = cVars::Flags::InActive;
	}

	short GetCurrentWeapon()
	{
		int weapon = *(int*)(LOCALPLAYER + offsets::m_hActiveWeapon);
		int weaponEnt = *(int*)(CLIENT + offsets::dwEntityList + ((weapon & 0xFFF) - 1) * 0x10);

		short weaponID = *(short*)(weaponEnt + offsets::m_iItemDefinitionIndex);

		return weaponID;
	}

	Vec3 Velocity()
	{
		return *(Vec3*)(LOCALPLAYER + offsets::m_vecVelocity);
	}

	Vec3 Position()
	{
		return *(Vec3*)(LOCALPLAYER + offsets::m_vecOrigin);
	}

	Vec3 ViewAngles()
	{
		return *(Vec3*)(CLIENTSTATE + offsets::dwClientState_ViewAngles);
	}
	
	int Health()
	{
		return *(__int32*)(LOCALPLAYER + offsets::m_iHealth);
	}

	int TeamNum()
	{
		return *(__int32*)(LOCALPLAYER + offsets::m_iTeamNum);
	}

	int Armor()
	{
		return *(__int32*)(LOCALPLAYER + offsets::m_ArmorValue);
	}

	BYTE Flags()
	{
		return *(BYTE*)(LOCALPLAYER + offsets::m_fFlags);
	}

	BOOL isDead()
	{
		return (Health() <= 0);
	}

	BOOL CheckIfScoped()
	{
		return *(bool*)(LOCALPLAYER + offsets::m_bIsScoped);
	}
} lPlayer;