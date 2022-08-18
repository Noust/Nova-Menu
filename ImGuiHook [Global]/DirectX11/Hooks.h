#pragma once
bool hooked = false;
DWORD64 HookAddr;
DWORD64 PatchAddr;
DWORD64 PatchAddr1;
DWORD64 ViewAddr;
DWORD64 jmpback;

class Entitys
{
public:
	char pad_0000[44]; //0x0000
	int32_t invicible; //0x002C
	class Posclass* RealP; //0x0030
	char pad_0038[1]; //0x0038
	int8_t EntityType; //0x0039
	char pad_003A[86]; //0x003A
	Vector3 pos; //0x0090
	char pad_009C[237]; //0x009C
	bool GodMode; //0x0189
	char pad_018A[2]; //0x018A
	int8_t hostility; //0x018C
	char pad_018D[243]; //0x018D
	float Health; //0x0280
	char pad_0284[28]; //0x0284
	float MaxHealth; //0x02A0
	char pad_02A4[124]; //0x02A4
	Vector3 Velocity; //0x0320
	char pad_032C[2564]; //0x032C
	class Car* CarPtr; //0x0D30
	char pad_0D38[896]; //0x0D38
	int8_t radgoll; //0x10B8
	char pad_10B9[15]; //0x10B9
	class PlayerInfo* PlayerInfoPtr; //0x10C8
	char pad_10D0[8]; //0x10D0
	class WeaponManager* Weapon; //0x10D8
	char pad_10E0[907]; //0x10E0
	int8_t IsInCar; //0x146B
	char pad_146C[196]; //0x146C
	float Armor; //0x1530
	char pad_1534[2904]; //0x1534
}; //Size: 0x208C

class Posclass
{
public:
	char pad_0000[80]; //0x0000
	Vector3 Realpos; //0x0050
	char pad_005C[48]; //0x005C
}; //Size: 0x008C

class Car
{
public:
	char pad_0000[48]; //0x0000
	class CarRP* CarPos; //0x0030
	char pad_0038[337]; //0x0038
	bool GodMode; //0x0189
	char pad_018A[1918]; //0x018A
	float Health; //0x0908
	char pad_090C[44]; //0x090C
	class CustomCar* CarCustom; //0x0938
	char pad_0940[796]; //0x0940
	float GravityCar; //0x0C5C
	char pad_0C60[3076]; //0x0C60
}; //Size: 0x1864

class PlayerInfo
{
public:
	char pad_0000[164]; //0x0000
	char Names[35]; //0x00A4
	char pad_00C7[169]; //0x00C7
	float SwimSpeed; //0x0170
	char pad_0174[1812]; //0x0174
	int32_t WantedLevel; //0x0888
	char pad_088C[1124]; //0x088C
	float RunSpeed; //0x0CF0
	float StaminaMax; //0x0CF4
	char pad_0CF8[8]; //0x0CF8
}; //Size: 0x2188

class WeaponManager
{
public:
	char pad_0000[32]; //0x0000
	class weapon1* Weaponptr; //0x0020
	char pad_0028[2392]; //0x0028
}; //Size: 0x0980

class CustomCar
{
public:
	char pad_0000[12]; //0x0000
	float Mass; //0x000C
	char pad_0010[48]; //0x0010
	float BUOYANCY; //0x0040
	char pad_0044[8]; //0x0044
	float ACCELERATION; //0x004C
	char pad_0050[8]; //0x0050
	float UPSHIFT; //0x0058
	float DOWNSHIFT; //0x005C
	char pad_0060[12]; //0x0060
	float BRAKEFORCE; //0x006C
	char pad_0070[12]; //0x0070
	float HANDBRAKEFORCE; //0x007C
	char pad_0080[16]; //0x0080
	float TRACTION_CURVE_MIN; //0x0090
	char pad_0094[40]; //0x0094
	float SUSPENSION_FORCE; //0x00BC
	char pad_00C0[16]; //0x00C0
	float SUSPENSION_HEIGH; //0x00D0
	char pad_00D4[28]; //0x00D4
	float COLISION_DAMAGE_MP; //0x00F0
	float WEAPON_DAMAGE_MP; //0x00F4
	float DEFORM_MULTIPLIER; //0x00F8
	float ENGINE_DAMAGE_MP; //0x00FC
	char pad_0100[28]; //0x0100
}; //Size: 0x011C

class CarRP
{
public:
	char pad_0000[80]; //0x0000
	Vector3 CarRealPos; //0x0050
	char pad_005C[48]; //0x005C
}; //Size: 0x008C

class weapon1
{
public:
	char pad_0000[96]; //0x0000
	class Weapon2* Weaponptr2; //0x0060
	char pad_0068[32]; //0x0068
}; //Size: 0x0088

class Weapon2
{
public:
	char pad_0000[8]; //0x0000
	class Weapon3* Weaponptr3; //0x0008
	char pad_0010[120]; //0x0010
}; //Size: 0x0088

class Weapon3
{
public:
	class Weapon4* Weaponptr4; //0x0000
	char pad_0008[128]; //0x0008
}; //Size: 0x0088

class Weapon4
{
public:
	char pad_0000[24]; //0x0000
	int32_t CurrentAmmo; //0x0018
	char pad_001C[108]; //0x001C
}; //Size: 0x0088

Entitys* ents;
Entitys* local;
Entitys* ents3;
Entitys* entsK;

__declspec(naked) void GetView() {
	__asm{
		mov[ViewAddr], rax
		movss[rbp + 0x00], xmm0
		movaps xmm4, [rax]
		movaps xmm5, [rax + 0x10]
		movaps xmm6, [rax + 0x20]
		jmp[jmpback]
	}
}