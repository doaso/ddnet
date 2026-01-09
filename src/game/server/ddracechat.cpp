/* (c) Shereef Marzouk. See "licence DDRace.txt" and the readme.txt in the root of the distribution for more information. */
#include "gamecontext.h"
#include "player.h"
#include "score.h"

#include <base/log.h>

#include <engine/shared/config.h>
#include <engine/shared/protocol.h>

#include <game/mapitems.h>
#include <game/server/entities/character.h>
#include <game/server/gamemodes/DDRace.h>
#include <game/server/teams.h>
#include <game/team_state.h>
#include <game/teamscore.h>
#include <game/version.h>

void CGameContext::Teleport(CCharacter *pChr, vec2 Pos)
{
	pChr->SetPosition(Pos);
	pChr->m_Pos = Pos;
	pChr->m_PrevPos = Pos;
}


void CGameContext::ConGiveGun(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

        if (pPlayer->m_AccountRole < 2) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно прав");
            return;
        }

        ConAddWeapon(pResult, pUserData);
}

void CGameContext::ConDropGun(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr) {
            return;
	}

        int ActiveWepapon = pChr->GetActiveWeapon();
        if (ActiveWepapon == WEAPON_HAMMER || ActiveWepapon == WEAPON_GUN) {
            return;
        }

	vec2 Pos = pChr->m_Pos;
        pSelf->CreateWeaponPickup(Pos, ActiveWepapon);
        pSelf->ModifyWeapons(pResult, pUserData, ActiveWepapon, true);
        pChr->SetActiveWeapon(WEAPON_HAMMER);
}

void CGameContext::ConEffect(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr) {
            return;
        }

        if (pPlayer->m_AccountRole < 2) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно прав");
            return;
        }

        pPlayer->m_Effect = pResult->GetInteger(0);
}


void CGameContext::ConInfhook(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

        if (pPlayer->m_AccountRole < 1) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно прав");
            return;
        }

	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr) {
            return;
        }

        pChr->SetEndlessHook(true);
}


void CGameContext::ConJetpackC(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

        if (pPlayer->m_AccountRole < 1) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно прав");
            return;
        }

	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr) {
            return;
        }

        pChr->SetJetpack(true);
}

void CGameContext::ConUnJetpackC(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

        if (pPlayer->m_AccountRole < 1) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно прав");
            return;
        }

	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr) {
            return;
        }

        pChr->SetJetpack(false);
}

void CGameContext::ConUnInfhook(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

        if (pPlayer->m_AccountRole < 1) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно прав");
            return;
        }

	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr) {
            return;
        }

        pChr->SetEndlessHook(false);
}

void CGameContext::ConInfjump(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

        if (pPlayer->m_AccountRole < 1) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно прав");
            return;
        }

	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr) {
            return;
        }

        pChr->SetEndlessJump(true);
}

void CGameContext::ConUnInfjump(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

        if (pPlayer->m_AccountRole < 1) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно прав");
            return;
        }

	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr) {
            return;
        }

        pChr->SetEndlessJump(false);
}

void CGameContext::ConLogin(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
        if (!pSelf) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

        if (pPlayer->m_IsLoginOrRegister == false) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "Вы уже авторизированы");
            return;
        }

        if (pPlayer->m_IsHaveAccount == false) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "Ваш аккаунт не зарегистрирован. Чтобы зарегистрировать его, используйте команду /register");
            return; 
        }

        if (str_comp(pPlayer->m_aAccountPassword, pResult->GetString(0)) != 0) {
            pPlayer->m_TryPassword--;
            if (pPlayer->m_TryPassword == 0) {
                pSelf->Server()->Kick(pPlayer->GetCid(), "Слишком много неверных попыток входа");
                return;
            }

            char aBuf[255];
            str_format(aBuf, sizeof(aBuf), "Неверный пароль. Осталось попыток: %d", pPlayer->m_TryPassword);
            pSelf->SendChatTarget(pPlayer->GetCid(), aBuf);
            return;
        }

	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr) {
            return;
        }

        pSelf->Score()->ChangeNetAdressAccount(pSelf->Server()->ClientName(pPlayer->GetCid()), pSelf->Server()->ClientAddrString(pPlayer->GetCid(), false));
        pChr->SetLiveFrozen(false);
        pPlayer->m_IsLoginOrRegister = false;

        char aBuf[255];
        str_format(aBuf, sizeof(aBuf), "На вашем балансе: %i пойнтов", pPlayer->m_AccountPoints);
        pSelf->SendChatTarget(pPlayer->GetCid(), aBuf);
}

void CGameContext::ConRegister(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
        if (!pSelf) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

        if (pPlayer->m_IsLoginOrRegister == false) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "Вы уже авторизированы");
            return;
        }

        if (pPlayer->m_IsHaveAccount == true) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "Ваш аккаунт зарегистрирован. Для входа в него воспользуйтесь командой /login");
            return; 
        }

        if (str_comp(pResult->GetString(0), pResult->GetString(1)) != 0) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "Пароли не совпадают");
            return;
        }

        pSelf->Score()->RegisterAccount(pSelf->Server()->ClientName(pPlayer->GetCid()), pResult->GetString(1));
        pSelf->Score()->ChangeNetAdressAccount(pSelf->Server()->ClientName(pPlayer->GetCid()), pSelf->Server()->ClientAddrString(pPlayer->GetCid(), false));
        pSelf->SendChatTarget(pPlayer->GetCid(), "Вы зарегистировались");


	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr) {
            return;
        }

        pChr->SetLiveFrozen(false);
        pPlayer->m_IsLoginOrRegister = false;
        pPlayer->m_IsHaveAccount = true;

        char aBuf[255];
        str_format(aBuf, sizeof(aBuf), "На вашем балансе: %i пойнтов", pPlayer->m_AccountPoints);
        pSelf->SendChatTarget(pPlayer->GetCid(), aBuf);
}


void CGameContext::ConPasswd(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
        if (!pSelf) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

        if (str_comp(pPlayer->m_aAccountPassword, pResult->GetString(0)) != 0) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "Текущий пароль введен неверно");
            return;
        }

        pSelf->Score()->ChangePasswordAccount(pSelf->Server()->ClientName(pPlayer->GetCid()), pResult->GetString(1));
        pSelf->SendChatTarget(pPlayer->GetCid(), "Вы сменили пароль");
}

void CGameContext::ConShop(IConsole::IResult *pResult, void *pUserData)
{
    CGameContext *pSelf = (CGameContext *)pUserData;
    if (!pSelf)
        return;

    CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
    if (!pPlayer)
        return;

    if (pResult->NumArguments() == 1)
    {
        int ItemID = pResult->GetInteger(0);
        int Price = 0;
        const char *ItemName = "";

        switch (ItemID) {
        case 1: Price = 100;  ItemName = "Нинзя"; break;
        case 2: Price = 250;  ItemName = "Эффект Хаммер-дробовик"; break;
        case 3: Price = 250;  ItemName = "Эффект Хаммер-лазер"; break;
        case 4: Price = 500;  ItemName = "Эффект Хеппи-бердей"; break;
        case 5: Price = 750;  ItemName = "Эффект Админ ауры"; break;
        case 6: Price = 500;  ItemName = "Эффект Огненной ауры"; break;
        case 7: Price = 1500;  ItemName = "Эффект Смерти"; break;
        default:
            pSelf->SendChatTarget(pPlayer->GetCid(), "Неверный номер предмета");
            return;
        }

        if (pPlayer->m_AccountPoints < Price) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно пойнтов");
            return;
        }

        pPlayer->m_AccountPoints -= Price;
        pSelf->Score()->ChangePointsAccount(pSelf->Server()->ClientName(pPlayer->GetCid()), pPlayer->m_AccountPoints);

        switch (ItemID) {
        case 1: pSelf->ModifyWeapons(pResult, pUserData, 5, false); break;
        case 2: pPlayer->m_Effect = 5; break;
        case 3: pPlayer->m_Effect = 6; break;
        case 4: pPlayer->m_Effect = 3; break;
        case 5: pPlayer->m_Effect = 4; break;
        case 6: pPlayer->m_Effect = 2; break;
        case 7: pPlayer->m_Effect = 1; break;
        }


        char aBuf[128];
        str_format(aBuf, sizeof(aBuf), "Вы купили %s за %d пойнтов", ItemName, Price);
        pSelf->SendChatTarget(pPlayer->GetCid(), aBuf);
    } else {
        char aBuf[1024];
        char aShopText[1024] = "";
        str_append(aShopText, "Магазин:\n", sizeof(aShopText));

        struct ItemInfo { int ID; const char *Name; int Price; };
        ItemInfo Items[] = {
            {1, "Нинзя", 100},
            {2, "Эффект Хаммер-дробовик", 250},
            {3, "Эффект Хаммер-лазер", 250},
            {4, "Эффект Хеппи-бердей", 500},
            {5, "Эффект Админ ауры", 750},
            {6, "Эффект Огненной ауры", 500},
            {7, "Эффект Смерти", 1500},
        };

        for (const auto &It : Items) {
            char aLine[256];
            str_format(aLine, sizeof(aLine), "/shop %d - %s %d пойнтов\n", It.ID, It.Name, It.Price);
            str_append(aShopText, aLine, sizeof(aShopText));
        }

        str_format(aBuf, sizeof(aBuf), "%s", aShopText);
        pSelf->SendMotd(pResult->m_ClientId, aBuf);
    }
}

void CGameContext::ConRules(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
        if (!pSelf) {
            return;
        }

        pSelf->SendMotd(pResult->m_ClientId, R"(Правила:
Читы - Кик
Оскорбления - Мут/Кик
        )");
}


void CGameContext::ConCmdList(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
        if (!pSelf) {
            return;
        }

            pSelf->SendMotd(pResult->m_ClientId, R"(Команды:
/cmdlist - Команды
/acmdlist - Команды администрации
/login - Авторизация
/register - Регистрация
/passwd - Поменять пароль
/rules - Правила
/points - Баланс
/pay - Передать пойнты
/shop - Магазин
/drop - Выбросить оружие
/orel - Регистрация игры в Орел и Решка за сторону Орел
/reshka - Регистрация игры в Орел и Решка за сторону Решка
            )");
}

