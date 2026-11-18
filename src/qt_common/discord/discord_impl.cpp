// SPDX-FileCopyrightText: Copyright 2026 Eden Emulator Project
// SPDX-License-Identifier: GPL-3.0-or-later

// SPDX-FileCopyrightText: 2018 Citra Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include <chrono>
#include <string>

#include <QEventLoop>
#include <boost/algorithm/string/replace.hpp>
#include "common/httplib.h"

#include <discord_rpc.h>
#include <fmt/format.h>

#include "common/common_types.h"
#include "common/string_util.h"
#include "core/core.h"
#include "core/loader/loader.h"

#include "discord_impl.h"

#ifdef YUZU_BUNDLED_OPENSSL
#include <openssl/cert.h>
#endif

namespace DiscordRPC {

DiscordImpl::DiscordImpl(Core::System& system_) : system{system_} {
    DiscordEventHandlers handlers{};
    // AghathNX's own Discord application (src/qt_common/CMakeLists.txt); Discord takes the name
    // shown from it.
    Discord_Initialize(AGHATHNX_DISCORD_CLIENT_ID, &handlers, 1, nullptr);
}

DiscordImpl::~DiscordImpl() {
    Discord_ClearPresence();
    Discord_Shutdown();
}

void DiscordImpl::Pause() {
    Discord_ClearPresence();
}

std::string DiscordImpl::GetGameString(const std::string& title) {
    // Convert to lowercase
    std::string icon_name = Common::ToLower(title);

    // Replace spaces with dashes
    std::replace(icon_name.begin(), icon_name.end(), ' ', '-');
    boost::replace_all(icon_name, "é", "e");

    // Remove non-alphanumeric characters but keep dashes
    std::erase_if(icon_name, [](char c) { return !std::isalnum(c) && c != '-'; });

    // Remove dashes from the start and end of the string
    icon_name.erase(icon_name.begin(), std::find_if(icon_name.begin(), icon_name.end(),
                                                    [](int ch) { return ch != '-'; }));
    icon_name.erase(
        std::find_if(icon_name.rbegin(), icon_name.rend(), [](int ch) { return ch != '-'; }).base(),
        icon_name.end());

    // Remove double dashes
    icon_name.erase(std::unique(icon_name.begin(), icon_name.end(),
                                [](char a, char b) { return a == '-' && b == '-'; }),
                    icon_name.end());

    return icon_name;
}

static constexpr char DEFAULT_DISCORD_TEXT[] = "AghathNX is an emulator for the Nintendo Switch";
static constexpr char DEFAULT_DISCORD_IMAGE[] =
    "https://raw.githubusercontent.com/VictorSdm/AghathNX/main/dist/aghathnx.png";

void DiscordImpl::UpdateGameStatus(std::string_view game_url, bool has_boxart) {
    const std::string url = std::string{has_boxart ? game_url : DEFAULT_DISCORD_IMAGE};
    s64 start_time = std::chrono::duration_cast<std::chrono::seconds>(
                         std::chrono::system_clock::now().time_since_epoch())
                         .count();
    DiscordRichPresence presence{};

    presence.largeImageKey = url.c_str();
    presence.largeImageText = game_title.c_str();
    presence.smallImageKey = DEFAULT_DISCORD_IMAGE;
    presence.smallImageText = DEFAULT_DISCORD_TEXT;
    presence.state = game_title.c_str();
    presence.status_display_type = DiscordStatusDisplayType_State;
    presence.details = "Currently in game";
    presence.startTimestamp = start_time;
    Discord_UpdatePresence(&presence);
}

void DiscordImpl::Update() {
    if (system.IsPoweredOn()) {
        system.GetAppLoader().ReadTitle(game_title);
        // AghathNX: no game picture from Eden's boxart repository; AghathNX's icon instead.
        UpdateGameStatus({}, false);
        return;
    }

    s64 start_time = std::chrono::duration_cast<std::chrono::seconds>(
                         std::chrono::system_clock::now().time_since_epoch())
                         .count();
    DiscordRichPresence presence{};
    presence.largeImageKey = DEFAULT_DISCORD_IMAGE;
    presence.largeImageText = DEFAULT_DISCORD_TEXT;
    presence.status_display_type = DiscordStatusDisplayType_Name;
    presence.details = "Currently not in game";
    presence.startTimestamp = start_time;
    Discord_UpdatePresence(&presence);
}
} // namespace DiscordRPC
