// Copyright 2020 Valentin Vanelslande
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <fstream>
#include <iostream>
#include <sstream>

#include <nlohmann/json.hpp>
#include <whereami.h>

#include "common_types.h"
#include "string_util.h"

#ifdef _WIN32
#define VVCTRE_PLUGIN_EXPORT extern "C" __declspec(dllexport)
#else
#define VVCTRE_PLUGIN_EXPORT extern "C"
#endif

static const char* required_function_names[] = {
    "vvctre_settings_set_limit_speed",
    "vvctre_settings_get_limit_speed",
    "vvctre_settings_set_speed_limit",
    "vvctre_settings_get_speed_limit",
    "vvctre_settings_set_use_custom_cpu_ticks",
    "vvctre_settings_get_use_custom_cpu_ticks",
    "vvctre_settings_set_custom_cpu_ticks",
    "vvctre_settings_get_custom_cpu_ticks",
    "vvctre_settings_set_cpu_clock_percentage",
    "vvctre_settings_get_cpu_clock_percentage",
    "vvctre_settings_set_audio_volume",
    "vvctre_settings_get_audio_volume",
    "vvctre_settings_set_resolution",
    "vvctre_settings_get_resolution",
    "vvctre_settings_set_post_processing_shader",
    "vvctre_settings_get_post_processing_shader",
    "vvctre_settings_set_texture_filter",
    "vvctre_settings_get_texture_filter",
    "vvctre_settings_set_render_3d",
    "vvctre_settings_get_render_3d",
    "vvctre_settings_set_factor_3d",
    "vvctre_settings_get_factor_3d",
    "vvctre_settings_set_layout",
    "vvctre_settings_get_layout",
    "vvctre_settings_set_use_custom_layout",
    "vvctre_settings_get_use_custom_layout",
    "vvctre_settings_set_swap_screens",
    "vvctre_settings_get_swap_screens",
    "vvctre_settings_set_upright_screens",
    "vvctre_settings_get_upright_screens",
};

typedef void (*vvctre_settings_set_limit_speed_t)(bool value);
typedef bool (*vvctre_settings_get_limit_speed_t)();
typedef void (*vvctre_settings_set_speed_limit_t)(u16 value);
typedef u16 (*vvctre_settings_get_speed_limit_t)();
typedef void (*vvctre_settings_set_use_custom_cpu_ticks_t)(bool value);
typedef bool (*vvctre_settings_get_use_custom_cpu_ticks_t)();
typedef void (*vvctre_settings_set_custom_cpu_ticks_t)(u64 value);
typedef u64 (*vvctre_settings_get_custom_cpu_ticks_t)();
typedef void (*vvctre_settings_set_cpu_clock_percentage_t)(u32 value);
typedef u32 (*vvctre_settings_get_cpu_clock_percentage_t)();
static vvctre_settings_set_limit_speed_t vvctre_settings_set_limit_speed;
static vvctre_settings_get_limit_speed_t vvctre_settings_get_limit_speed;
static vvctre_settings_set_speed_limit_t vvctre_settings_set_speed_limit;
static vvctre_settings_get_speed_limit_t vvctre_settings_get_speed_limit;
static vvctre_settings_set_use_custom_cpu_ticks_t vvctre_settings_set_use_custom_cpu_ticks;
static vvctre_settings_get_use_custom_cpu_ticks_t vvctre_settings_get_use_custom_cpu_ticks;
static vvctre_settings_set_custom_cpu_ticks_t vvctre_settings_set_custom_cpu_ticks;
static vvctre_settings_get_custom_cpu_ticks_t vvctre_settings_get_custom_cpu_ticks;
static vvctre_settings_set_cpu_clock_percentage_t vvctre_settings_set_cpu_clock_percentage;
static vvctre_settings_get_cpu_clock_percentage_t vvctre_settings_get_cpu_clock_percentage;