void CGameContext::ConACmdList(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
        if (!pSelf) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

        if (pPlayer->m_AccountRole < 1) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно прав");
            return;
        }

        if (pResult->GetInteger(0) == 1) {
             pSelf->SendMotd(pResult->m_ClientId, R"(Команды для 1-го уровня доступа:
/infjump - Включить бесконечные прыжки
/uninfjump - Выключить бесконечные прыжки
/infhook - Включить бесконечный хук
/uninfhook - Выключить бесконечный хук
/jetpack - Включить джетпак
/unjetpack - Выключить джетпак
            )");       
        } else if (pResult->GetInteger(0) == 2) {
             pSelf->SendMotd(pResult->m_ClientId, R"(Команды для 2-го уровня доступа:
/effect - Включить эффекты
/givegun - Выдать оружие
/solo - Включить соло
/unsolo - Выключить соло
            )");       
        } else if (pResult->GetInteger(0) == 3) {
             pSelf->SendMotd(pResult->m_ClientId, R"(Команды для 3-го уровня доступа:
/az - Телепортироваться в админ зону
/goto - Телепортироваться к игроку
/gethere - Телепортировать игрока к себе
            )");       
        } else if (pResult->GetInteger(0) == 4) {
             pSelf->SendMotd(pResult->m_ClientId, R"(Команды для 4-го уровня доступа:
/broadcast - Вывести сообщение на весь сервер
/vanis - Включить ваниш
/unvanis - Выключить ваниш
/freeze - Заморозить игрока
/unfreeze - Разморозить игрока
/kill - Убить игрока
/god - Включить бессмертие
/ungod - Выключить бессмертие
            )");
        } else if (pResult->GetInteger(0) == 5) {
             pSelf->SendMotd(pResult->m_ClientId, R"(Команды для 5-го уровня доступа:
/stats - Посмотреть статистику игрока
/tpspec - Включить телепорт в режиме наблюдателя 
/untpspec - Выключить телепорт в режиме наблюдателя
/mute - Заблокировать чат игроку
/unmute - Разаблокировать чат игроку
/kick - Кикнуть игрока
            )");
        } else if (pResult->GetInteger(0) == 6) {
             pSelf->SendMotd(pResult->m_ClientId, R"(Команды для 6-го уровня доступа:
/ban - Заблокировать игрока
/setpoints - Установить пойнты
/setlevel - Установить постоянный уровень доступа
/settemplevel - Установить временный уровень доступа
            )");
        }
}

void CGameContext::ConBroadcastC(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

        if (pPlayer->m_AccountRole < 4) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно прав");
            return;
        }

        pSelf->SendBroadcast(pResult->GetString(0), -1);
}

void CGameContext::ConSetLevel(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

        if (pPlayer->m_AccountRole < 6) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно прав");
            return;
        }

        if (pResult->GetInteger(1) < 0) {
            return;
        }

        int ClientId = 0;
        for(ClientId = 0; ClientId < MAX_CLIENTS; ClientId = ClientId + 1) {
            if(str_comp(pResult->GetString(0), pSelf->Server()->ClientName(ClientId)) == 0)
                break;
        }

        char aBuf[255];

        if(ClientId == MAX_CLIENTS) {
            str_format(aBuf, sizeof(aBuf), "Игрок %s не найден", pResult->GetString(0));
            pSelf->SendChatTarget(pPlayer->GetCid(), aBuf);
            return;
        }

        CPlayer *pPlayerTarget = pSelf->m_apPlayers[ClientId];
        if(!pPlayerTarget)
            return;

        if (pPlayerTarget->m_IsHaveAccount == false) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "Игрок не зарегистрирован");
            return;
        }

        int AccountRole = pResult->GetInteger(1);
        pPlayerTarget->m_AccountRole = AccountRole;
        pSelf->Score()->ChangeRoleAccount(pResult->GetString(0), AccountRole);

        str_format(aBuf, sizeof(aBuf), "Администратор %s установил %s постоянный %i уровень доступа",
                   pSelf->Server()->ClientName(pResult->m_ClientId), pResult->GetString(0), AccountRole);
        pSelf->SendChat(-1, TEAM_ALL, aBuf);

        pSelf->SendChatTarget(pPlayerTarget->GetCid(), "Используйте /acmdlist что-бы посмотреть ваши команды");
}


void CGameContext::ConSetPoints(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

        if (pPlayer->m_AccountRole < 6) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно прав");
            return;
        }

        if (pResult->GetInteger(1) < 0) {
            return;
        }

        int ClientId = 0;
        for(ClientId = 0; ClientId < MAX_CLIENTS; ClientId = ClientId + 1) {
            if(str_comp(pResult->GetString(0), pSelf->Server()->ClientName(ClientId)) == 0)
                break;
        }

        char aBuf[255];

        if(ClientId == MAX_CLIENTS) {
            str_format(aBuf, sizeof(aBuf), "Игрок %s не найден", pResult->GetString(0));
            pSelf->SendChatTarget(pPlayer->GetCid(), aBuf);
            return;
        }

        CPlayer *pPlayerTarget = pSelf->m_apPlayers[ClientId];
        if(!pPlayerTarget)
            return;

        if (pPlayerTarget->m_IsHaveAccount == false) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "Игрок не зарегистрирован");
            return;
        }

        int AccountPoints = pResult->GetInteger(1);
        pPlayerTarget->m_AccountPoints = AccountPoints;
        pSelf->Score()->ChangePointsAccount(pResult->GetString(0), AccountPoints);

        str_format(aBuf, sizeof(aBuf), "Администратор %s установил %s %i пойнтов",
                   pSelf->Server()->ClientName(pResult->m_ClientId), pResult->GetString(0), AccountPoints);
        pSelf->SendChat(-1, TEAM_ALL, aBuf);
}

void CGameContext::ConSetTempLevel(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

        if (pPlayer->m_AccountRole < 6) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно прав");
            return;
        }

        if (pResult->GetInteger(1) < 0) {
            return;
        }

        int ClientId = 0;
        for(ClientId = 0; ClientId < MAX_CLIENTS; ClientId = ClientId + 1) {
            if(str_comp(pResult->GetString(0), pSelf->Server()->ClientName(ClientId)) == 0)
                break;
        }

        char aBuf[255];

        if(ClientId == MAX_CLIENTS) {
            str_format(aBuf, sizeof(aBuf), "Игрок %s не найден", pResult->GetString(0));
            pSelf->SendChatTarget(pPlayer->GetCid(), aBuf);
            return;
        }

        CPlayer *pPlayerTarget = pSelf->m_apPlayers[ClientId];
        if(!pPlayerTarget)
            return;

        int AccountRole = pResult->GetInteger(1);
        pPlayerTarget->m_AccountRole = AccountRole;
        str_format(aBuf, sizeof(aBuf), "Администратор %s установил %s временный %i уровень доступа",
                   pSelf->Server()->ClientName(pResult->m_ClientId), pResult->GetString(0), AccountRole);
        pSelf->SendChat(-1, TEAM_ALL, aBuf);

        pSelf->SendChatTarget(pPlayerTarget->GetCid(), "Используйте /acmdlist что-бы посмотреть ваши команды");
}

void CGameContext::ConUnMuteC(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

        if (pPlayer->m_AccountRole < 5) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно прав");
            return;
        }

        int ClientId = 0;
        for(ClientId = 0; ClientId < MAX_CLIENTS; ClientId = ClientId + 1) {
            if(str_comp(pResult->GetString(0), pSelf->Server()->ClientName(ClientId)) == 0)
                break;
        }

        char aBuf[255];

        if(ClientId == MAX_CLIENTS) {
            str_format(aBuf, sizeof(aBuf), "Игрок %s не найден", pResult->GetString(0));
            pSelf->SendChatTarget(pPlayer->GetCid(), aBuf);
            return;
        }

        CPlayer *pPlayerTarget = pSelf->m_apPlayers[ClientId];
        if(!pPlayerTarget)
            return;

        const NETADDR *pAddr = pSelf->Server()->ClientAddr(ClientId);
        if(!pAddr)
        {
            pSelf->SendChatTarget(pPlayer->GetCid(), "Не удалось получить адрес игрока");
            return;
        }

        pSelf->m_Mutes.UnmuteAddr(pAddr);

        str_format(aBuf, sizeof(aBuf), "Администратор %s разблокировал чат %s Причина: %s",
        pSelf->Server()->ClientName(pResult->m_ClientId), pResult->GetString(0), pResult->GetString(1));
        pSelf->SendChat(-1, TEAM_ALL, aBuf);
}

void CGameContext::ConMuteC(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

        if (str_comp(pSelf->Server()->ClientName(pPlayer->GetCid()), pResult->GetString(0)) == 0) {
            return;            
        }

        if (pPlayer->m_AccountRole < 5) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно прав");
            return;
        }

        int ClientId = 0;
        for(ClientId = 0; ClientId < MAX_CLIENTS; ClientId = ClientId + 1) {
            if(str_comp(pResult->GetString(0), pSelf->Server()->ClientName(ClientId)) == 0)
                break;
        }

        char aBuf[255];

        if(ClientId == MAX_CLIENTS) {
            str_format(aBuf, sizeof(aBuf), "Игрок %s не найден", pResult->GetString(0));
            pSelf->SendChatTarget(pPlayer->GetCid(), aBuf);
            return;
        }

        CPlayer *pPlayerTarget = pSelf->m_apPlayers[ClientId];
        if(!pPlayerTarget)
            return;

        str_format(aBuf, sizeof(aBuf), "Администратор %s заблокировал чат %s на %i секунд Причина: %s",
                   pSelf->Server()->ClientName(pResult->m_ClientId), pResult->GetString(0), pResult->GetInteger(1), pResult->GetString(2));
        pSelf->SendChat(-1, TEAM_ALL, aBuf);
        pSelf->MuteWithMessage(pSelf->Server()->ClientAddr(ClientId), pResult->GetInteger(1), pResult->GetString(2), pResult->GetString(1));
}

void CGameContext::ConKickC(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

        if (str_comp(pSelf->Server()->ClientName(pPlayer->GetCid()), pResult->GetString(0)) == 0) {
            return;            
        }

        if (pPlayer->m_AccountRole < 5) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно прав");
            return;
        }

        int ClientId = 0;
        for(ClientId = 0; ClientId < MAX_CLIENTS; ClientId = ClientId + 1) {
            if(str_comp(pResult->GetString(0), pSelf->Server()->ClientName(ClientId)) == 0)
                break;
        }

        char aBuf[255];

        if(ClientId == MAX_CLIENTS) {
            str_format(aBuf, sizeof(aBuf), "Игрок %s не найден", pResult->GetString(0));
            pSelf->SendChatTarget(pPlayer->GetCid(), aBuf);
            return;
        }

        CPlayer *pPlayerTarget = pSelf->m_apPlayers[ClientId];
        if(!pPlayerTarget)
            return;

        str_format(aBuf, sizeof(aBuf), "Администратор %s кикнул %s Причина: %s",
                   pSelf->Server()->ClientName(pResult->m_ClientId), pResult->GetString(0), pResult->GetString(1));
        pSelf->SendChat(-1, TEAM_ALL, aBuf);
}


void CGameContext::ConBanC(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

        if (str_comp(pSelf->Server()->ClientName(pPlayer->GetCid()), pResult->GetString(0)) == 0) {
            return;            
        }

        if (pPlayer->m_AccountRole < 6) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно прав");
            return;
        }

        int ClientId = 0;
        for(ClientId = 0; ClientId < MAX_CLIENTS; ClientId = ClientId + 1) {
            if(str_comp(pResult->GetString(0), pSelf->Server()->ClientName(ClientId)) == 0)
                break;
        }

        char aBuf[255];

        if(ClientId == MAX_CLIENTS) {
            str_format(aBuf, sizeof(aBuf), "Игрок %s не найден", pResult->GetString(0));
            pSelf->SendChatTarget(pPlayer->GetCid(), aBuf);
            return;
        }

        CPlayer *pPlayerTarget = pSelf->m_apPlayers[ClientId];
        if(!pPlayerTarget)
            return;

        str_format(aBuf, sizeof(aBuf), "Администратор %s заблокировал %s на %i секунд Причина: %s",
                   pSelf->Server()->ClientName(pResult->m_ClientId), pResult->GetString(0), pResult->GetInteger(1), pResult->GetString(2));
        pSelf->SendChat(-1, TEAM_ALL, aBuf);
        pSelf->Server()->Ban(ClientId, pResult->GetInteger(1), pResult->GetString(2), false);
}

