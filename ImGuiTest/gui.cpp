// Dear ImGui: standalone example application for DirectX 9

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp
#pragma execution_character_set("utf-8")

#include "imgui.h"
#include "vendor/imgui/imgui_internal.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <iostream>
#include <mutex>
#include <fstream>
#include <chrono>
#include "utils.h"


// Data
static LPDIRECT3D9              g_pD3D = nullptr;
static LPDIRECT3DDEVICE9        g_pd3dDevice = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};


// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

std::string currentRequestData;
std::string currentResponseData;

struct TableItem {
    std::string time;
    std::string code;
    std::string status;
    std::string link;
    std::string requestData;
    std::string responseData;
};

std::vector<TableItem> tableData = {
    
};

std::atomic_bool isRunning = false;
std::atomic_bool isDone = false;
std::mutex mux;

// Main code
bool init(HMODULE hModule)
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Tls Client Sniffer", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\Arial.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    IM_ASSERT(font != nullptr);

    isRunning = true;

    // Main loop
    //bool done = false;
    // Our state
    isDone = false;
    bool show_demo_window = false;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    while (!isDone)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT) {
                isDone = true;
                isRunning = false;
            }
        }
        if (isDone)
            break;

        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            g_d3dpp.BackBufferWidth = g_ResizeWidth;
            g_d3dpp.BackBufferHeight = g_ResizeHeight;
            g_ResizeWidth = g_ResizeHeight = 0;
            ResetDevice();
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y));
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::Begin("Selectable Table", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

        if (ImGui::Button("Очистить Историю", ImVec2(-1.0f, 25.f))) {

            std::scoped_lock<std::mutex> lock(mux);
            {
                tableData.clear();
                currentRequestData = "";
                currentResponseData = "";
            }
        }

        // Начинаем таблицу с тремя столбцами
        if (ImGui::BeginTable("##table", 4, ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX)) {
            // Устанавливаем ширину столбцов
            ImGui::TableSetupColumn("Время");
            ImGui::TableSetupColumn("Статус");
            ImGui::TableSetupColumn("Метод");
            ImGui::TableSetupColumn("Ссылка");
            ImGui::TableHeadersRow();

            // Заполняем таблицу данными
            std::scoped_lock<std::mutex> lock(mux);
            {
                for (int i = 0; i < tableData.size(); i++) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", tableData[i].time.c_str());
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", tableData[i].code.c_str());
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", tableData[i].status.c_str());
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", tableData[i].link.c_str());

                    if (ImGui::IsItemHovered()) {
                        ImDrawList* drawList = ImGui::GetWindowDrawList();
                        ImVec2 min = ImGui::GetItemRectMin();
                        ImVec2 max = ImGui::GetItemRectMax();
                        drawList->AddRect(min, max, IM_COL32(255, 255, 0, 255));
                    }

                    // Обрабатываем клик на элемент таблицы
                    if (ImGui::IsItemClicked()) {
                        currentRequestData = tableData[i].requestData;
                        currentResponseData = tableData[i].responseData;
                    }

                    if (ImGui::IsItemHovered()) {
                        ImGui::PopStyleColor();
                        ImGui::PopStyleVar();
                    }
                }
            }
            ImGui::EndTable();
        }

        ImGuiIO& io = ImGui::GetIO();
        ImVec2 windowSize = io.DisplaySize;

        ImGui::SetNextWindowPos(ImVec2(windowSize.x - windowSize.x / 2, 0));
        ImGui::SetNextWindowSize(ImVec2(windowSize.x / 2, windowSize.y / 2));
        ImGui::Begin("Panel 1", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
        ImGui::Text("Запрос Клиента");

        const char* charArray = currentRequestData.c_str();

        ImGui::InputTextMultiline("##source", const_cast<char*>(charArray), currentRequestData.size() + 1, ImVec2(-1.0f, -1.0f));

        if (ImGui::Button("Открыть в Блокноте", ImVec2(-1.0f, 25.f))) {
            std::scoped_lock<std::mutex> lock(mux);
            {
                std::filesystem::path tmpPath = std::filesystem::temp_directory_path();
                std::ofstream out;
                std::string filePath = tmpPath.string() + "worker.txt";
                out.open(filePath);
                if (out.is_open()) {
                    out << currentRequestData.c_str() << std::endl;
                }
                out.close();

                std::string command = "notepad.exe " + filePath;
                std::thread([command]() {
                    std::system(command.c_str());
                    }
                ).detach();
            }
        }

        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(windowSize.x - windowSize.x / 2, windowSize.y / 2 + 1));
        ImGui::SetNextWindowSize(ImVec2(windowSize.x / 2, windowSize.y / 2));
        ImGui::Begin("Panel 2", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
        ImGui::Text("Ответ от Сервера");

        const char* charArrayRes = currentResponseData.c_str();
        ImGui::InputTextMultiline("##source", const_cast<char*>(charArrayRes), currentResponseData.size() + 1, ImVec2(-1.0f, -1.0f));
        
        if (ImGui::Button("Открыть в Блокноте", ImVec2(-1.0f, 25.f))) {
            std::scoped_lock<std::mutex> lock(mux);
            {
                std::filesystem::path tmpPath = std::filesystem::temp_directory_path();
                std::ofstream out;
                std::string filePath = tmpPath.string() + "worker.txt";
                out.open(filePath);
                if (out.is_open()) {
                    out << currentResponseData.c_str() << std::endl;
                }
                out.close();

                std::string command = "notepad.exe " + filePath;
                std::thread([command]() {
                        std::system(command.c_str());
                    }
                ).detach();
            }
        }

        ImGui::End();

        ImGui::End();


        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // Rendering
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * clear_color.w * 255.0f), (int)(clear_color.y * clear_color.w * 255.0f), (int)(clear_color.z * clear_color.w * 255.0f), (int)(clear_color.w * 255.0f));
        g_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    isRunning = false;
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = nullptr; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}