typedef void (*vvctre_settings_set_audio_volume_t)(float value);
typedef float (*vvctre_settings_get_audio_volume_t)();
static vvctre_settings_set_audio_volume_t vvctre_settings_set_audio_volume;
static vvctre_settings_get_audio_volume_t vvctre_settings_get_audio_volume;

typedef void (*vvctre_settings_set_resolution_t)(u16 value);
typedef u16 (*vvctre_settings_get_resolution_t)();
typedef void (*vvctre_settings_set_post_processing_shader_t)(const char* value);
typedef const char* (*vvctre_settings_get_post_processing_shader_t)();
typedef void (*vvctre_settings_set_texture_filter_t)(const char* value);
typedef const char* (*vvctre_settings_get_texture_filter_t)();
typedef void (*vvctre_settings_set_render_3d_t)(int value);
typedef int (*vvctre_settings_get_render_3d_t)();
typedef void (*vvctre_settings_set_factor_3d_t)(u8 value);
typedef u8 (*vvctre_settings_get_factor_3d_t)();
static vvctre_settings_set_resolution_t vvctre_settings_set_resolution;
static vvctre_settings_get_resolution_t vvctre_settings_get_resolution;
static vvctre_settings_set_post_processing_shader_t vvctre_settings_set_post_processing_shader;
static vvctre_settings_get_post_processing_shader_t vvctre_settings_get_post_processing_shader;
static vvctre_settings_set_texture_filter_t vvctre_settings_set_texture_filter;
static vvctre_settings_get_texture_filter_t vvctre_settings_get_texture_filter;
static vvctre_settings_set_render_3d_t vvctre_settings_set_render_3d;
static vvctre_settings_get_render_3d_t vvctre_settings_get_render_3d;
static vvctre_settings_set_factor_3d_t vvctre_settings_set_factor_3d;
static vvctre_settings_get_factor_3d_t vvctre_settings_get_factor_3d;

typedef void (*vvctre_settings_set_layout_t)(int value);
typedef int (*vvctre_settings_get_layout_t)();
typedef void (*vvctre_settings_set_use_custom_layout_t)(bool value);
typedef bool (*vvctre_settings_get_use_custom_layout_t)();
typedef void (*vvctre_settings_set_swap_screens_t)(bool value);
typedef bool (*vvctre_settings_get_swap_screens_t)();
typedef void (*vvctre_settings_set_upright_screens_t)(bool value);
typedef bool (*vvctre_settings_get_upright_screens_t)();

static vvctre_settings_set_layout_t vvctre_settings_set_layout;
static vvctre_settings_get_layout_t vvctre_settings_get_layout;
static vvctre_settings_set_use_custom_layout_t vvctre_settings_set_use_custom_layout;
static vvctre_settings_get_use_custom_layout_t vvctre_settings_get_use_custom_layout;
static vvctre_settings_set_swap_screens_t vvctre_settings_set_swap_screens;
static vvctre_settings_get_swap_screens_t vvctre_settings_get_swap_screens;
static vvctre_settings_set_upright_screens_t vvctre_settings_set_upright_screens;
static vvctre_settings_get_upright_screens_t vvctre_settings_get_upright_screens;

static nlohmann::json json;

VVCTRE_PLUGIN_EXPORT int GetRequiredFunctionCount() {
    return 30;
}

VVCTRE_PLUGIN_EXPORT const char** GetRequiredFunctionNames() {
    return required_function_names;
}