void CGameContext::ConGetHere(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

        if (str_comp(pSelf->Server()->ClientName(pPlayer->GetCid()), pResult->GetString(0)) == 0) {
            return;            
        }

        if (pPlayer->m_AccountRole < 3) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно прав");
            return;
        }

	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr) {
            return;
        }

        int ClientId = 0;
        for(ClientId = 0; ClientId < MAX_CLIENTS; ClientId = ClientId + 1) {
            if(str_comp(pResult->GetString(0), pSelf->Server()->ClientName(ClientId)) == 0)
                break;
        }

        char aBuf[255];

        if(ClientId == MAX_CLIENTS) {
            str_format(aBuf, sizeof(aBuf), "Игрок %s не найден", pResult->GetString(0));
            pSelf->SendChatTarget(pPlayer->GetCid(), aBuf);
            return;
        }

        CPlayer *pPlayerTarget = pSelf->m_apPlayers[ClientId];
        if(!pPlayerTarget)
            return;

        CCharacter *pChrTarget = pPlayerTarget->GetCharacter();
        if(!pChrTarget)
            return;

        pSelf->CreatePlayerSpawn(pChr->m_Pos);
        pSelf->CreatePlayerSpawn(pChrTarget->m_Pos);
        pSelf->Teleport(pChrTarget, pChr->m_Pos);
	pChrTarget->ResetJumps();
	pChrTarget->UnFreeze();
	pChrTarget->ResetVelocity();

        str_format(aBuf, sizeof(aBuf), "Вы были телепортированы администратором %s", pSelf->Server()->ClientName(pPlayer->GetCid()));
        pSelf->SendChatTarget(pPlayerTarget->GetCid(), aBuf);
        pSelf->CreateSound(pChrTarget->m_Pos, SOUND_PLAYER_DIE);

	// pPlayerTarget->m_LastTeleTee.Save(pChrTarget);
}


void CGameContext::ConKillC(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

        if (pPlayer->m_AccountRole < 4) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно прав");
            return;
        }

	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr) {
            return;
        }

        int ClientId = 0;
        for(ClientId = 0; ClientId < MAX_CLIENTS; ClientId = ClientId + 1) {
            if(str_comp(pResult->GetString(0), pSelf->Server()->ClientName(ClientId)) == 0)
                break;
        }

        char aBuf[255];

        if(ClientId == MAX_CLIENTS) {
            str_format(aBuf, sizeof(aBuf), "Игрок %s не найден", pResult->GetString(0));
            pSelf->SendChatTarget(pPlayer->GetCid(), aBuf);
            return;
        }

        CPlayer *pPlayerTarget = pSelf->m_apPlayers[ClientId];
        if(!pPlayerTarget)
            return;

        CCharacter *pChrTarget = pPlayerTarget->GetCharacter();
        if(!pChrTarget)
            return;

        str_format(aBuf, sizeof(aBuf), "Администратор %s убил %s",
                   pSelf->Server()->ClientName(pResult->m_ClientId), pResult->GetString(0));
        pSelf->SendChat(-1, TEAM_ALL, aBuf);

        pChrTarget->Die(ClientId, WEAPON_SELF, false);
        pChrTarget->UnFreeze();
}

void CGameContext::ConUnFreeze(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

        if (pPlayer->m_AccountRole < 4) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно прав");
            return;
        }

	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr) {
            return;
        }

        int ClientId = 0;
        for(ClientId = 0; ClientId < MAX_CLIENTS; ClientId = ClientId + 1) {
            if(str_comp(pResult->GetString(0), pSelf->Server()->ClientName(ClientId)) == 0)
                break;
        }

        char aBuf[255];

        if(ClientId == MAX_CLIENTS) {
            str_format(aBuf, sizeof(aBuf), "Игрок %s не найден", pResult->GetString(0));
            pSelf->SendChatTarget(pPlayer->GetCid(), aBuf);
            return;
        }

        CPlayer *pPlayerTarget = pSelf->m_apPlayers[ClientId];
        if(!pPlayerTarget)
            return;

        CCharacter *pChrTarget = pPlayerTarget->GetCharacter();
        if(!pChrTarget)
            return;

        str_format(aBuf, sizeof(aBuf), "Администратор %s разморозил %s",
                   pSelf->Server()->ClientName(pResult->m_ClientId), pResult->GetString(0));
        pSelf->SendChat(-1, TEAM_ALL, aBuf);

        pChrTarget->SetDeepFrozen(false);
        pChrTarget->UnFreeze();
}

void CGameContext::ConFreeze(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

        if (pPlayer->m_AccountRole < 4) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно прав");
            return;
        }

	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr) {
            return;
        }

        int ClientId = 0;
        for(ClientId = 0; ClientId < MAX_CLIENTS; ClientId = ClientId + 1) {
            if(str_comp(pResult->GetString(0), pSelf->Server()->ClientName(ClientId)) == 0)
                break;
        }

        char aBuf[255];

        if(ClientId == MAX_CLIENTS) {
            str_format(aBuf, sizeof(aBuf), "Игрок %s не найден", pResult->GetString(0));
            pSelf->SendChatTarget(pPlayer->GetCid(), aBuf);
            return;
        }

        CPlayer *pPlayerTarget = pSelf->m_apPlayers[ClientId];
        if(!pPlayerTarget)
            return;

        CCharacter *pChrTarget = pPlayerTarget->GetCharacter();
        if(!pChrTarget)
            return;

        str_format(aBuf, sizeof(aBuf), "Администратор %s заморозил %s",
                   pSelf->Server()->ClientName(pResult->m_ClientId), pResult->GetString(0));
        pSelf->SendChat(-1, TEAM_ALL, aBuf);
        pChrTarget->SetDeepFrozen(true);
}

void CGameContext::ConSoloC(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

        if (pPlayer->m_AccountRole < 2) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно прав");
            return;
        }

	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr) {
            return;
        }

        pChr->SetSolo(true);
}

void CGameContext::ConUnSoloC(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

        if (pPlayer->m_AccountRole < 2) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно прав");
            return;
        }

	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr) {
            return;
        }

        pChr->SetSolo(false);
}

void CGameContext::ConSuperC(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

        if (pPlayer->m_AccountRole < 4) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно прав");
            return;
        }

	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr) {
            return;
        }

        pChr->SetSuper(true);
}

void CGameContext::ConUnSuperC(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

        if (pPlayer->m_AccountRole < 4) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно прав");
            return;
        }

	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr) {
            return;
        }

        pChr->SetSuper(false);
}

void CGameContext::ConAZ(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

        if (pPlayer->m_AccountRole < 3) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно прав");
            return;
        }

	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr) {
            return;
        }

        vec2 AdminZonaPos = vec2(240.0f, 144.0f);
        pChr->SetPosition(AdminZonaPos);
}

void CGameContext::ConTpSpec(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

        if (pPlayer->m_AccountRole < 5) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно прав");
            return;
        }

        pPlayer->m_IsTpSpec = true;
}

void CGameContext::ConUnTpSpec(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

        if (pPlayer->m_AccountRole < 5) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно прав");
            return;
        }

        pPlayer->m_IsTpSpec = false;
}

void CGameContext::ConGoto(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

        if (str_comp(pSelf->Server()->ClientName(pPlayer->GetCid()), pResult->GetString(0)) == 0) {
            return;            
        }

        if (pPlayer->m_AccountRole < 3) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно прав");
            return;
        }

	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr) {
            return;
        }

        int ClientId = 0;
        for(ClientId = 0; ClientId < MAX_CLIENTS; ClientId = ClientId + 1) {
            if(str_comp(pResult->GetString(0), pSelf->Server()->ClientName(ClientId)) == 0)
                break;
        }

        char aBuf[255];

        if(ClientId == MAX_CLIENTS) {
            str_format(aBuf, sizeof(aBuf), "Игрок %s не найден", pResult->GetString(0));
            pSelf->SendChatTarget(pPlayer->GetCid(), aBuf);
            return;
        }

        CPlayer *pPlayerTarget = pSelf->m_apPlayers[ClientId];
        if(!pPlayerTarget)
            return;

        CCharacter *pChrTarget = pPlayerTarget->GetCharacter();
        if(!pChrTarget)
            return;

        pSelf->CreatePlayerSpawn(pChr->m_Pos);
        pSelf->CreatePlayerSpawn(pChrTarget->m_Pos);
        pSelf->Teleport(pChr, pChrTarget->m_Pos);
	pChrTarget->ResetJumps();
	pChrTarget->UnFreeze();
	pChrTarget->ResetVelocity();

        str_format(aBuf, sizeof(aBuf), "К вам телепортировался администратор %s", pSelf->Server()->ClientName(pPlayer->GetCid()));
        pSelf->SendChatTarget(pPlayerTarget->GetCid(), aBuf);
        pSelf->CreateSound(pChr->m_Pos, SOUND_PLAYER_DIE);
	// pPlayer->m_LastTeleTee.Save(pChrTarget);
}


static void ToggleSpecPause(IConsole::IResult *pResult, void *pUserData, int PauseType)
{
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CGameContext *pSelf = (CGameContext *)pUserData;
	IServer *pServ = pSelf->Server();
	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

	int PauseState = pPlayer->IsPaused();
	if(PauseState > 0)
	{
		char aBuf[128];
		str_format(aBuf, sizeof(aBuf), "You are force-paused for %d seconds.", (PauseState - pServ->Tick()) / pServ->TickSpeed());
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", aBuf);
	}
	else if(pResult->NumArguments() > 0)
	{
		if(-PauseState == PauseType && pPlayer->SpectatorId() != pResult->m_ClientId && pServ->ClientIngame(pPlayer->SpectatorId()) && !str_comp(pServ->ClientName(pPlayer->SpectatorId()), pResult->GetString(0)))
		{
			pPlayer->Pause(CPlayer::PAUSE_NONE, false);
		}
		else
		{
			pPlayer->Pause(PauseType, false);
			pPlayer->SpectatePlayerName(pResult->GetString(0));
		}
	}
	else if(-PauseState != CPlayer::PAUSE_NONE && PauseType != CPlayer::PAUSE_NONE)
	{
		pPlayer->Pause(CPlayer::PAUSE_NONE, false);
	}
	else if(-PauseState != PauseType)
	{
		pPlayer->Pause(PauseType, false);
	}
}

