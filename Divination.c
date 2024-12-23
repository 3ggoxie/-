#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>

#define ALL 49
#define MAN 1

typedef struct
{
    long x;
    long y;
} MousePos;

int modv(int sky, int earth);
int fate(int remain);

int a[4] = {4, 1, 2, 3};
MousePos capturedPos = {0, 0};
bool isCaptured = false;

int main()
{
    char input[10];
    while (1)
    {
        printf("请输入操作命令（输入 'y' 开始占卜，或输入 'n' 停止程序）：");
        scanf("%s", input);

        if (strcmp(input, "n") == 0)
        {
            printf("程序已停止。\n");
            break;
        }
        int sky, earth;

        for (int i = 0; i < 6; i++)
        {
            int extra = 0;
            int yao = 3;
            int remain = ALL;

            while (yao > 0)
            {
                sky = fate(remain);
                isCaptured = false;
                earth = remain - sky - MAN;
                extra += modv(sky, earth);
                remain -= modv(sky, earth);
                // printf("%3d%3d%3d%3d\n", sky, earth, extra, remain);
                yao--;
            }
            printf("第%2d次结果为%3d\n", i + 1, remain / 4);
            // printf("\n");
        }
        printf("\n----------------------这是一条分隔线----------------------\n\n");
    }
    return 0;
}
int modv(int sky, int earth)
{

    return a[sky % 4] + a[earth % 4] + MAN;
}

LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION && !isCaptured)
    { // 仅处理一次点击
        MSLLHOOKSTRUCT *mouse = (MSLLHOOKSTRUCT *)lParam;

        if (wParam == WM_LBUTTONDOWN)
        {
            // printf("捕获的鼠标位置: x=%ld, y=%ld\n", mouse->pt.x, mouse->pt.y);
            capturedPos.x = mouse->pt.x;
            capturedPos.y = mouse->pt.y;
            isCaptured = true; // 设置已捕获标志
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam); // 传递事件
}
int fate(int remain)
{
    // 安装鼠标钩子
    HHOOK mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, NULL, 0);
    if (mouseHook == NULL)
    {
        printf("无法安装鼠标钩子！\n");
        return 1;
    }

    printf("请在任意位置点击鼠标左键模拟信手一分...\n");

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

    // printf("鼠标位置: x=%ld, y=%ld\n", capturedPos.x, capturedPos.y);
    return capturedPos.x * remain / 1920;
}
