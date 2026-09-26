#include <windows.h>


class Sun {
public:
    void show(HDC dc, int X, int Y, int radius) {
        HBRUSH brush = CreateSolidBrush(RGB(255, 255, 0));
        HGDIOBJ oldBrush = SelectObject(dc, brush);

        Ellipse(dc, X - radius, Y - radius, X + radius, Y + radius);

        SelectObject(dc, oldBrush);
        DeleteObject(brush);
    }
};


class Lake {
public:
    void show(HDC dc, int X, int Y, int width, int height) {
        HBRUSH brush = CreateSolidBrush(RGB(0, 0, 255));
        HGDIOBJ oldBrush = SelectObject(dc, brush);

        Ellipse(dc, X, Y, X + width, Y + height);

        SelectObject(dc, oldBrush);
        DeleteObject(brush);
    }
};


class Tree {
public:
    void show(HDC dc, int X, int Y) {
        HBRUSH trunkBrush = CreateSolidBrush(RGB(139, 69, 19));
        HGDIOBJ oldBrush = SelectObject(dc, trunkBrush);

        Rectangle(dc, X - 10, Y - 50, X + 10, Y);

        HBRUSH leavesBrush = CreateSolidBrush(RGB(0, 128, 0));
        SelectObject(dc, leavesBrush);

        Ellipse(dc, X - 35, Y - 100, X + 35, Y - 40);

        SelectObject(dc, oldBrush);
        DeleteObject(trunkBrush);
        DeleteObject(leavesBrush);
    }
};



class Fence {
public:
    void show(HDC dc, int X, int Y, int numElements) {
        HBRUSH brush = CreateSolidBrush(RGB(210, 180, 140));
        HGDIOBJ oldBrush = SelectObject(dc, brush);

        int elementWidth = 10;  
        int elementHeight = 40; 
        int gap = 5;            

        for (int i = 0; i < numElements; i++) {
            int currentX = X + i * (elementWidth + gap);
            Rectangle(dc, currentX, Y, currentX + elementWidth, Y + elementHeight);
        }

        MoveToEx(dc, X, Y + 10, NULL);
        LineTo(dc, X + numElements * (elementWidth + gap) - gap, Y + 10);

        MoveToEx(dc, X, Y + 30, NULL);
        LineTo(dc, X + numElements * (elementWidth + gap) - gap, Y + 30);

        SelectObject(dc, oldBrush);
        DeleteObject(brush);
    }
};


void DrawScene(HDC dc, int width, int height) {
    HBRUSH bgBrush = CreateSolidBrush(RGB(0, 200, 0));
    HGDIOBJ oldBrush = SelectObject(dc, bgBrush);
    Rectangle(dc, 0, 0, width, height);

    HBRUSH skyBrush = CreateSolidBrush(RGB(135, 206, 250));
    SelectObject(dc, skyBrush);
    Rectangle(dc, 0, 0, width, height / 3);

    SelectObject(dc, oldBrush);
    DeleteObject(bgBrush);
    DeleteObject(skyBrush);

    Sun sun;
    sun.show(dc, width - 100, 60, 35);

    Lake lake;
    lake.show(dc, width / 2 - 150, height - 180, 300, 100);

    Tree tree;
    tree.show(dc, 100, height / 3 + 50);
    tree.show(dc, 220, height / 3 + 50);
    tree.show(dc, width - 120, height / 3 + 50);

    Fence fence;
    fence.show(dc, 30, height - 100, 10);       
    fence.show(dc, width - 200, height - 100, 12); 
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC dc = BeginPaint(hwnd, &ps);

        RECT rect;
        GetClientRect(hwnd, &rect);
        int width = rect.right;
        int height = rect.bottom;

        DrawScene(dc, width, height);

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_LBUTTONDOWN:
        MessageBox(hwnd, L"Клік мишкою!", L"Повідомлення", MB_OK);
        return 0;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            DestroyWindow(hwnd);
        }
        return 0;

    case WM_SIZE:
        InvalidateRect(hwnd, nullptr, TRUE);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"SimpleCompositionClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, CLASS_NAME, L"Завдання 2: Спрощена композиція",
        WS_OVERLAPPEDWINDOW,
        100, 100, 800, 600, nullptr, nullptr, hInstance, nullptr);

    if (hwnd == nullptr) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG message = {};
    while (GetMessage(&message, nullptr, 0, 0) > 0) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
    return 0;
}