static void ToggleSpecPauseVoted(IConsole::IResult *pResult, void *pUserData, int PauseType)
{
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CGameContext *pSelf = (CGameContext *)pUserData;
	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

	int PauseState = pPlayer->IsPaused();
	if(PauseState > 0)
	{
		IServer *pServ = pSelf->Server();
		char aBuf[128];
		str_format(aBuf, sizeof(aBuf), "You are force-paused for %d seconds.", (PauseState - pServ->Tick()) / pServ->TickSpeed());
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", aBuf);
		return;
	}

	bool IsPlayerBeingVoted = pSelf->m_VoteCloseTime &&
				  (pSelf->IsKickVote() || pSelf->IsSpecVote()) &&
				  pResult->m_ClientId != pSelf->m_VoteVictim;
	if((!IsPlayerBeingVoted && -PauseState == PauseType) ||
		(IsPlayerBeingVoted && PauseState && pPlayer->SpectatorId() == pSelf->m_VoteVictim))
	{
		pPlayer->Pause(CPlayer::PAUSE_NONE, false);
	}
	else
	{
		pPlayer->Pause(PauseType, false);
		if(IsPlayerBeingVoted)
			pPlayer->SetSpectatorId(pSelf->m_VoteVictim);
	}
}

void CGameContext::ConToggleSpec(IConsole::IResult *pResult, void *pUserData)
{
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CGameContext *pSelf = (CGameContext *)pUserData;
	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

	int PauseType = g_Config.m_SvPauseable ? CPlayer::PAUSE_SPEC : CPlayer::PAUSE_PAUSED;

	if(pPlayer->GetCharacter())
	{
		CGameTeams &Teams = pSelf->m_pController->Teams();
		if(Teams.IsPractice(Teams.m_Core.Team(pResult->m_ClientId)))
			PauseType = CPlayer::PAUSE_SPEC;


                if (pPlayer->m_IsTpSpec == true) {
                    CCharacter *pChr = pPlayer->GetCharacter();
                    if(!pChr) {
                        return;
                    }

                    vec2 TargetPos = vec2(pPlayer->m_pLastTarget->m_TargetX, pPlayer->m_pLastTarget->m_TargetY);
                    pChr->UnFreeze();
                    pSelf->Teleport(pChr, TargetPos);
                    pSelf->CreatePlayerSpawn(TargetPos);
                    pSelf->CreateSound(TargetPos, SOUND_PLAYER_DIE);
                }
	}

	ToggleSpecPause(pResult, pUserData, PauseType);
}

void CGameContext::ConToggleSpecVoted(IConsole::IResult *pResult, void *pUserData)
{
	ToggleSpecPauseVoted(pResult, pUserData, g_Config.m_SvPauseable ? CPlayer::PAUSE_SPEC : CPlayer::PAUSE_PAUSED);
}

void CGameContext::ConTogglePause(IConsole::IResult *pResult, void *pUserData)
{
	ToggleSpecPause(pResult, pUserData, CPlayer::PAUSE_PAUSED);
}

void CGameContext::ConTogglePauseVoted(IConsole::IResult *pResult, void *pUserData)
{
	ToggleSpecPauseVoted(pResult, pUserData, CPlayer::PAUSE_PAUSED);
}

void CGameContext::ConTeamTop5(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	if(g_Config.m_SvHideScore)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
			"Showing the team top 5 is not allowed on this server.");
		return;
	}

	if(pResult->NumArguments() == 0)
	{
		pSelf->Score()->ShowTeamTop5(pResult->m_ClientId, 1);
	}
	else if(pResult->NumArguments() == 1)
	{
		if(pResult->GetInteger(0) != 0)
		{
			pSelf->Score()->ShowTeamTop5(pResult->m_ClientId, pResult->GetInteger(0));
		}
		else
		{
			const char *pRequestedName = (str_comp_nocase(pResult->GetString(0), "me") == 0) ?
							     pSelf->Server()->ClientName(pResult->m_ClientId) :
							     pResult->GetString(0);
			pSelf->Score()->ShowPlayerTeamTop5(pResult->m_ClientId, pRequestedName, 0);
		}
	}
	else if(pResult->NumArguments() == 2 && pResult->GetInteger(1) != 0)
	{
		const char *pRequestedName = (str_comp_nocase(pResult->GetString(0), "me") == 0) ?
						     pSelf->Server()->ClientName(pResult->m_ClientId) :
						     pResult->GetString(0);
		pSelf->Score()->ShowPlayerTeamTop5(pResult->m_ClientId, pRequestedName, pResult->GetInteger(1));
	}
	else
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "/top5team needs 0, 1 or 2 parameter. 1. = name, 2. = start number");
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "Example: /top5team, /top5team me, /top5team Hans, /top5team \"Papa Smurf\" 5");
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "Bad: /top5team Papa Smurf 5 # Good: /top5team \"Papa Smurf\" 5 ");
	}
}

void CGameContext::ConTop(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	if(g_Config.m_SvHideScore)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
			"Showing the top is not allowed on this server.");
		return;
	}

	if(pResult->NumArguments() > 0)
		pSelf->Score()->ShowTop(pResult->m_ClientId, pResult->GetInteger(0));
	else
		pSelf->Score()->ShowTop(pResult->m_ClientId);
}

void CGameContext::ConTimes(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	if(pResult->NumArguments() == 0)
	{
		pSelf->Score()->ShowTimes(pResult->m_ClientId, 1);
	}
	else if(pResult->NumArguments() == 1)
	{
		if(pResult->GetInteger(0) != 0)
		{
			pSelf->Score()->ShowTimes(pResult->m_ClientId, pResult->GetInteger(0));
		}
		else
		{
			const char *pRequestedName = (str_comp_nocase(pResult->GetString(0), "me") == 0) ?
							     pSelf->Server()->ClientName(pResult->m_ClientId) :
							     pResult->GetString(0);
			pSelf->Score()->ShowTimes(pResult->m_ClientId, pRequestedName, pResult->GetInteger(1));
		}
	}
	else if(pResult->NumArguments() == 2 && pResult->GetInteger(1) != 0)
	{
		const char *pRequestedName = (str_comp_nocase(pResult->GetString(0), "me") == 0) ?
						     pSelf->Server()->ClientName(pResult->m_ClientId) :
						     pResult->GetString(0);
		pSelf->Score()->ShowTimes(pResult->m_ClientId, pRequestedName, pResult->GetInteger(1));
	}
	else
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "/times needs 0, 1 or 2 parameter. 1. = name, 2. = start number");
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "Example: /times, /times me, /times Hans, /times \"Papa Smurf\" 5");
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "Bad: /times Papa Smurf 5 # Good: /times \"Papa Smurf\" 5 ");
	}
}

void CGameContext::ConDND(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

	pPlayer->m_DND = pResult->NumArguments() == 0 ? !pPlayer->m_DND : pResult->GetInteger(0);
	pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", pPlayer->m_DND ? "You will not receive any further global chat and server messages" : "You will receive global chat and server messages");
}

void CGameContext::ConMap(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	if(g_Config.m_SvMapVote == 0)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
			"/map is disabled");
		return;
	}

	if(pResult->NumArguments() <= 0)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "Example: /map adr3 to call vote for Adrenaline 3. This means that the map name must start with 'a' and contain the characters 'd', 'r' and '3' in that order");
		return;
	}

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

	if(pSelf->RateLimitPlayerVote(pResult->m_ClientId) || pSelf->RateLimitPlayerMapVote(pResult->m_ClientId))
		return;

	pSelf->Score()->MapVote(pResult->m_ClientId, pResult->GetString(0));
}

void CGameContext::ConTimeout(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

	const char *pTimeout = pResult->NumArguments() > 0 ? pResult->GetString(0) : pPlayer->m_aTimeoutCode;

	if(!pSelf->Server()->IsSixup(pResult->m_ClientId))
	{
		for(int i = 0; i < pSelf->Server()->MaxClients(); i++)
		{
			if(i == pResult->m_ClientId)
				continue;
			if(!pSelf->m_apPlayers[i])
				continue;
			if(str_comp(pSelf->m_apPlayers[i]->m_aTimeoutCode, pTimeout))
				continue;
			if(pSelf->Server()->SetTimedOut(i, pResult->m_ClientId))
			{
				if(pSelf->m_apPlayers[i]->GetCharacter())
					pSelf->SendTuningParams(i, pSelf->m_apPlayers[i]->GetCharacter()->m_TuneZone);
				return;
			}
		}
	}
	else
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
			"Your timeout code has been set. 0.7 clients can not reclaim their tees on timeout; however, a 0.6 client can claim your tee ");
	}

	pSelf->Server()->SetTimeoutProtected(pResult->m_ClientId);
	str_copy(pPlayer->m_aTimeoutCode, pResult->GetString(0), sizeof(pPlayer->m_aTimeoutCode));
}

void CGameContext::ConTeamRank(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	if(pResult->NumArguments() > 0)
	{
		if(!g_Config.m_SvHideScore)
			pSelf->Score()->ShowTeamRank(pResult->m_ClientId, pResult->GetString(0));
		else
			pSelf->Console()->Print(
				IConsole::OUTPUT_LEVEL_STANDARD,
				"chatresp",
				"Showing the team rank of other players is not allowed on this server.");
	}
	else
		pSelf->Score()->ShowTeamRank(pResult->m_ClientId,
			pSelf->Server()->ClientName(pResult->m_ClientId));
}

void CGameContext::ConRank(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	if(pResult->NumArguments() > 0)
	{
		if(!g_Config.m_SvHideScore)
			pSelf->Score()->ShowRank(pResult->m_ClientId, pResult->GetString(0));
		else
			pSelf->Console()->Print(
				IConsole::OUTPUT_LEVEL_STANDARD,
				"chatresp",
				"Showing the rank of other players is not allowed on this server.");
	}
	else
		pSelf->Score()->ShowRank(pResult->m_ClientId,
			pSelf->Server()->ClientName(pResult->m_ClientId));
}

void CGameContext::ConLock(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	if(g_Config.m_SvTeam == SV_TEAM_FORBIDDEN || g_Config.m_SvTeam == SV_TEAM_FORCED_SOLO)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
			"Teams are disabled");
		return;
	}

	int Team = pSelf->GetDDRaceTeam(pResult->m_ClientId);

	bool Lock = pSelf->m_pController->Teams().TeamLocked(Team);

	if(pResult->NumArguments() > 0)
		Lock = !pResult->GetInteger(0);

	if(Team <= TEAM_FLOCK || Team >= TEAM_SUPER)
	{
		pSelf->Console()->Print(
			IConsole::OUTPUT_LEVEL_STANDARD,
			"chatresp",
			"This team can't be locked");
		return;
	}

	if(pSelf->ProcessSpamProtection(pResult->m_ClientId, false))
		return;

	char aBuf[512];
	if(Lock)
	{
		pSelf->UnlockTeam(pResult->m_ClientId, Team);
	}
	else
	{
		pSelf->m_pController->Teams().SetTeamLock(Team, true);

		if(pSelf->m_pController->Teams().TeamFlock(Team))
			str_format(aBuf, sizeof(aBuf), "'%s' locked your team.", pSelf->Server()->ClientName(pResult->m_ClientId));
		else
			str_format(aBuf, sizeof(aBuf), "'%s' locked your team. After the race starts, killing will kill everyone in your team.", pSelf->Server()->ClientName(pResult->m_ClientId));
		pSelf->SendChatTeam(Team, aBuf);
	}
}