VVCTRE_PLUGIN_EXPORT void PluginLoaded(void* core, void* plugin_manager,
                                       void* required_functions[]) {
    vvctre_settings_set_limit_speed = (vvctre_settings_set_limit_speed_t)required_functions[0];
    vvctre_settings_get_limit_speed = (vvctre_settings_get_limit_speed_t)required_functions[1];
    vvctre_settings_set_speed_limit = (vvctre_settings_set_speed_limit_t)required_functions[2];
    vvctre_settings_get_speed_limit = (vvctre_settings_get_speed_limit_t)required_functions[3];
    vvctre_settings_set_use_custom_cpu_ticks =
        (vvctre_settings_set_use_custom_cpu_ticks_t)required_functions[4];
    vvctre_settings_get_use_custom_cpu_ticks =
        (vvctre_settings_get_use_custom_cpu_ticks_t)required_functions[5];
    vvctre_settings_set_custom_cpu_ticks =
        (vvctre_settings_set_custom_cpu_ticks_t)required_functions[6];
    vvctre_settings_get_custom_cpu_ticks =
        (vvctre_settings_get_custom_cpu_ticks_t)required_functions[7];
    vvctre_settings_set_cpu_clock_percentage =
        (vvctre_settings_set_cpu_clock_percentage_t)required_functions[8];
    vvctre_settings_get_cpu_clock_percentage =
        (vvctre_settings_get_cpu_clock_percentage_t)required_functions[9];
    vvctre_settings_set_audio_volume = (vvctre_settings_set_audio_volume_t)required_functions[10];
    vvctre_settings_get_audio_volume = (vvctre_settings_get_audio_volume_t)required_functions[11];
    vvctre_settings_set_resolution = (vvctre_settings_set_resolution_t)required_functions[12];
    vvctre_settings_get_resolution = (vvctre_settings_get_resolution_t)required_functions[13];
    vvctre_settings_set_post_processing_shader =
        (vvctre_settings_set_post_processing_shader_t)required_functions[14];
    vvctre_settings_get_post_processing_shader =
        (vvctre_settings_get_post_processing_shader_t)required_functions[15];
    vvctre_settings_set_texture_filter =
        (vvctre_settings_set_texture_filter_t)required_functions[16];
    vvctre_settings_get_texture_filter =
        (vvctre_settings_get_texture_filter_t)required_functions[17];
    vvctre_settings_set_render_3d = (vvctre_settings_set_render_3d_t)required_functions[18];
    vvctre_settings_get_render_3d = (vvctre_settings_get_render_3d_t)required_functions[19];
    vvctre_settings_set_factor_3d = (vvctre_settings_set_factor_3d_t)required_functions[20];
    vvctre_settings_get_factor_3d = (vvctre_settings_get_factor_3d_t)required_functions[21];
    vvctre_settings_set_layout = (vvctre_settings_set_layout_t)required_functions[22];
    vvctre_settings_get_layout = (vvctre_settings_get_layout_t)required_functions[23];
    vvctre_settings_set_use_custom_layout =
        (vvctre_settings_set_use_custom_layout_t)required_functions[24];
    vvctre_settings_get_use_custom_layout =
        (vvctre_settings_get_use_custom_layout_t)required_functions[25];
    vvctre_settings_set_swap_screens = (vvctre_settings_set_swap_screens_t)required_functions[26];
    vvctre_settings_get_swap_screens = (vvctre_settings_get_swap_screens_t)required_functions[27];
    vvctre_settings_set_upright_screens =
        (vvctre_settings_set_upright_screens_t)required_functions[28];
    vvctre_settings_get_upright_screens =
        (vvctre_settings_get_upright_screens_t)required_functions[29];
}

