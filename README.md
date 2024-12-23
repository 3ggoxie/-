# 蓍草六爻占卜法模拟程序

## 简介
蓍草六爻占卜法是中国古代的一种占卜方式，通过使用蓍草推演卦象，以预测未来或回答问题。本项目使用 C 语言实现蓍草占卜法的基本流程，并通过屏幕点击来模拟手分蓍草棒的过程。点击横向坐标的位置将决定每次分组的方式。

## 使用说明
### 编译与运行
确保系统上已安装 C 编译器，如 GCC 和支持 Windows 平台的开发环境。
```bash
gcc Divination.c  -o div
./div.exe
```

### 操作步骤
1. 运行程序后，输入 `y` 开始占卜。
2. 程序提示“请在任意位置点击鼠标左键模拟信手一分”，点击屏幕任意位置。
3. 鼠标点击的横向坐标将用于计算分组，完成六次操作后生成完整的卦象。
4. 输入 `n` 可退出程序。

### 核心功能
1. **鼠标点击获取分组**
   - 程序通过捕获鼠标左键点击事件获取屏幕横向位置。
   - 使用位置与总数的比例模拟随机分组。

2. **余数计算**
   - 根据蓍草规则计算每次操作的余数。

3. **卦爻生成**
   - 累积六次分组结果，生成完整的卦象。

### 示例输出
运行程序后，输出示例：
```plaintext
请输入操作命令（输入 'y' 开始占卜，或输入 'n' 停止程序）：y
请在任意位置点击鼠标左键模拟信手一分...
第 1 次结果为 8
第 2 次结果为 7
第 3 次结果为 6
第 4 次结果为 9
第 5 次结果为 8
第 6 次结果为 7
----------------------这是一条分隔线----------------------
```

## 源码简述

### 主程序逻辑
```c
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

        for (int i = 0; i < 6; i++)
        {
            int yao = 3;
            int remain = ALL;

            while (yao > 0)
            {
                int sky = fate(remain);
                isCaptured = false;
                int earth = remain - sky - MAN;
                remain -= modv(sky, earth);
                yao--;
            }
            printf("第%2d次结果为%3d\n", i + 1, remain / 4);
        }
        printf("\n----------------------这是一条分隔线----------------------\n\n");
    }
    return 0;
}
```

### 鼠标点击获取
```c
LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION && !isCaptured)
    {
        MSLLHOOKSTRUCT *mouse = (MSLLHOOKSTRUCT *)lParam;

        if (wParam == WM_LBUTTONDOWN)
        {
            capturedPos.x = mouse->pt.x;
            capturedPos.y = mouse->pt.y;
            isCaptured = true;
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int fate(int remain)
{
    HHOOK mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, NULL, 0);
    if (mouseHook == NULL)
    {
        printf("无法安装鼠标钩子！\n");
        return 1;
    }

    printf("请在任意位置点击鼠标左键模拟信手一分...\n");

    MSG msg;
    while (!isCaptured)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    UnhookWindowsHookEx(mouseHook);

    return capturedPos.x * remain / 1920;
}
```

### 余数计算
```c
int modv(int sky, int earth)
{
    int a[4] = {4, 1, 2, 3};
    return a[sky % 4] + a[earth % 4] + MAN;
}
```

## 注意事项
- 程序仅支持 Windows 平台，使用了 Windows 的鼠标钩子功能。
- 鼠标点击位置在整个屏幕范围内，否则可能导致异常结果。

---

**参考文献**
- 《易经》