void CGameContext::ConUnlock(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	if(g_Config.m_SvTeam == SV_TEAM_FORBIDDEN || g_Config.m_SvTeam == SV_TEAM_FORCED_SOLO)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
			"Teams are disabled");
		return;
	}

	int Team = pSelf->GetDDRaceTeam(pResult->m_ClientId);

	if(Team <= TEAM_FLOCK || Team >= TEAM_SUPER)
		return;

	if(pSelf->ProcessSpamProtection(pResult->m_ClientId, false))
		return;

	pSelf->UnlockTeam(pResult->m_ClientId, Team);
}

void CGameContext::UnlockTeam(int ClientId, int Team) const
{
	m_pController->Teams().SetTeamLock(Team, false);

	char aBuf[512];
	str_format(aBuf, sizeof(aBuf), "'%s' unlocked your team.", Server()->ClientName(ClientId));
	SendChatTeam(Team, aBuf);
}

void CGameContext::AttemptJoinTeam(int ClientId, int Team)
{
	CPlayer *pPlayer = m_apPlayers[ClientId];
	if(!pPlayer)
		return;

	if(IsRunningKickOrSpecVote(ClientId))
	{
		Console()->Print(
			IConsole::OUTPUT_LEVEL_STANDARD,
			"chatresp",
			"You are running a vote please try again after the vote is done!");
		return;
	}
	else if(g_Config.m_SvTeam == SV_TEAM_FORBIDDEN || g_Config.m_SvTeam == SV_TEAM_FORCED_SOLO)
	{
		Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
			"Teams are disabled");
		return;
	}
	else if(g_Config.m_SvTeam == SV_TEAM_MANDATORY && Team == 0 && pPlayer->GetCharacter() && pPlayer->GetCharacter()->m_LastStartWarning < Server()->Tick() - 3 * Server()->TickSpeed())
	{
		Console()->Print(
			IConsole::OUTPUT_LEVEL_STANDARD,
			"chatresp",
			"You must join a team and play with somebody or else you can\'t play");
		pPlayer->GetCharacter()->m_LastStartWarning = Server()->Tick();
	}

	if(pPlayer->GetCharacter() == nullptr)
	{
		Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
			"You can't change teams while you are dead/a spectator.");
	}
	else
	{
		if(Team < 0 || Team >= TEAM_SUPER)
		{
			auto EmptyTeam = m_pController->Teams().GetFirstEmptyTeam();
			if(!EmptyTeam.has_value())
			{
				Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
					"No empty team left.");
				return;
			}
			Team = EmptyTeam.value();
		}

		if(pPlayer->m_LastDDRaceTeamChange + (int64_t)Server()->TickSpeed() * g_Config.m_SvTeamChangeDelay > Server()->Tick())
		{
			Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
				"Не флуди");
		}
		else if(Team != TEAM_FLOCK && m_pController->Teams().TeamLocked(Team) && !m_pController->Teams().IsInvited(Team, ClientId))
		{
			Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
				g_Config.m_SvInvite ?
					"This team is locked using /lock. Only members of the team can unlock it using /lock." :
					"This team is locked using /lock. Only members of the team can invite you or unlock it using /lock.");
		}
		else if(Team != TEAM_FLOCK && m_pController->Teams().Count(Team) >= g_Config.m_SvMaxTeamSize && !m_pController->Teams().TeamFlock(Team) && !m_pController->Teams().IsPractice(Team))
		{
			char aBuf[512];
			str_format(aBuf, sizeof(aBuf), "This team already has the maximum allowed size of %d players", g_Config.m_SvMaxTeamSize);
			Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", aBuf);
		}
		else if(const char *pError = m_pController->Teams().SetCharacterTeam(pPlayer->GetCid(), Team))
		{
			Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", pError);
		}
		else
		{
			if(PracticeByDefault())
			{
				// joined an empty team
				if(m_pController->Teams().Count(Team) == 1)
					m_pController->Teams().SetPractice(Team, true);
			}

			char aBuf[512];
			str_format(aBuf, sizeof(aBuf), "%s вошел в команду %d",
				Server()->ClientName(pPlayer->GetCid()),
				Team);
			SendChat(-1, TEAM_ALL, aBuf);
			pPlayer->m_LastDDRaceTeamChange = Server()->Tick();

			if(m_pController->Teams().IsPractice(Team))
				SendChatTarget(pPlayer->GetCid(), "Practice mode enabled for your team, happy practicing!");

			if(m_pController->Teams().TeamFlock(Team))
				SendChatTarget(pPlayer->GetCid(), "Team 0 mode enabled for your team. This will make your team behave like team 0.");
		}
	}
}

void CGameContext::ConInvite(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	auto *pController = pSelf->m_pController;
	const char *pName = pResult->GetString(0);

	if(g_Config.m_SvTeam == SV_TEAM_FORBIDDEN || g_Config.m_SvTeam == SV_TEAM_FORCED_SOLO)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
			"Teams are disabled");
		return;
	}

	if(!g_Config.m_SvInvite)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "Invites are disabled");
		return;
	}

	int Team = pController->Teams().m_Core.Team(pResult->m_ClientId);
	if(Team > TEAM_FLOCK && Team < TEAM_SUPER)
	{
		int Target = -1;
		for(int i = 0; i < MAX_CLIENTS; i++)
		{
			if(!str_comp(pName, pSelf->Server()->ClientName(i)))
			{
				Target = i;
				break;
			}
		}

		if(Target < 0)
		{
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "Player not found");
			return;
		}

		if(pController->Teams().IsInvited(Team, Target))
		{
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "Player already invited");
			return;
		}

		if(pSelf->m_apPlayers[pResult->m_ClientId] && pSelf->m_apPlayers[pResult->m_ClientId]->m_LastInvited + g_Config.m_SvInviteFrequency * pSelf->Server()->TickSpeed() > pSelf->Server()->Tick())
		{
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "Can't invite this quickly");
			return;
		}

		pController->Teams().SetClientInvited(Team, Target, true);
		pSelf->m_apPlayers[pResult->m_ClientId]->m_LastInvited = pSelf->Server()->Tick();

		char aBuf[512];
		str_format(aBuf, sizeof(aBuf), "'%s' invited you to team %d. Use /team %d to join.", pSelf->Server()->ClientName(pResult->m_ClientId), Team, Team);
		pSelf->SendChatTarget(Target, aBuf);

		str_format(aBuf, sizeof(aBuf), "'%s' invited '%s' to your team.", pSelf->Server()->ClientName(pResult->m_ClientId), pSelf->Server()->ClientName(Target));
		pSelf->SendChatTeam(Team, aBuf);
	}
	else
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "Can't invite players to this team");
}

void CGameContext::ConVanis(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_AccountRole < 4) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно прав");
            return;
        }

        char aBuf[512];
        str_format(aBuf, sizeof(aBuf), "%s отключился", pSelf->Server()->ClientName(pResult->m_ClientId));
        pSelf->SendChat(-1, TEAM_ALL, aBuf, -1, CGameContext::FLAG_SIX);
        pSelf->m_apPlayers[pResult->m_ClientId]->SetTeam(1);
}


void CGameContext::ConUnVanis(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_AccountRole < 4) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно прав");
            return;
        }

        char aBuf[512];
        str_format(aBuf, sizeof(aBuf), "%s подключился", pSelf->Server()->ClientName(pResult->m_ClientId));
        pSelf->SendChat(-1, TEAM_ALL, aBuf, -1, CGameContext::FLAG_SIX);
        pSelf->m_apPlayers[pResult->m_ClientId]->SetTeam(0);
}

void CGameContext::ConTeam0Mode(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	auto *pController = pSelf->m_pController;

	if(!CheckClientId(pResult->m_ClientId))
		return;

	if(g_Config.m_SvTeam == SV_TEAM_FORBIDDEN || g_Config.m_SvTeam == SV_TEAM_FORCED_SOLO || g_Config.m_SvTeam == SV_TEAM_MANDATORY)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
			"Team mode change disabled");
		return;
	}

	if(!g_Config.m_SvTeam0Mode)
	{
		pSelf->Console()->Print(
			IConsole::OUTPUT_LEVEL_STANDARD,
			"chatresp",
			"Team mode change is disabled on this server.");
		return;
	}

	int Team = pController->Teams().m_Core.Team(pResult->m_ClientId);
	bool Mode = pController->Teams().TeamFlock(Team);

	if(Team <= TEAM_FLOCK || Team >= TEAM_SUPER)
	{
		pSelf->Console()->Print(
			IConsole::OUTPUT_LEVEL_STANDARD,
			"chatresp",
			"This team can't have the mode changed");
		return;
	}

	if(pController->Teams().GetTeamState(Team) != ETeamState::OPEN)
	{
		pSelf->SendChatTarget(pResult->m_ClientId, "Team mode can't be changed while racing");
		return;
	}

	if(pResult->NumArguments() > 0)
		Mode = !pResult->GetInteger(0);

	if(pSelf->ProcessSpamProtection(pResult->m_ClientId, false))
		return;

	char aBuf[512];
	if(Mode)
	{
		if(pController->Teams().Count(Team) > g_Config.m_SvMaxTeamSize)
		{
			str_format(aBuf, sizeof(aBuf), "Can't disable team 0 mode. This team exceeds the maximum allowed size of %d players for regular team", g_Config.m_SvMaxTeamSize);
			pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		}
		else
		{
			pController->Teams().SetTeamFlock(Team, false);

			str_format(aBuf, sizeof(aBuf), "'%s' disabled team 0 mode.", pSelf->Server()->ClientName(pResult->m_ClientId));
			pSelf->SendChatTeam(Team, aBuf);
		}
	}
	else
	{
		if(pController->Teams().IsPractice(Team))
		{
			pSelf->SendChatTarget(pResult->m_ClientId, "Can't enable team 0 mode with practice mode on.");
		}
		else
		{
			pController->Teams().SetTeamFlock(Team, true);

			str_format(aBuf, sizeof(aBuf), "'%s' enabled team 0 mode. This will make your team behave like team 0.", pSelf->Server()->ClientName(pResult->m_ClientId));
			pSelf->SendChatTeam(Team, aBuf);
		}
	}
}

void CGameContext::ConTeam(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

	if(pResult->NumArguments() > 0)
	{
		pSelf->AttemptJoinTeam(pResult->m_ClientId, pResult->GetInteger(0));
	}
	else
	{
		char aBuf[512];
		if(!pPlayer->IsPlaying())
		{
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "You can't check your team while you are dead/a spectator.");
		}
		else
		{
			int TeamSize = 0;
			const int PlayerTeam = pSelf->GetDDRaceTeam(pResult->m_ClientId);

			// Count players in team
			for(int ClientId = 0; ClientId < MAX_CLIENTS; ClientId++)
			{
				const CPlayer *pOtherPlayer = pSelf->m_apPlayers[ClientId];
				if(!pOtherPlayer || !pOtherPlayer->IsPlaying())
					continue;

				if(pSelf->GetDDRaceTeam(ClientId) == PlayerTeam)
					TeamSize++;
			}

			str_format(aBuf, sizeof(aBuf), "Вы в команде %d", PlayerTeam);
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", aBuf);
		}
	}
}

