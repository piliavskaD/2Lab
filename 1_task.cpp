#include <windows.h> 
#include <math.h>    
#include <string>    


BOOL RegClass(WNDPROC, LPCTSTR, UINT);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


HINSTANCE hInstance;
wchar_t szClassName[] = L"GraphClass_Var17";


struct FuncData {
    wchar_t name[30] = L"y = 0.5 - cos^2(1+x)";
    float x[100];
    float y[100];
} myFunc;




int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow) {
    MSG msg;
    HWND hwnd;
    hInstance = hInst;

    if (!RegClass(WndProc, szClassName, COLOR_WINDOW))
        return FALSE;

    hwnd = CreateWindow(
        szClassName,
        L"Завдання 1: Графік функції (Варіант 17)",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        0, 0,
        hInstance,
        NULL
    );

    if (!hwnd) return FALSE;

    while (GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}



BOOL RegClass(WNDPROC Proc, LPCTSTR szName, UINT brBackground) {
    WNDCLASS wc = { 0 };
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = Proc;
    wc.hInstance = hInstance;
    wc.lpszClassName = szName;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(brBackground + 1);
    return (RegisterClass(&wc) != 0);
}



LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static short cx, cy;

    switch (msg) {

    case WM_SIZE:
        cx = LOWORD(lParam);
        cy = HIWORD(lParam);
        return 0;

    case WM_CREATE: {
        float xMin = 0.0f;
        float xMax = 2.5f;
        float step = (xMax - xMin) / 99.0f;

        for (int i = 0; i < 100; i++) {
            myFunc.x[i] = xMin + i * step;
            float cos_val = cosf(1.0f + myFunc.x[i]);
            myFunc.y[i] = 0.5f - (cos_val * cos_val);
        }
        return 0;
    }

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);


        int offsetLeft = cx / 10;
        int offsetRight = cx / 10;
        int offsetTop = cy / 10;
        int offsetBottom = cy / 10;

        int graphWidth = cx - offsetLeft - offsetRight;
        int graphHeight = cy - offsetTop - offsetBottom;


        float xMin = myFunc.x[0], xMax = myFunc.x[99];
        float yMin = myFunc.y[0], yMax = myFunc.y[0];
        for (int i = 1; i < 100; i++) {
            if (myFunc.y[i] < yMin) yMin = myFunc.y[i];
            if (myFunc.y[i] > yMax) yMax = myFunc.y[i];
        }

        float yRange = yMax - yMin;
        if (yRange == 0) yRange = 1.0f;
        yMin -= yRange * 0.1f;
        yMax += yRange * 0.1f;

        float kx = (float)graphWidth / (xMax - xMin);
        float ky = (float)graphHeight / (yMax - yMin);


        auto GetScreenX = [&](float x_val) { return offsetLeft + (int)((x_val - xMin) * kx); };
        auto GetScreenY = [&](float y_val) { return offsetTop + graphHeight - (int)((y_val - yMin) * ky); };

        int axisX_Y = (yMin <= 0.0f && yMax >= 0.0f) ? GetScreenY(0.0f) : (offsetTop + graphHeight);
        int axisY_X = (xMin <= 0.0f && xMax >= 0.0f) ? GetScreenX(0.0f) : offsetLeft;


        HPEN axisPen = CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
        HPEN oldPen = (HPEN)SelectObject(hdc, axisPen);


        MoveToEx(hdc, offsetLeft, axisX_Y, NULL);
        LineTo(hdc, cx - offsetRight + 15, axisX_Y);
        LineTo(hdc, cx - offsetRight, axisX_Y - 5);
        MoveToEx(hdc, cx - offsetRight + 15, axisX_Y, NULL);
        LineTo(hdc, cx - offsetRight, axisX_Y + 5);


        MoveToEx(hdc, axisY_X, cy - offsetBottom, NULL);
        LineTo(hdc, axisY_X, offsetTop - 15);
        LineTo(hdc, axisY_X - 5, offsetTop);
        MoveToEx(hdc, axisY_X, offsetTop - 15, NULL);
        LineTo(hdc, axisY_X + 5, offsetTop);


        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(0, 0, 0));

        for (int i = 0; i <= 5; i++) {
            float xVal = xMin + i * (xMax - xMin) / 5.0f;
            int sx = GetScreenX(xVal);
            MoveToEx(hdc, sx, axisX_Y - 4, NULL);
            LineTo(hdc, sx, axisX_Y + 4);

            std::wstring text = std::to_wstring(xVal).substr(0, 4);
            TextOut(hdc, sx - 10, axisX_Y + 8, text.c_str(), (int)text.length());
        }

        for (int i = 0; i <= 5; i++) {
            float yVal = yMin + i * (yMax - yMin) / 5.0f;
            int sy = GetScreenY(yVal);
            MoveToEx(hdc, axisY_X - 4, sy, NULL);
            LineTo(hdc, axisY_X + 4, sy);

            std::wstring text = std::to_wstring(yVal).substr(0, 4);
            TextOut(hdc, axisY_X - 35, sy - 6, text.c_str(), (int)text.length());
        }

        TextOut(hdc, cx - offsetRight + 20, axisX_Y - 5, L"X", 1);
        TextOut(hdc, axisY_X - 15, offsetTop - 25, L"Y", 1);
        TextOut(hdc, offsetLeft, offsetTop / 2, myFunc.name, (int)wcslen(myFunc.name));


        HPEN graphPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
        SelectObject(hdc, graphPen);

        MoveToEx(hdc, GetScreenX(myFunc.x[0]), GetScreenY(myFunc.y[0]), NULL);
        for (int i = 1; i < 100; i++) {
            LineTo(hdc, GetScreenX(myFunc.x[i]), GetScreenY(myFunc.y[i]));
        }


        SelectObject(hdc, oldPen);
        DeleteObject(axisPen);
        DeleteObject(graphPen);

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