VVCTRE_PLUGIN_EXPORT void InitialSettingsOpening() {
    int length = wai_getExecutablePath(nullptr, 0, nullptr);
    std::string vvctre_folder(length, '\0');
    int dirname_length = 0;
    wai_getExecutablePath(&vvctre_folder[0], length, &dirname_length);
    vvctre_folder = vvctre_folder.substr(0, dirname_length);

    std::ifstream file;
#ifdef _MSC_VER
    file.open(Common::UTF8ToUTF16W(vvctre_folder + "\\quick-settings.json"));
#else
    file.open(vvctre_folder + "/quick-settings.json");
#endif

    if (!file.fail()) {
        std::ostringstream oss;
        oss << file.rdbuf();

        json = nlohmann::json::parse(oss.str());

        if (json.count("general")) {
            if (json["general"].count("limit_speed")) {
                vvctre_settings_set_limit_speed(json["general"]["limit_speed"].get<bool>());
            }
            if (json["general"].count("speed_limit")) {
                vvctre_settings_set_speed_limit(json["general"]["speed_limit"].get<u16>());
            }
            if (json["general"].count("use_custom_cpu_ticks")) {
                vvctre_settings_set_use_custom_cpu_ticks(
                    json["general"]["use_custom_cpu_ticks"].get<bool>());
            }
            if (json["general"].count("custom_cpu_ticks")) {
                vvctre_settings_set_custom_cpu_ticks(
                    json["general"]["custom_cpu_ticks"].get<u64>());
            }
            if (json["general"].count("cpu_clock_percentage")) {
                vvctre_settings_set_cpu_clock_percentage(
                    json["general"]["cpu_clock_percentage"].get<u32>());
            }
        }
        if (json.count("audio")) {
            if (json["audio"].count("output")) {
                if (json["audio"]["output"].count("volume")) {
                    vvctre_settings_set_audio_volume(
                        json["audio"]["output"]["volume"].get<float>());
                }
            }
        }
        if (json.count("graphics")) {
            if (json["graphics"].count("resolution")) {
                if (json["graphics"]["resolution"].is_string() &&
                    json["graphics"]["resolution"].get<std::string>() == "Window Size") {
                    vvctre_settings_set_resolution(0);
                } else {
                    vvctre_settings_set_resolution(json["graphics"]["resolution"].get<u16>());
                }
            }
            if (json["graphics"].count("post_processing_shader")) {
                vvctre_settings_set_post_processing_shader(
                    json["graphics"]["post_processing_shader"].get<std::string>().c_str());
            }
            if (json["graphics"].count("texture_filter")) {
                vvctre_settings_set_texture_filter(
                    json["graphics"]["texture_filter"].get<std::string>().c_str());
            }
            if (json["graphics"].count("3d_mode")) {
                const std::string graphics_3d_mode = json["graphics"]["3d_mode"].get<std::string>();
                if (graphics_3d_mode == "Off") {
                    vvctre_settings_set_render_3d(0);
                } else if (graphics_3d_mode == "Side by Side") {
                    vvctre_settings_set_render_3d(1);
                } else if (graphics_3d_mode == "Anaglyph") {
                    vvctre_settings_set_render_3d(2);
                } else if (graphics_3d_mode == "Interlaced") {
                    vvctre_settings_set_render_3d(3);
                } else if (graphics_3d_mode == "Reverse Interlaced") {
                    vvctre_settings_set_render_3d(4);
                }
            }
            if (json["graphics"].count("3d_factor")) {
                vvctre_settings_set_factor_3d(json["graphics"]["3d_factor"].get<u8>());
            }
        }
        if (json.count("layout")) {
            if (json["layout"].count("layout")) {
                const std::string layout = json["layout"]["layout"].get<std::string>();
                if (layout == "Default") {
                    vvctre_settings_set_layout(0);
                } else if (layout == "Single Screen") {
                    vvctre_settings_set_layout(1);
                } else if (layout == "Large Screen") {
                    vvctre_settings_set_layout(2);
                } else if (layout == "Side by Side") {
                    vvctre_settings_set_layout(3);
                } else if (layout == "Medium Screen") {
                    vvctre_settings_set_layout(4);
                }
            }
            if (json["layout"].count("use_custom_layout")) {
                vvctre_settings_set_use_custom_layout(
                    json["layout"]["use_custom_layout"].get<bool>());
            }
            if (json["layout"].count("swap_screens")) {
                vvctre_settings_set_swap_screens(json["layout"]["swap_screens"].get<bool>());
            }
            if (json["layout"].count("upright_screens")) {
                vvctre_settings_set_upright_screens(json["layout"]["upright_screens"].get<bool>());
            }
        }
    }
}