void CGameContext::ConJoin(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	int Target = -1;
	const char *pName = pResult->GetString(0);
	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		if(!str_comp(pName, pSelf->Server()->ClientName(i)))
		{
			Target = i;
			break;
		}
	}

	if(Target == -1)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "Player not found");
		return;
	}

	int Team = pSelf->GetDDRaceTeam(Target);
	if(pSelf->ProcessSpamProtection(pResult->m_ClientId, false))
		return;

	pSelf->AttemptJoinTeam(pResult->m_ClientId, Team);
}

void CGameContext::ConEyeEmote(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(g_Config.m_SvEmotionalTees == -1)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
			"Emotes are disabled.");
		return;
	}

	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

	if(pResult->NumArguments() == 0)
	{
		pSelf->Console()->Print(
			IConsole::OUTPUT_LEVEL_STANDARD,
			"chatresp",
			"Emote commands are: /emote surprise /emote blink /emote close /emote angry /emote happy /emote pain /emote normal");
		pSelf->Console()->Print(
			IConsole::OUTPUT_LEVEL_STANDARD,
			"chatresp",
			"Example: /emote surprise 10 for 10 seconds or /emote surprise (default 1 second)");
	}
	else
	{
		if(!pPlayer->CanOverrideDefaultEmote())
			return;

		int EmoteType = 0;
		if(!str_comp_nocase(pResult->GetString(0), "angry"))
			EmoteType = EMOTE_ANGRY;
		else if(!str_comp_nocase(pResult->GetString(0), "blink"))
			EmoteType = EMOTE_BLINK;
		else if(!str_comp_nocase(pResult->GetString(0), "close"))
			EmoteType = EMOTE_BLINK;
		else if(!str_comp_nocase(pResult->GetString(0), "happy"))
			EmoteType = EMOTE_HAPPY;
		else if(!str_comp_nocase(pResult->GetString(0), "pain"))
			EmoteType = EMOTE_PAIN;
		else if(!str_comp_nocase(pResult->GetString(0), "surprise"))
			EmoteType = EMOTE_SURPRISE;
		else if(!str_comp_nocase(pResult->GetString(0), "normal"))
			EmoteType = EMOTE_NORMAL;
		else
		{
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD,
				"chatresp", "Unknown emote... Say /emote");
			return;
		}

		int Duration = 1;
		if(pResult->NumArguments() > 1)
			Duration = std::clamp(pResult->GetInteger(1), 1, 86400);

		pPlayer->OverrideDefaultEmote(EmoteType, pSelf->Server()->Tick() + Duration * pSelf->Server()->TickSpeed());
	}
}

void CGameContext::ConNinjaJetpack(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;
	if(pResult->NumArguments())
		pPlayer->m_NinjaJetpack = pResult->GetInteger(0);
	else
		pPlayer->m_NinjaJetpack = !pPlayer->m_NinjaJetpack;
}

void CGameContext::ConSpecTeam(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

	if(pResult->NumArguments())
		pPlayer->m_SpecTeam = pResult->GetInteger(0);
	else
		pPlayer->m_SpecTeam = !pPlayer->m_SpecTeam;
}

void CGameContext::ConSayTime(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	int ClientId;
	char aBufName[MAX_NAME_LENGTH];

	if(pResult->NumArguments() > 0)
	{
		for(ClientId = 0; ClientId < MAX_CLIENTS; ClientId++)
			if(str_comp(pResult->GetString(0), pSelf->Server()->ClientName(ClientId)) == 0)
				break;

		if(ClientId == MAX_CLIENTS)
			return;

		str_format(aBufName, sizeof(aBufName), "%s's", pSelf->Server()->ClientName(ClientId));
	}
	else
	{
		str_copy(aBufName, "Your", sizeof(aBufName));
		ClientId = pResult->m_ClientId;
	}

	CPlayer *pPlayer = pSelf->m_apPlayers[ClientId];
	if(!pPlayer)
		return;
	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr)
		return;
	if(pChr->m_DDRaceState != ERaceState::STARTED)
		return;

	char aBufTime[32];
	char aBuf[64];
	int64_t Time = (int64_t)100 * (float)(pSelf->Server()->Tick() - pChr->m_StartTime) / ((float)pSelf->Server()->TickSpeed());
	str_time(Time, TIME_HOURS, aBufTime, sizeof(aBufTime));
	str_format(aBuf, sizeof(aBuf), "%s current race time is %s", aBufName, aBufTime);
	pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", aBuf);
}

void CGameContext::ConSayTimeAll(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;
	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr)
		return;
	if(pChr->m_DDRaceState != ERaceState::STARTED)
		return;

	char aBufTime[32];
	char aBuf[64];
	int64_t Time = (int64_t)100 * (float)(pSelf->Server()->Tick() - pChr->m_StartTime) / ((float)pSelf->Server()->TickSpeed());
	const char *pName = pSelf->Server()->ClientName(pResult->m_ClientId);
	str_time(Time, TIME_HOURS, aBufTime, sizeof(aBufTime));
	str_format(aBuf, sizeof(aBuf), "%s\'s current race time is %s", pName, aBufTime);
	pSelf->SendChat(-1, TEAM_ALL, aBuf, pResult->m_ClientId);
}

void CGameContext::ConTime(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;
	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr)
		return;

	char aBufTime[32];
	char aBuf[64];
	int64_t Time = (int64_t)100 * (float)(pSelf->Server()->Tick() - pChr->m_StartTime) / ((float)pSelf->Server()->TickSpeed());
	str_time(Time, TIME_HOURS, aBufTime, sizeof(aBufTime));
	str_format(aBuf, sizeof(aBuf), "Your time is %s", aBufTime);
	pSelf->SendBroadcast(aBuf, pResult->m_ClientId);
}

static const char s_aaMsg[4][128] = {"game/round timer.", "broadcast.", "both game/round timer and broadcast.", "racetime."};

void CGameContext::ConSetTimerType(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;

	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

	char aBuf[128];

	if(pResult->NumArguments() > 0)
	{
		int OldType = pPlayer->m_TimerType;
		bool Result = false;

		if(str_comp_nocase(pResult->GetString(0), "default") == 0)
			Result = pPlayer->SetTimerType(CPlayer::TIMERTYPE_DEFAULT);
		else if(str_comp_nocase(pResult->GetString(0), "gametimer") == 0)
			Result = pPlayer->SetTimerType(CPlayer::TIMERTYPE_GAMETIMER);
		else if(str_comp_nocase(pResult->GetString(0), "broadcast") == 0)
			Result = pPlayer->SetTimerType(CPlayer::TIMERTYPE_BROADCAST);
		else if(str_comp_nocase(pResult->GetString(0), "both") == 0)
			Result = pPlayer->SetTimerType(CPlayer::TIMERTYPE_GAMETIMER_AND_BROADCAST);
		else if(str_comp_nocase(pResult->GetString(0), "none") == 0)
			Result = pPlayer->SetTimerType(CPlayer::TIMERTYPE_NONE);
		else
		{
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "Unknown parameter. Accepted values: default, gametimer, broadcast, both, none");
			return;
		}

		if(!Result)
		{
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "Selected timertype is not supported by your client");
			return;
		}

		if((OldType == CPlayer::TIMERTYPE_BROADCAST || OldType == CPlayer::TIMERTYPE_GAMETIMER_AND_BROADCAST) && (pPlayer->m_TimerType == CPlayer::TIMERTYPE_GAMETIMER || pPlayer->m_TimerType == CPlayer::TIMERTYPE_NONE))
			pSelf->SendBroadcast("", pResult->m_ClientId);
	}

	if(pPlayer->m_TimerType <= CPlayer::TIMERTYPE_SIXUP && pPlayer->m_TimerType >= CPlayer::TIMERTYPE_GAMETIMER)
		str_format(aBuf, sizeof(aBuf), "Timer is displayed in %s", s_aaMsg[pPlayer->m_TimerType]);
	else if(pPlayer->m_TimerType == CPlayer::TIMERTYPE_NONE)
		str_copy(aBuf, "Timer isn't displayed.");

	pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", aBuf);
}

void CGameContext::ConRescue(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;
	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;
	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr)
		return;

	CGameTeams &Teams = pSelf->m_pController->Teams();
	int Team = pSelf->GetDDRaceTeam(pResult->m_ClientId);
	if(!g_Config.m_SvRescue && !Teams.IsPractice(Team))
	{
		pSelf->SendChatTarget(pPlayer->GetCid(), "Rescue is not enabled on this server and you're not in a team with /practice turned on. Note that you can't earn a rank with practice enabled.");
		return;
	}

	bool GoRescue = true;

	if(pPlayer->m_RescueMode == RESCUEMODE_MANUAL)
	{
		// if character can't set his rescue state then we should rescue him instead
		GoRescue = !pChr->TrySetRescue(RESCUEMODE_MANUAL);
	}

	if(GoRescue)
	{
		pChr->Rescue();
		pChr->UnFreeze();
	}
}

void CGameContext::ConRescueMode(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;
	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

	CGameTeams &Teams = pSelf->m_pController->Teams();
	int Team = pSelf->GetDDRaceTeam(pResult->m_ClientId);
	if(!g_Config.m_SvRescue && !Teams.IsPractice(Team))
	{
		pSelf->SendChatTarget(pPlayer->GetCid(), "Rescue is not enabled on this server and you're not in a team with /practice turned on. Note that you can't earn a rank with practice enabled.");
		return;
	}

	if(str_comp_nocase(pResult->GetString(0), "auto") == 0)
	{
		if(pPlayer->m_RescueMode != RESCUEMODE_AUTO)
		{
			pPlayer->m_RescueMode = RESCUEMODE_AUTO;

			pSelf->SendChatTarget(pPlayer->GetCid(), "Rescue mode changed to auto.");
		}

		return;
	}

	if(str_comp_nocase(pResult->GetString(0), "manual") == 0)
	{
		if(pPlayer->m_RescueMode != RESCUEMODE_MANUAL)
		{
			pPlayer->m_RescueMode = RESCUEMODE_MANUAL;

			pSelf->SendChatTarget(pPlayer->GetCid(), "Rescue mode changed to manual.");
		}

		return;
	}

	if(str_comp_nocase(pResult->GetString(0), "list") == 0)
	{
		pSelf->SendChatTarget(pPlayer->GetCid(), "Available rescue modes: auto, manual");
	}
	else if(str_comp_nocase(pResult->GetString(0), "") == 0)
	{
		char aBuf[64];
		str_format(aBuf, sizeof(aBuf), "Current rescue mode: %s.", pPlayer->m_RescueMode == RESCUEMODE_MANUAL ? "manual" : "auto");
		pSelf->SendChatTarget(pPlayer->GetCid(), aBuf);
	}
	else
	{
		pSelf->SendChatTarget(pPlayer->GetCid(), "Unknown argument. Check '/rescuemode list'");
	}
}

CCharacter *CGameContext::GetPracticeCharacter(IConsole::IResult *pResult)
{
	if(!CheckClientId(pResult->m_ClientId))
		return nullptr;
	CPlayer *pPlayer = m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return nullptr;
	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr)
		return nullptr;

	CGameTeams &Teams = m_pController->Teams();
	int Team = GetDDRaceTeam(pResult->m_ClientId);
	if(!Teams.IsPractice(Team))
	{
		SendChatTarget(pPlayer->GetCid(), "You're not in a team with /practice turned on. Note that you can't earn a rank with practice enabled.");
		return nullptr;
	}
	return pChr;
}

