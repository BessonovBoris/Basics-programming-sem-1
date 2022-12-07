#include <stdio.h>
#include <windows.h>

#define MAX_ELEMENTS 500
#define WIDTH_EL 20
#define WIDTH 640
#define HEIGHT 480

RECT rects_before[MAX_ELEMENTS];
RECT rects_after[MAX_ELEMENTS];

int left_bin_search(int key) {
    int left = 0;
    int right = MAX_ELEMENTS - 1;
    int mid;

    while (right >= left) {
        mid = (left + right) / 2;

        if (rects_before[mid].left >= key || rects_before[mid].left == -999)
            right = mid - 1;
        else
            left = mid + 1;
    }

    return right + 1;
}

int right_bin_search(int key) {
    int left = 0;
    int right = MAX_ELEMENTS-1;
    int mid;

    while(right >= left) {
        mid = (left+right)/2;
        if(rects_before[mid].left > key || rects_before[mid].left == -999)
            right = mid - 1;
        else
            left = mid + 1;
    }

    return left-1;
}

int left_bin_search_after(int key) {
    int left = 0;
    int right = MAX_ELEMENTS - 1;
    int mid;

    while (right >= left) {
        mid = (left + right) / 2;

        if (rects_after[mid].left >= key || rects_after[mid].left == -999)
            right = mid - 1;
        else
            left = mid + 1;
    }

    return right + 1;
}

int right_bin_search_after(int key) {
    int left = 0;
    int right = MAX_ELEMENTS-1;
    int mid;

    while(right >= left) {
        mid = (left+right)/2;
        if(rects_after[mid].left > key || rects_after[mid].left == -999)
            right = mid - 1;
        else
            left = mid + 1;
    }

    return left-1;
}

void push_new(RECT rect) {
    int l = left_bin_search_after(rect.left);
    int r = right_bin_search_after(rect.left);

    int place = l;
    while(place <= r && rect.top > rects_after[place].top)
        place++;

    for(int i = MAX_ELEMENTS; i > place; i--)
        rects_after[i] = rects_after[i-1];
    rects_after[place] = rect;
}

void add_new_element(int i, int j) {
    int comrades = 0;

    int l_l = left_bin_search(i - WIDTH_EL);
    int r_l = right_bin_search(i + WIDTH_EL);

    if(l_l == -1)
        return;

    for (int k = l_l; k <= r_l; k++) {
        if (rects_before[k].left == i && rects_before[k].top == j) {
            return;
        }

        if (abs(rects_before[k].left - i) <= WIDTH_EL && abs(rects_before[k].top - j) <= WIDTH_EL)
            comrades++;
        }

    if(comrades == 3) {
        RECT rect;
        rect.left = i;
        rect.top = j;
        rect.right = rect.left + WIDTH_EL - 1;
        rect.bottom = rect.top + WIDTH_EL - 1;

        push_new(rect);
    }
}

void delete_element() {
    for(int i = 0; i < MAX_ELEMENTS && rects_before[i].left != -999; i++) {
        int comrades = 0;

        int l = left_bin_search(rects_before[i].left - WIDTH_EL);
        int r = right_bin_search(rects_before[i].left + WIDTH_EL);

        for (int k = l; k <= r; k++) {
            if(rects_before[k].left == rects_before[i].left && rects_before[k].top == rects_before[i].top) {
                continue;
            }

            if (abs(rects_before[k].left - rects_before[i].left) <= WIDTH_EL
                && abs(rects_before[k].top - rects_before[k].top) <= WIDTH_EL) {
                comrades++;
            }
        }

        if(comrades != 2 && comrades != 3) {
            int j;
            for(j = i; j+1 < MAX_ELEMENTS; j++) {
                rects_after[j] = rects_after[j+1];
            }
            rects_after[j].left = -999;
        }
    }
}

void game_life() {
    memcpy(rects_after, rects_before, sizeof(rects_before));

    delete_element();

    for(int i = 0; i < WIDTH; i += WIDTH_EL) {
        for(int j = 0; j < HEIGHT; j += WIDTH_EL) {
            add_new_element(i, j);
        }
    }

    memcpy(rects_before, rects_after, sizeof(rects_after));
}

void add_new_rect(int i, int x, int y) {
    rects_before[i].left = x * WIDTH_EL;
    rects_before[i].top = y * WIDTH_EL;
    rects_before[i].right = rects_before[i].left + WIDTH_EL - 1;
    rects_before[i].bottom = rects_before[i].top + WIDTH_EL - 1;
}

void setup() {
    for(int i = 0; i < MAX_ELEMENTS; i++) {
        rects_before[i].left = -999;
        rects_after[i].left = -999;
    }

//    add_new_rect(0, 9, 9);
//    add_new_rect(1, 9, 10);
//    add_new_rect(2, 10, 8);
//    add_new_rect(3, 10, 11);
//    add_new_rect(4, 11, 9);
//    add_new_rect(5, 11, 10);

    add_new_rect(0, 10, 10);
    add_new_rect(1, 10, 12);
    add_new_rect(2, 11, 11);
}

void draw(HDC dc) {
    SelectObject(dc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(dc, RGB(255, 255, 255));
    Rectangle(dc, 0, 0, WIDTH, HEIGHT);

    SelectObject(dc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(dc, RGB(0, 0, 0));
    SelectObject(dc, GetStockObject(DC_PEN));
    SetDCPenColor(dc, RGB(0, 0, 0));

    for(int i = 0; i < MAX_ELEMENTS; i++) {
        if(rects_before[i].left != -999)
            Rectangle(dc, rects_before[i].left, rects_before[i].top, rects_before[i].right, rects_before[i].bottom);
    }
}

LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if(msg == WM_DESTROY) {
        PostQuitMessage(0);
    }

    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

int main() {
    WNDCLASSA window;
    MSG message;
    HWND hwnd;
    HDC dc;

    memset(&window, 0, sizeof(WNDCLASSA));
    window.lpszClassName = "GameLife";
    window.lpfnWndProc = WndProc;
    RegisterClassA(&window);

    hwnd = CreateWindow(window.lpszClassName, "Window", WS_OVERLAPPEDWINDOW, 10, 10, WIDTH, HEIGHT, NULL, NULL, NULL, NULL);
    ShowWindow(hwnd, SW_SHOWNORMAL);

    dc = GetDC(hwnd);

    setup();

    while(1) {
        if(PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
            if(message.message == WM_QUIT) {
                break;
            }
            DispatchMessage(&message);
        } else {
            draw(dc);
            game_life();
//            Sleep(100);
            getchar();
        }
    }

    return 0;
}
