#include "includes.h"

int config::FOV = 90;
int config::SmoothStep = 1;
int config::SpinBotSpeed = 0;
int config::TargetBonePos = constVars.HeadBone;
int config::LegitAimBotDiff = 300;

bool config::console = false;
bool config::Bhop = false;
bool config::HealthTreshold = false;
bool config::TriggerBot = false;
bool config::NeonWallHack = false;
bool config::AimBot = false;
bool config::RecoilControlSystem = false;
bool config::RadarHack = false;
bool config::NoFlash = true;
bool config::WallHackESP = false;
bool config::SpinBot = false;
bool config::SmoothAimBot = false;
bool config::ThirdPersonView = false;
bool config::TPSActive = false;
bool config::AimLegitMode = false;
bool config::AimingAssistance = false;
bool config::TriggerBotInAimBot = false;

bool config::ImGui_Init = false;
bool config::ImGui_Attached = false;

int config::esp::BoxWidth = 3;
int config::esp::LineWidth = 1;

int config::CurrentSkinID = 0;
int config::HealthTresholdVal = 33;
int config::BhopDelay = 17;
int config::TriggerBotCooldown = 65;
int config::MainThreadDelay = 1;

int config::AimBotHotKey = 18;   // VK_ALT
int config::EndHotKey = 0x23;   // VK_END
int config::MenuHotKey = 0x24; // VK_HOME
bool config::MenuActive = false;

float config::esp::LT_NEONESP[3] = { 0, 1, 1 };
float config::esp::ET_NEONESP[3] = { 1, 0, 1 };
float config::esp::DX_ESP[3] = { 1, 0, 1 };
float config::esp::health::color[3] = { 1, 0, 1 };
float config::esp::weapon::color[3] = { 0, 1, 1 };

int config::esp::health::offset_x = 0;
int config::esp::health::offset_y = 0;
int config::esp::weapon::offset_x = 20;
int config::esp::weapon::offset_y = 0;
int config::esp::health::BarsPos = 0; // 0 - top, 1 - left, 2 - bottom, 3 - right

bool config::esp::health::HealthBar = false;
bool config::esp::health::ArmorBar = false;
bool config::esp::weapon::ShowWeapon = false;
bool config::esp::HP = false;
bool config::esp::ESPBones = false;
bool config::esp::Lines = true;
bool config::esp::health::custom_color = false;
bool config::esp::weapon::custom_color = false;

void ToggleButton(const char* str_id, bool* v)
{
	ImVec2 p = ImGui::GetCursorScreenPos();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	float height = ImGui::GetFrameHeight();
	float width = height * 1.55f;
	float radius = height * 0.50f;

	ImGui::InvisibleButton(str_id, ImVec2(width, height));
	if (ImGui::IsItemClicked())
		*v = !*v;

	float t = *v ? 1.0f : 0.0f;

	ImGuiContext& g = *GImGui;
	float ANIM_SPEED = 0.08f;
	if (g.LastActiveId == g.CurrentWindow->GetID(str_id))
	{
		float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
		t = *v ? (t_anim) : (1.0f - t_anim);
	}

	ImU32 col_bg;
	if (ImGui::IsItemHovered())
		col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.78f, 0.78f, 0.78f, 1.0f), ImVec4(0.64f, 0.83f, 0.34f, 1.0f), t));
	else
		col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.85f, 0.85f, 0.85f, 1.0f), ImVec4(0.56f, 0.83f, 0.26f, 1.0f), t));

	draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
	draw_list->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
}

static int CurrTab = 0;
static bool BonesIds = false;

void HACK::LoadImageToDll(BYTE* Image, LPDIRECT3DDEVICE9& pDevice)
{
	D3DXCreateTextureFromFileInMemoryEx(pDevice, Image, sizeof(Image), 495, 659, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &tImage);
}

