#ifndef XBOX
#error Must be built with nxdk
#endif

#include <SDL.h>
#include <hal/debug.h>
#include <hal/video.h>
#include <pbkit/pbkit.h>
#include <windows.h>

#include <cassert>
#include <string>
#include <vector>

#include "logger.h"
#include "mmx_tests/mmx_test_common.h"
#include "mmx_tests/test_mmx_arithmetic.h"
#include "mmx_tests/test_mmx_extensions.h"
#include "mmx_tests/test_mmx_logical_compare_shift.h"
#include "mmx_tests/test_mmx_pack_unpack.h"
#include "mmx_tests/test_mmx_x87_aliasing.h"
#include "nv2astate.h"
#include "test_suite.h"
#include "test_util.h"

static const std::string kLogPath =
    R"(e:\devkit\nxdk_low_level_cpu_tests\mmx_log.txt)";

static constexpr TestCase kTests[] = {
    TestCase::From<TestMmxArithmetic>(),
    TestCase::From<TestMmxLogicalCompareShift>(),
    TestCase::From<TestMmxPackUnpack>(),
    TestCase::From<TestMmxExtensions>(),
    TestCase::From<TestMmxX87Aliasing>(),
};

static void RenderLogScreen() {
  pb_wait_for_vbl();
  pb_reset();
  pb_target_back_buffer();
  ClearScreen(0);
  pb_print("  Xbox CPU (MMX) Validation Suite\n");
  pb_print("============================================================\n\n");

  const auto& log = GetOnScreenLog();
  for (const auto& line : log) {
    pb_print("%s\n", line.c_str());
  }

  pb_draw_text_screen();
  PBKitPlusPlus::NV2AState::FinishDraw();
}

static void RunAllTests() {
  std::vector<bool> results;
  results.reserve(std::size(kTests));

  for (const auto& test : kTests) {
    bool result = test.test_function();
    results.push_back(result);
    RenderLogScreen();
  }

  pb_erase_text_screen();
  ClearOnScreenLog();

  for (size_t i = 0; i < std::size(kTests); ++i) {
    LogMsg("  %2d %-32s : %s\n", i + 1, kTests[i].name,
           results[i] ? "PASS" : "FAIL");
  }
  LogMsg("See %s\n\n", kLogPath.c_str());
}

int main() {
  debugPrint("Setting video mode...\n");
  if (!XVideoSetMode(kFramebufferWidth, kFramebufferHeight, kBitsPerPixel,
                     REFRESH_DEFAULT)) {
    debugPrint("Failed to set video mode\n");
    Sleep(2000);
    return 1;
  }

  int status = pb_init();
  if (status) {
    debugPrint("pb_init Error %d\n", status);
    Sleep(2000);
    return 1;
  }

  debugPrint("Initializing SDL Game Controller...\n");
  if (SDL_Init(SDL_INIT_GAMECONTROLLER)) {
    debugPrint("Failed to initialize SDL: %s\n", SDL_GetError());
  }

  debugPrint("Mounting Drive E: for log output...\n");
  if (EnsureDriveMounted('E', false)) {
    size_t last_slash = kLogPath.find_last_of("\\/");
    if (last_slash != std::string::npos) {
      EnsureFolderExists(kLogPath.substr(0, last_slash));
    }
    Logger::Initialize(kLogPath, true);
    LogMsg("Mounted Drive E: and initialized log at %s\n", kLogPath.c_str());
  } else {
    DbgPrint("WARNING: Failed to mount Drive E:! Disk logging disabled.\n");
  }

  pb_show_front_screen();
  debugClearScreen();

  pb_wait_for_vbl();
  pb_reset();
  pb_target_back_buffer();
  ClearScreen(0);
  pb_print("  Xbox CPU (MMX) Validation Suite\n");
  pb_print("============================================================\n\n");
  pb_print("Testing MMX instructions...\n");
  pb_draw_text_screen();
  PBKitPlusPlus::NV2AState::FinishDraw();

  LogMsg("Running MMX test suite...\n");
  RunAllTests();

  bool running = true;
  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_CONTROLLERDEVICEADDED: {
          SDL_GameControllerOpen(event.cdevice.which);
        } break;
        case SDL_CONTROLLERDEVICEREMOVED: {
          SDL_GameController* controller =
              SDL_GameControllerFromInstanceID(event.cdevice.which);
          if (controller) SDL_GameControllerClose(controller);
        } break;
        case SDL_CONTROLLERBUTTONUP: {
          if (event.cbutton.state == SDL_RELEASED) {
            auto btn =
                static_cast<SDL_GameControllerButton>(event.cbutton.button);

            if (btn == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER ||
                btn == SDL_CONTROLLER_BUTTON_START) {
              running = false;
            }
          }
        } break;
        default:
          break;
      }
    }

    pb_wait_for_vbl();
    pb_reset();
    pb_target_back_buffer();
    ClearScreen(0);

    pb_print("  Xbox CPU (MMX) Validation Suite\n");
    pb_print("Log file: %s\n\n", kLogPath.c_str());

    const auto& log = GetOnScreenLog();
    for (const auto& line : log) {
      pb_print("%s\n", line.c_str());
    }

    pb_print("\n[Black] Exit\n");

    pb_draw_text_screen();
    PBKitPlusPlus::NV2AState::FinishDraw();
  }

  pb_kill();
  return 0;
}