void CGameContext::ConPracticeUnSolo(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;
	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;
	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr)
		return;

	if(g_Config.m_SvTeam == SV_TEAM_FORBIDDEN || g_Config.m_SvTeam == SV_TEAM_FORCED_SOLO)
	{
		pSelf->SendChatTarget(pPlayer->GetCid(), "Command is not available on solo servers");
		return;
	}

	CGameTeams &Teams = pSelf->m_pController->Teams();
	int Team = pSelf->GetDDRaceTeam(pResult->m_ClientId);
	if(!Teams.IsPractice(Team))
	{
		pSelf->SendChatTarget(pPlayer->GetCid(), "You're not in a team with /practice turned on. Note that you can't earn a rank with practice enabled.");
		return;
	}
	pChr->SetSolo(false);
}

void CGameContext::ConPracticeSolo(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;
	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;
	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr)
		return;

	if(g_Config.m_SvTeam == SV_TEAM_FORBIDDEN || g_Config.m_SvTeam == SV_TEAM_FORCED_SOLO)
	{
		pSelf->SendChatTarget(pPlayer->GetCid(), "Command is not available on solo servers");
		return;
	}

	CGameTeams &Teams = pSelf->m_pController->Teams();
	int Team = pSelf->GetDDRaceTeam(pResult->m_ClientId);
	if(!Teams.IsPractice(Team))
	{
		pSelf->SendChatTarget(pPlayer->GetCid(), "You're not in a team with /practice turned on. Note that you can't earn a rank with practice enabled.");
		return;
	}
	pChr->SetSolo(true);
}

void CGameContext::ConPracticeUnDeep(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	auto *pChr = pSelf->GetPracticeCharacter(pResult);
	if(!pChr)
		return;

	pChr->SetDeepFrozen(false);
	pChr->UnFreeze();
}

void CGameContext::ConPracticeDeep(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	auto *pChr = pSelf->GetPracticeCharacter(pResult);
	if(!pChr)
		return;

	pChr->SetDeepFrozen(true);
}

void CGameContext::ConPracticeUnLiveFreeze(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	auto *pChr = pSelf->GetPracticeCharacter(pResult);
	if(!pChr)
		return;

	pChr->SetLiveFrozen(false);
}

void CGameContext::ConPracticeLiveFreeze(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	auto *pChr = pSelf->GetPracticeCharacter(pResult);
	if(!pChr)
		return;

	pChr->SetLiveFrozen(true);
}

void CGameContext::ConPracticeShotgun(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(pSelf->GetPracticeCharacter(pResult))
		ConShotgun(pResult, pUserData);
}

void CGameContext::ConPracticeGrenade(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(pSelf->GetPracticeCharacter(pResult))
		ConGrenade(pResult, pUserData);
}

void CGameContext::ConPracticeLaser(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(pSelf->GetPracticeCharacter(pResult))
		ConLaser(pResult, pUserData);
}

void CGameContext::ConPracticeJetpack(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(pSelf->GetPracticeCharacter(pResult))
		ConJetpack(pResult, pUserData);
}

void CGameContext::ConPracticeEndlessJump(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(pSelf->GetPracticeCharacter(pResult))
		ConEndlessJump(pResult, pUserData);
}

void CGameContext::ConPracticeSetJumps(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(pSelf->GetPracticeCharacter(pResult))
		ConSetJumps(pResult, pUserData);
}

void CGameContext::ConPracticeWeapons(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(pSelf->GetPracticeCharacter(pResult))
		ConWeapons(pResult, pUserData);
}

void CGameContext::ConPracticeUnShotgun(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(pSelf->GetPracticeCharacter(pResult))
		ConUnShotgun(pResult, pUserData);
}

void CGameContext::ConPracticeUnGrenade(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(pSelf->GetPracticeCharacter(pResult))
		ConUnGrenade(pResult, pUserData);
}

void CGameContext::ConPracticeUnLaser(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(pSelf->GetPracticeCharacter(pResult))
		ConUnLaser(pResult, pUserData);
}

void CGameContext::ConPracticeUnJetpack(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(pSelf->GetPracticeCharacter(pResult))
		ConUnJetpack(pResult, pUserData);
}

void CGameContext::ConPracticeUnEndlessJump(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(pSelf->GetPracticeCharacter(pResult))
		ConUnEndlessJump(pResult, pUserData);
}

void CGameContext::ConPracticeUnWeapons(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(pSelf->GetPracticeCharacter(pResult))
		ConUnWeapons(pResult, pUserData);
}

void CGameContext::ConPracticeNinja(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(pSelf->GetPracticeCharacter(pResult))
		ConNinja(pResult, pUserData);
}

void CGameContext::ConPracticeUnNinja(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(pSelf->GetPracticeCharacter(pResult))
		ConUnNinja(pResult, pUserData);
}

void CGameContext::ConPracticeEndlessHook(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(pSelf->GetPracticeCharacter(pResult))
		ConEndlessHook(pResult, pUserData);
}

void CGameContext::ConPracticeUnEndlessHook(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(pSelf->GetPracticeCharacter(pResult))
		ConUnEndlessHook(pResult, pUserData);
}

void CGameContext::ConPracticeToggleInvincible(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(pSelf->GetPracticeCharacter(pResult))
		ConToggleInvincible(pResult, pUserData);
}

void CGameContext::ConPracticeAddWeapon(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(pSelf->GetPracticeCharacter(pResult))
		ConAddWeapon(pResult, pUserData);
}

void CGameContext::ConPracticeRemoveWeapon(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(pSelf->GetPracticeCharacter(pResult))
		ConRemoveWeapon(pResult, pUserData);
}

void CGameContext::ConProtectedKill(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;
	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;
	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr)
		return;

	int CurrTime = (pSelf->Server()->Tick() - pChr->m_StartTime) / pSelf->Server()->TickSpeed();
	if(g_Config.m_SvKillProtection != 0 && CurrTime >= (60 * g_Config.m_SvKillProtection) && pChr->m_DDRaceState == ERaceState::STARTED)
	{
		pPlayer->KillCharacter(WEAPON_SELF);
		pPlayer->Respawn();
	}
}

void CGameContext::ConStats(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_AccountRole < 5) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно прав");
            return;
        }

        int ClientId = 0;
        for(ClientId = 0; ClientId < MAX_CLIENTS; ClientId = ClientId + 1) {
            if(str_comp(pResult->GetString(0), pSelf->Server()->ClientName(ClientId)) == 0)
                break;
        }

        char aBuf[255];

        if(ClientId == MAX_CLIENTS) {
            str_format(aBuf, sizeof(aBuf), "Игрок %s не найден", pResult->GetString(0));
            pSelf->SendChatTarget(pPlayer->GetCid(), aBuf);
            return;
        }

        CPlayer *pPlayerTarget = pSelf->m_apPlayers[ClientId];
        if(!pPlayerTarget)
            return;

        str_format(aBuf, sizeof(aBuf), "Статистика игрока %s:\nID: %i\nIP-Адресс: %s\nУровень доступа: %i\nБаланс: %i пойнтов",
                pResult->GetString(0), pPlayerTarget->m_AccountId, pSelf->Server()->ClientAddrString(ClientId, false), pPlayerTarget->m_AccountRole, pPlayerTarget->m_AccountPoints);
        pSelf->SendMotd(pResult->m_ClientId, aBuf);
}

void CGameContext::ConPoints(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;


        char aBuf[255];
        str_format(aBuf, sizeof(aBuf), "На вашем балансе: %i пойнтов", pPlayer->m_AccountPoints);
        pSelf->SendChatTarget(pPlayer->GetCid(), aBuf);
}

void CGameContext::ConOrel(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) return;

	if(pPlayer->m_OrelReshkaCode != 0)
	{
		pSelf->SendChatTarget(pPlayer->GetCid(), "У вас уже есть активное предложение. Дождитесь ответа.");
		return;
	}

	if(str_comp(pSelf->Server()->ClientName(pPlayer->GetCid()), pResult->GetString(0)) == 0)
		return;

	int bet = pResult->GetInteger(1);
	if(bet <= 0)
		return;

	if(pPlayer->m_AccountPoints < bet)
	{
		pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно пойнтов");
		return;
	}

	int ClientId = -1;
	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		if(pSelf->m_apPlayers[i] && str_comp(pResult->GetString(0), pSelf->Server()->ClientName(i)) == 0)
		{
			ClientId = i;
			break;
		}
	}

	if(ClientId == -1)
	{
		pSelf->SendChatTarget(pPlayer->GetCid(), "Игрок не найден");
		return;
	}

	CPlayer *pTarget = pSelf->m_apPlayers[ClientId];
	if(!pTarget || !pTarget->m_IsHaveAccount)
	{
		pSelf->SendChatTarget(pPlayer->GetCid(), "Игрок не зарегистрирован");
		return;
	}

	if(pTarget->m_AccountPoints < bet)
	{
		pSelf->SendChatTarget(pPlayer->GetCid(), "У игрока недостаточно пойнтов");
		return;
	}

	if(pTarget->m_OrelReshkaCode != 0)
	{
		pSelf->SendChatTarget(pPlayer->GetCid(), "Этот игрок уже участвует в другой игре орёл/решка");
		return;
	}

	pPlayer->m_OrelReshkaPoints = bet;
	pPlayer->m_OrelReshkaCode = 0;
	pPlayer->m_OrelReshkaOpponent = ClientId;

	pTarget->m_OrelReshkaPoints = bet;
	pTarget->m_OrelReshkaCode = 2;
	pTarget->m_OrelReshkaOpponent = pPlayer->GetCid();

	char aBuf[256];
	str_format(aBuf, sizeof(aBuf), "Вы предложили %s сыграть на %d пойнтов (ваша сторона: Орёл)", pResult->GetString(0), bet);
	pSelf->SendChatTarget(pPlayer->GetCid(), aBuf);

	str_format(aBuf, sizeof(aBuf), "%s предложил вам орёл/решку на %d пойнтов (ваша сторона: Решка)", pSelf->Server()->ClientName(pPlayer->GetCid()), bet);
	pSelf->SendChatTarget(ClientId, aBuf);
	pSelf->SendChatTarget(ClientId, "Принять: /yes Отклонить: /no");
}