void HACK::MenuThread()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (!ImGui::Begin("Settings"))
	{
		ImGui::End();
		return;
	}
	ImGui::SetWindowSize(ImVec2(700, 650));

	{   /*                 Tabs                  */
		if (ImGui::Button("Main", ImVec2(150, 30)))
			CurrTab = 0;

		ImGui::SameLine();
		if (ImGui::Button("Visuals", ImVec2(150, 30)))
			CurrTab = 1;

		ImGui::SameLine();
		if (ImGui::Button("Aimbot", ImVec2(150, 30)))
			CurrTab = 2;

		ImGui::SameLine();
		if (ImGui::Button("SkinChanger", ImVec2(150, 30)))
			CurrTab = 3;
	}

	switch (CurrTab)
	{
		case 0: /* Main */
		{
			using namespace config;

			ImGui::Separator();

			ImGui::Text("Main Functions");
			ImGui::Separator();

			if (ImGui::RadioButton("Bhop", Bhop))
				Bhop = !Bhop;
			if (ImGui::RadioButton("AimBot", AimBot))
				AimBot = !AimBot;
			if (ImGui::RadioButton("Glow ESP", NeonWallHack))
				NeonWallHack = !NeonWallHack;
			if (ImGui::RadioButton("Default ESP", WallHackESP))
				WallHackESP = !WallHackESP;
			if (ImGui::RadioButton("Recoil Control System", RecoilControlSystem))
				RecoilControlSystem = RecoilControlSystem;
			if (ImGui::RadioButton("Radar Hack", RadarHack))
				RadarHack = !RadarHack;
			if (ImGui::RadioButton("Trigger Bot", config::TriggerBot))
				TriggerBot = !TriggerBot;
		}
		case 1: /* Visuals */
		{
			using namespace config::esp;

			ImGui::Text("View");
			ImGui::Separator();
			ImGui::SliderInt("FOV Slider", &config::FOV, 90, 170);
			ImGui::SameLine();
			if (ImGui::Button("Reset", ImVec2(100, 25)))
			{
				config::FOV = 90;
			}

			if (ImGui::RadioButton("Third Person Mode", config::ThirdPersonView))
				config::ThirdPersonView = !config::ThirdPersonView;
			
			// TODO: the ability to shift the position of the weapon

			ImGui::Text("ESP");
			ImGui::Separator();

			if (ImGui::RadioButton("Show Boxes", config::WallHackESP))
				config::WallHackESP = !config::WallHackESP;
			if (ImGui::RadioButton("Show Lines", Lines))
				Lines = !Lines;
			if (ImGui::RadioButton("Show Health", HP))
				HP = !HP;
			if (ImGui::RadioButton("Show Bones", ESPBones))
				ESPBones = !ESPBones;
			if (ImGui::RadioButton("Show Weapon", weapon::ShowWeapon))
				weapon::ShowWeapon = !weapon::ShowWeapon;
			if (ImGui::RadioButton("Show HealthBar", health::HealthBar))
				health::HealthBar = !health::HealthBar;
			if (ImGui::RadioButton("Show ArmorBar", health::ArmorBar))
				health::ArmorBar = !health::ArmorBar;

			if (health::HealthBar || health::ArmorBar)
			{
				ImGui::SameLine();
				ImGui::InputInt("HB Pos", &health::BarsPos, 1, 1);
			}

			ImGui::Text("Colors");
			ImGui::Separator();

			ImGui::ColorEdit4("Boxes", config::esp::DX_ESP);
			ImGui::ColorEdit4("Weapon", config::esp::weapon::color);
			ImGui::ColorEdit4("Health", config::esp::health::color);
			ImGui::ColorEdit4("Glow (ET)", config::esp::ET_NEONESP);
			ImGui::ColorEdit4("Glow (LT)", config::esp::LT_NEONESP);

			ImGui::Text("Offsets");
			ImGui::Separator();

			ImGui::SliderInt("Health x", &config::esp::health::offset_x, -150, 150);
			ImGui::SliderInt("Health y", &config::esp::health::offset_y, -150, 150);
			ImGui::SliderInt("Weapon x", &config::esp::weapon::offset_x, -150, 150);
			ImGui::SliderInt("Weapon y", &config::esp::weapon::offset_y, -150, 150);
		}
		case 2: /* AimBot */
		{
			using namespace config;

			ImGui::Text("AimBot");
			ImGui::Separator();

			if (ImGui::RadioButton("SpinBot", SpinBot))
				SpinBot = !SpinBot;

			if (SpinBot)
			{
				ImGui::SameLine();
				ImGui::SliderInt("Speed", &SpinBotSpeed, 0, 220);
			}

			if (ImGui::RadioButton("TriggerBot", TriggerBot))
				TriggerBot = !TriggerBot;

			if (TriggerBot)
			{
				ImGui::SameLine();
				ImGui::SliderInt("cooldown", &TriggerBotCooldown, 20, 1000);
			}

			if (ImGui::RadioButton("Health Treshold", HealthTreshold))
				HealthTreshold = !HealthTreshold;

			if (HealthTreshold)
			{
				ImGui::SameLine();
				ImGui::SliderInt("Val", &config::HealthTresholdVal, 1, 100);
			}

			ImGui::InputInt("Target Bone", &config::TargetBonePos, 1, 79, 0);

			ImGui::Text("Legit");
			ImGui::Separator();

			if (ImGui::RadioButton("Smooth Mode", SmoothAimBot))
				SmoothAimBot = !SmoothAimBot;

			if (SmoothAimBot)
			{
				ImGui::SameLine();
				ImGui::SliderInt("val", &config::SmoothStep, 1, 180);
			}

			if (ImGui::RadioButton("through walls", AimLegitMode))
				AimLegitMode = !AimLegitMode;

			if (ImGui::RadioButton("Aiming assistance", AimingAssistance))
				AimingAssistance = !AimingAssistance;

			if (AimingAssistance)
			{
				ImGui::SameLine();
				ImGui::SliderInt("Treshold", &config::LegitAimBotDiff, 1, 900);
			}


		}
		case 3: /* SkinChanger */
		{

		}
	}

	ImGui::End();

	ImGui::EndFrame();
	ImGui::Render();

	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}