VVCTRE_PLUGIN_EXPORT void EmulatorClosing() {
    if (!json["write_when_emulator_is_closing"].get<bool>()) {
        return;
    }

    int length = wai_getExecutablePath(nullptr, 0, nullptr);
    std::string vvctre_folder(length, '\0');
    int dirname_length = 0;
    wai_getExecutablePath(&vvctre_folder[0], length, &dirname_length);
    vvctre_folder = vvctre_folder.substr(0, dirname_length);

    std::ofstream file;
#ifdef _MSC_VER
    file.open(Common::UTF8ToUTF16W(vvctre_folder + "\\quick-settings.json"), std::ofstream::trunc);
#else
    file.open(vvctre_folder + "/quick-settings.json", std::ofstream::trunc);
#endif

    if (!file.fail()) {
        if (json.count("general")) {
            if (json["general"].count("limit_speed")) {
                json["general"]["limit_speed"] = vvctre_settings_get_limit_speed();
            }
            if (json["general"].count("speed_limit")) {
                json["general"]["speed_limit"] = vvctre_settings_get_speed_limit();
            }
            if (json["general"].count("use_custom_cpu_ticks")) {
                json["general"]["use_custom_cpu_ticks"] =
                    vvctre_settings_get_use_custom_cpu_ticks();
            }
            if (json["general"].count("custom_cpu_ticks")) {
                json["general"]["custom_cpu_ticks"] = vvctre_settings_get_custom_cpu_ticks();
            }
            if (json["general"].count("cpu_clock_percentage")) {
                json["general"]["cpu_clock_percentage"] =
                    vvctre_settings_get_cpu_clock_percentage();
            }
        }
        if (json.count("audio")) {
            if (json["audio"].count("output")) {
                if (json["audio"]["output"].count("volume")) {
                    json["audio"]["output"]["volume"] = vvctre_settings_get_audio_volume();
                }
            }
        }
        if (json.count("graphics")) {
            if (json["graphics"].count("resolution")) {
                const u16 resolution = vvctre_settings_get_resolution();
                if (resolution == 0) {
                    json["graphics"]["resolution"] = "Window Size";
                } else {
                    json["graphics"]["resolution"] = resolution;
                }
            }
            if (json["graphics"].count("post_processing_shader")) {
                json["graphics"]["post_processing_shader"] =
                    std::string(vvctre_settings_get_post_processing_shader());
            }
            if (json["graphics"].count("texture_filter")) {
                json["graphics"]["texture_filter"] = vvctre_settings_get_texture_filter();
            }
            if (json["graphics"].count("3d_mode")) {
                switch (vvctre_settings_get_render_3d()) {
                case 0:
                    json["graphics"]["3d_mode"] = "Off";
                    break;
                case 1:
                    json["graphics"]["3d_mode"] = "Side by Side";
                    break;
                case 2:
                    json["graphics"]["3d_mode"] = "Anaglyph";
                    break;
                case 3:
                    json["graphics"]["3d_mode"] = "Interlaced";
                    break;
                case 4:
                    json["graphics"]["3d_mode"] = "Reverse Interlaced";
                    break;
                default:
                    break;
                }
            }
            if (json["graphics"].count("3d_factor")) {
                json["graphics"]["3d_factor"] = vvctre_settings_get_factor_3d();
            }
        }
        if (json.count("layout")) {
            if (json["layout"].count("layout")) {
                switch (vvctre_settings_get_layout()) {
                case 0:
                    json["layout"]["layout"] = "Default";
                    break;
                case 1:
                    json["layout"]["layout"] = "Single Screen";
                    break;
                case 2:
                    json["layout"]["layout"] = "Large Screen";
                    break;
                case 3:
                    json["layout"]["layout"] = "Side by Side";
                    break;
                case 4:
                    json["layout"]["layout"] = "Medium Screen";
                    break;
                default:
                    break;
                }
            }
            if (json["layout"].count("use_custom_layout")) {
                json["layout"]["use_custom_layout"] = vvctre_settings_get_use_custom_layout();
            }
            if (json["layout"].count("swap_screens")) {
                json["layout"]["swap_screens"] = vvctre_settings_get_swap_screens();
            }
            if (json["layout"].count("upright_screens")) {
                json["layout"]["upright_screens"] = vvctre_settings_get_upright_screens();
            }
        }
        file << json.dump(2);
    }
}