void CGameContext::ConReshka(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) return;

	if(pPlayer->m_OrelReshkaCode != 0)
	{
		pSelf->SendChatTarget(pPlayer->GetCid(), "У вас уже есть активное предложение. Дождитесь ответа.");
		return;
	}

	if(str_comp(pSelf->Server()->ClientName(pPlayer->GetCid()), pResult->GetString(0)) == 0)
		return;

	int bet = pResult->GetInteger(1);
	if(bet <= 0)
		return;

	if(pPlayer->m_AccountPoints < bet)
	{
		pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно пойнтов");
		return;
	}

	int ClientId = -1;
	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		if(pSelf->m_apPlayers[i] && str_comp(pResult->GetString(0), pSelf->Server()->ClientName(i)) == 0)
		{
			ClientId = i;
			break;
		}
	}

	if(ClientId == -1)
	{
		pSelf->SendChatTarget(pPlayer->GetCid(), "Игрок не найден");
		return;
	}

	CPlayer *pTarget = pSelf->m_apPlayers[ClientId];
	if(!pTarget || !pTarget->m_IsHaveAccount)
	{
		pSelf->SendChatTarget(pPlayer->GetCid(), "Игрок не зарегистрирован");
		return;
	}

	if(pTarget->m_AccountPoints < bet)
	{
		pSelf->SendChatTarget(pPlayer->GetCid(), "У игрока недостаточно пойнтов");
		return;
	}

	if(pTarget->m_OrelReshkaCode != 0)
	{
		pSelf->SendChatTarget(pPlayer->GetCid(), "Этот игрок уже участвует в другой игре орёл/решка");
		return;
	}

	pPlayer->m_OrelReshkaPoints = bet;
	pPlayer->m_OrelReshkaCode = 0;
	pPlayer->m_OrelReshkaOpponent = ClientId;

	pTarget->m_OrelReshkaPoints = bet;
	pTarget->m_OrelReshkaCode = 1;
	pTarget->m_OrelReshkaOpponent = pPlayer->GetCid();

	char aBuf[256];
	str_format(aBuf, sizeof(aBuf), "Вы предложили %s сыграть на %d пойнтов (ваша сторона: Решка)", pResult->GetString(0), bet);
	pSelf->SendChatTarget(pPlayer->GetCid(), aBuf);

	str_format(aBuf, sizeof(aBuf), "%s предложил вам орёл/решку на %d пойнтов (ваша сторона: Орёл)", pSelf->Server()->ClientName(pPlayer->GetCid()), bet);
	pSelf->SendChatTarget(ClientId, aBuf);
	pSelf->SendChatTarget(ClientId, "Принять: /yes Отклонить: /no");
}

void CGameContext::ConYes(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) {
		return;
	}

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
		return;
	}

	if (pPlayer->m_OrelReshkaCode == 0 || pPlayer->m_OrelReshkaOpponent == -1)
	{
		pSelf->SendChatTarget(pPlayer->GetCid(), "Вам никто не предлагал сыграть в орёл и решку");
		return;
	}

	int ClientId = pPlayer->m_OrelReshkaOpponent;
	
	if (pPlayer->GetCid() == ClientId)
	{
		pSelf->SendChatTarget(pPlayer->GetCid(), "Вы не можете принять своё собственное предложение");
		pPlayer->m_OrelReshkaCode = 0;
		pPlayer->m_OrelReshkaPoints = 0;
		pPlayer->m_OrelReshkaOpponent = -1;
		return;
	}

	CPlayer *pPlayerTarget = pSelf->m_apPlayers[ClientId];

	if(!pPlayerTarget || pPlayerTarget->m_OrelReshkaOpponent != pPlayer->GetCid())
	{
		pPlayer->m_OrelReshkaCode = 0;
		pPlayer->m_OrelReshkaOpponent = -1;
		pSelf->SendChatTarget(pPlayer->GetCid(), "Предложение больше недействительно");
		return;
	}

	if (pPlayer->m_AccountPoints < pPlayer->m_OrelReshkaPoints)
	{
		pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно пойнтов");
		pPlayer->m_OrelReshkaCode = 0;
		pPlayer->m_OrelReshkaPoints = 0;
		pPlayer->m_OrelReshkaOpponent = -1;
		pPlayerTarget->m_OrelReshkaCode = 0;
		pPlayerTarget->m_OrelReshkaPoints = 0;
		pPlayerTarget->m_OrelReshkaOpponent = -1;
		return;
	}

	if (pPlayerTarget->m_AccountPoints < pPlayerTarget->m_OrelReshkaPoints)
	{
		pSelf->SendChatTarget(pPlayer->GetCid(), "У вашего соперника недостаточно пойнтов");
		pPlayer->m_OrelReshkaCode = 0;
		pPlayer->m_OrelReshkaPoints = 0;
		pPlayer->m_OrelReshkaOpponent = -1;
		pPlayerTarget->m_OrelReshkaCode = 0;
		pPlayerTarget->m_OrelReshkaPoints = 0;
		pPlayerTarget->m_OrelReshkaOpponent = -1;
		return;
	}

	char aBuf[255];
	str_format(aBuf, sizeof(aBuf), "%s принял ваше предложение!", pSelf->Server()->ClientName(pPlayer->GetCid()));
	pSelf->SendChatTarget(ClientId, aBuf);

	int winnerCode = (rand() % 2) + 1;

	if (winnerCode == pPlayer->m_OrelReshkaCode)
	{
		str_format(aBuf, sizeof(aBuf), "WIN +%i", pPlayer->m_OrelReshkaPoints);
		pSelf->SendBroadcast(aBuf, pPlayer->GetCid());
		str_format(aBuf, sizeof(aBuf), "LOSE -%i", pPlayer->m_OrelReshkaPoints);
		pSelf->SendBroadcast(aBuf, ClientId);

		pPlayer->m_AccountPoints += pPlayer->m_OrelReshkaPoints;
		pPlayerTarget->m_AccountPoints -= pPlayer->m_OrelReshkaPoints;
	}
	else
	{
		str_format(aBuf, sizeof(aBuf), "LOSE -%i", pPlayer->m_OrelReshkaPoints);
		pSelf->SendBroadcast(aBuf, pPlayer->GetCid());
		str_format(aBuf, sizeof(aBuf), "WIN +%i", pPlayer->m_OrelReshkaPoints);
		pSelf->SendBroadcast(aBuf, ClientId);

		pPlayer->m_AccountPoints -= pPlayer->m_OrelReshkaPoints;
		pPlayerTarget->m_AccountPoints += pPlayer->m_OrelReshkaPoints;
	}

	pSelf->Score()->ChangePointsAccount(pSelf->Server()->ClientName(pPlayer->GetCid()), pPlayer->m_AccountPoints);
	pSelf->Score()->ChangePointsAccount(pSelf->Server()->ClientName(ClientId), pPlayerTarget->m_AccountPoints);

	str_format(aBuf, sizeof(aBuf), "На вашем балансе: %i пойнтов", pPlayer->m_AccountPoints);
	pSelf->SendChatTarget(pPlayer->GetCid(), aBuf);
	str_format(aBuf, sizeof(aBuf), "На вашем балансе: %i пойнтов", pPlayerTarget->m_AccountPoints);
	pSelf->SendChatTarget(ClientId, aBuf);

	pPlayer->m_OrelReshkaCode = 0;
	pPlayer->m_OrelReshkaPoints = 0;
	pPlayer->m_OrelReshkaOpponent = -1;
	pPlayerTarget->m_OrelReshkaCode = 0;
	pPlayerTarget->m_OrelReshkaPoints = 0;
	pPlayerTarget->m_OrelReshkaOpponent = -1;
}

void CGameContext::ConNo(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) {
		return;
	}

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
		return;
	}

	if (pPlayer->m_OrelReshkaCode == 0 || pPlayer->m_OrelReshkaOpponent == -1)
	{
		pSelf->SendChatTarget(pPlayer->GetCid(), "Вам никто не предлагал сыграть в орёл и решку");
		return;
	}

	int ClientId = pPlayer->m_OrelReshkaOpponent;
	CPlayer *pPlayerTarget = pSelf->m_apPlayers[ClientId];

	if(pPlayerTarget && pPlayerTarget->m_OrelReshkaOpponent == pPlayer->GetCid())
	{
		char aBuf[255];
		str_format(aBuf, sizeof(aBuf), "%s отклонил ваше предложение", pSelf->Server()->ClientName(pPlayer->GetCid()));
		pSelf->SendChatTarget(ClientId, aBuf);
	}

	pPlayer->m_OrelReshkaCode = 0;
	pPlayer->m_OrelReshkaPoints = 0;
	pPlayer->m_OrelReshkaOpponent = -1;
	if(pPlayerTarget)
	{
		pPlayerTarget->m_OrelReshkaCode = 0;
		pPlayerTarget->m_OrelReshkaPoints = 0;
		pPlayerTarget->m_OrelReshkaOpponent = -1;
	}
}

void CGameContext::ConPayPoints(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId)) {
            return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer) {
            return;
        }

        if (str_comp(pSelf->Server()->ClientName(pPlayer->GetCid()), pResult->GetString(0)) == 0) {
            return;            
        }

        if (pPlayer->m_AccountPoints < pResult->GetInteger(1)) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно пойнтов");
            return;
        }

        if (pResult->GetInteger(1) <= 0) {
            return;
        }

        int ClientId = 0;
        for(ClientId = 0; ClientId < MAX_CLIENTS; ClientId = ClientId + 1) {
            if(str_comp(pResult->GetString(0), pSelf->Server()->ClientName(ClientId)) == 0)
                break;
        }

        char aBuf[255];

        if(ClientId == MAX_CLIENTS) {
            str_format(aBuf, sizeof(aBuf), "Игрок %s не найден", pResult->GetString(0));
            pSelf->SendChatTarget(pPlayer->GetCid(), aBuf);
            return;
        }

        CPlayer *pPlayerTarget = pSelf->m_apPlayers[ClientId];
        if(!pPlayerTarget)
            return;

        if (pPlayerTarget->m_IsHaveAccount == false) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "Игрок не зарегистрирован");
            return;
        }

        if (pPlayer->m_AccountPoints < pResult->GetInteger(1)) {
            pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно пойнтов");
            return;
        }

        pPlayer->m_AccountPoints = pPlayer->m_AccountPoints - pResult->GetInteger(1);
        pSelf->Score()->ChangePointsAccount(pSelf->Server()->ClientName(pPlayer->GetCid()), pPlayer->m_AccountPoints);
        pPlayerTarget->m_AccountPoints = pPlayerTarget->m_AccountPoints + pResult->GetInteger(1);
        pSelf->Score()->ChangePointsAccount(pResult->GetString(0), pPlayerTarget->m_AccountPoints);

        str_format(aBuf, sizeof(aBuf), "Вы передали %s %i пойнтов", pResult->GetString(0), pResult->GetInteger(1));
        pSelf->SendChatTarget(pPlayer->GetCid(), aBuf);
        str_format(aBuf, sizeof(aBuf), "%s передал вам %i пойнтов", pSelf->Server()->ClientName(pPlayer->GetCid()), pResult->GetInteger(1));
        pSelf->SendChatTarget(pPlayerTarget->GetCid(), aBuf);
}

void CGameContext::ConTopPoints(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	if(g_Config.m_SvHideScore)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
			"Showing the global top points is not allowed on this server.");
		return;
	}

	if(pResult->NumArguments() > 0)
		pSelf->Score()->ShowTopPoints(pResult->m_ClientId, pResult->GetInteger(0));
	else
		pSelf->Score()->ShowTopPoints(pResult->m_ClientId);
}

void CGameContext::ConTimeCP(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	if(g_Config.m_SvHideScore)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
			"Showing the checkpoint times is not allowed on this server.");
		return;
	}

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

	const char *pName = pResult->GetString(0);
	pSelf->Score()->LoadPlayerTimeCp(pResult->m_ClientId, pName);
}