std::string formatHeaders(const nlohmann::json& headers) {
    std::string formattedHeaders;
    for (auto it = headers.begin(); it != headers.end(); ++it) {
        formattedHeaders += it.key() + ": " + it.value().get<std::string>() + "\n";
    }
    return formattedHeaders;
}

bool startGui() {
    if (isRunning)
        return false;

    std::thread([]() {
        init(nullptr);
    }).detach();

    return true;
}

bool stopGui() {
    if (!isRunning)
        return false;

    isDone = true;
    return true;
}

bool addHttpData(char* requestData, char* responseData) {
    if (!isRunning) {
        return false;
    }

    nlohmann::json requestDoc = nlohmann::json::parse(requestData);

    if (!requestDoc.is_object()) {
        return false;
    }

    std::string requestUrl = getValueOrDefault(requestDoc, "requestUrl", "<No Url>");
    std::string requestMethod = getValueOrDefault(requestDoc, "requestMethod", "<No Method>");
    std::string requestBody = getValueOrDefault(requestDoc, "requestBody", "<No Body>");
    std::string requestHeaders = getValueOrDefault(requestDoc, "requestHeaders", "<No Headers>");

    if (requestDoc.find("headers") != requestDoc.end()) {
        const nlohmann::json& headers = requestDoc["headers"];
        requestHeaders = "";

        for (auto& header : headers.items()) {
            requestHeaders += header.key() + ": " + header.value().get <std::string>() + "\n";
        }
    }

    std::string requestFormatedString = requestUrl + "\n\n" + requestHeaders + "\n\n" + requestBody;

    nlohmann::json responseDoc = nlohmann::json::parse(responseData);

    if (!responseDoc.is_object()) {
        return false;
    }

    std::string responseUrl = getValueOrDefault(responseDoc, "target", "<No Url>");
    std::string responseBody = getValueOrDefault(responseDoc, "body", "<No Body>");

    if (responseBody.find(";base64,") != std::string::npos) {
        std::string base64Body = splitString(responseBody, ";base64,").at(1);

        std::vector<BYTE> decodeBase64Body = base64_decode(base64Body);

        responseBody = std::string(decodeBase64Body.begin(), decodeBase64Body.end());
    }

    std::string responseStatus = std::to_string(responseDoc["status"].get<int>());
    std::string responseProtocol = getValueOrDefault(responseDoc, "usedProtocol", "<No Protocol>");
    std::string responseHeaders = "<No Headers>";

    if (responseDoc.find("headers") != responseDoc.end()) {
        const nlohmann::json& headers = responseDoc["headers"];
        responseHeaders = "";

        for (auto& header : headers.items()) {
            if (header.value().size() > 1) {
                for (const auto& it : header.value()) {
                    responseHeaders += header.key() + ": " + it.get<std::string>() + "\n";
                }

            }
            else {
                responseHeaders += header.key() + ": " + header.value()[0].get<std::string>() + "\n";
            }
        }
    }

    std::string responseFormatedString = responseProtocol + " " + responseUrl + "\n\n" + responseHeaders + "\n\n" + responseBody;

    std::scoped_lock<std::mutex> lock(mux);
    {
        auto now = std::chrono::system_clock::now();
        time_t now_c = std::chrono::system_clock::to_time_t(now);
        struct tm local_time;
        localtime_s(&local_time, &now_c);

        int hour = local_time.tm_hour;
        int minute = local_time.tm_min;
        std::string minuteStr = std::to_string(minute);
        std::string hourStr = std::to_string(hour);

        if (hour < 10) {
            hourStr = "0" + hourStr;
        }
        if (minute < 10) {
            minuteStr = "0" + minuteStr;
        }

        tableData.push_back({hourStr + ":" + minuteStr, responseStatus, requestMethod, requestUrl, requestFormatedString, responseFormatedString});

        if (tableData.size() > 100) {
            tableData.erase(tableData.begin());
        }
    }

    return true;
}
