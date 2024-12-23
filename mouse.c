#include <stdio.h>
#include <windows.h>
#include <stdbool.h>

// 定义一个结构体来存储鼠标坐标
typedef struct
{
    long x;
    long y;
} MousePos;

// 全局变量，用于存储捕获到的鼠标坐标
MousePos capturedPos = {0, 0};
bool isCaptured = false; // 标记是否已捕获鼠标位置

// 鼠标钩子回调函数
LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION && !isCaptured)
    { // 仅处理一次点击
        MSLLHOOKSTRUCT *mouse = (MSLLHOOKSTRUCT *)lParam;

        if (wParam == WM_LBUTTONDOWN)
        {
            capturedPos.x = mouse->pt.x;
            capturedPos.y = mouse->pt.y;
            isCaptured = true; // 设置已捕获标志
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam); // 传递事件
}

int main()
{
    // 安装鼠标钩子
    HHOOK mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, NULL, 0);
    if (mouseHook == NULL)
    {
        printf("无法安装鼠标钩子！\n");
        return 1;
    }

    printf("请点击鼠标左键以捕获位置...\n");

    // 消息循环
    MSG msg;
    while (!isCaptured)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    // 卸载鼠标钩子
    UnhookWindowsHookEx(mouseHook);

    printf("捕获的鼠标位置: x=%ld, y=%ld\n", capturedPos.x, capturedPos.y);
    printf("位置已捕获，程序退出。\n");

    return 0;
}
