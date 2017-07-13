#pragma once

bool bIsPlanted = false;
float flPlantedTime;
float explodesIn;
class C4Timer
{
public:
	static void PaintTraverse_Post()
	{
		if (!bIsPlanted)
			return;

		using namespace se;
		auto flCurrentTime = C_CSPlayer::GetLocalPlayer()->GetTickBase() * Interfaces::GlobalVars()->interval_per_tick;
		int width, height;
		Interfaces::MatSurface()->GetScreenSize(width, height);
		auto textSize1 = GetSize("EXPLODE IN", 16, FW_NORMAL);
		auto textSize2 = GetSize(std::to_string(static_cast<int>(explodesIn - flCurrentTime)).c_str(), 36, FW_HEAVY);
		auto hpos1 = static_cast<int>(height / 1.5f);
		auto hpos2 = hpos1 + 30 + textSize1.height + textSize2.height;
		auto wpos1 = width - 160;
		auto wpos2 = width;
		Interfaces::MatSurface()->DrawSetColor(0, 0, 0, 200);
		Interfaces::MatSurface()->DrawFilledRect(wpos1, hpos1, wpos2, hpos2);
		DrawString((wpos1 + wpos2) / 2, hpos1 + 10, 255, 255, 255, 255, "EXPLODE IN", 16, FW_NORMAL);
		DrawString((wpos1 + wpos2) / 2, hpos1 + 20 + textSize1.height / 2, 255, 255, 255, 255, std::to_string(static_cast<int>(explodesIn - flCurrentTime)).c_str(), 36, FW_HEAVY);
	}

	static void OnBombPlanted(C_CSPlayer* planter, int site)
	{
		auto pLocal = C_CSPlayer::GetLocalPlayer();
		explodesIn = pLocal->GetTickBase() * se::Interfaces::GlobalVars()->interval_per_tick + se::Interfaces::CVar()->FindVar("mp_c4timer")->GetInt() + 1;
		bIsPlanted = true;
	}

	static void OnBombExploded(C_CSPlayer* planter, int site)
	{
		bIsPlanted = false;
	}

	static void OnRoundStart(int timelimit, int fraglimit, const char* objective)
	{
		bIsPlanted = false;
	}

	static void OnLocalPlayerSpawn(bool inrestart)
	{
		bIsPlanted = false;
	}
private:
	static FontSize GetSize(const char* pszText, int tall, int weight)
	{
		if (pszText == nullptr)
			return FontSize{ -1, -1 };

		wchar_t szString[64];
		MultiByteToWideChar(CP_UTF8, 0, pszText, -1, szString, 64);

		auto font = se::Interfaces::MatSurface()->CreateFont();
		se::Interfaces::MatSurface()->SetFontGlyphSet(font, XorStr("Tahoma"), tall, weight, 0, 0, static_cast<int>(se::FontFlags::FONTFLAG_NONE));

		int iWidth, iHeight;
		se::Interfaces::MatSurface()->GetTextSize(font, szString, iWidth, iHeight);
		return FontSize{ iWidth, iHeight };
	}

	static FontSize DrawString(int x, int y, int r, int g, int b, int a, const char* pszText, int tall, int weight)
	{
		if (pszText == nullptr)
			return FontSize{ -1, -1 };

		wchar_t szString[64];
		MultiByteToWideChar(CP_UTF8, 0, pszText, -1, szString, 64);

		auto font = se::Interfaces::MatSurface()->CreateFont();
		se::Interfaces::MatSurface()->SetFontGlyphSet(font, XorStr("Tahoma"), tall, weight, 0, 0, static_cast<int>(se::FontFlags::FONTFLAG_NONE));

		int iWidth, iHeight;
		se::Interfaces::MatSurface()->GetTextSize(font, szString, iWidth, iHeight);
		se::Interfaces::MatSurface()->DrawSetTextFont(font);
		se::Interfaces::MatSurface()->DrawSetTextPos(x - iWidth / 2, y);
		se::Interfaces::MatSurface()->DrawSetTextColor(se::Color(r, g, b, a));
		se::Interfaces::MatSurface()->DrawPrintText(szString, wcslen(szString));
		return FontSize{ iWidth, iHeight };
	}
};