#include "raylib.h"
#include <vector>

#define screenWidth 800
#define screenHeight 600

#define minWindowWidth 500
#define minWindowHeight 500

#define FPS 120

// pillar states
#define NORMAL 0
#define SORTED 1
#define SELECTED 2

// bools
bool shouldRandomizeArray = true;
bool shouldShowMenuScreen = true;
bool bubbleSortPressed = false;
bool shouldShowStartOptions = false;
bool shouldStartSorting = false;
bool shouldShowEndingScreen = false;
bool addSpeed = false;
bool subSpeed = false;
bool addSize = false;
bool subSize = false;
bool normalSpeed = false;
bool normalSize = false;

// sorting algos
void bubbleSort(std::vector<std::pair<int, int>> &arr);

// screen
void showMenuScreen();
void showStartOptions();
void showEndingScreen();
void sortArray();
void Button(float x, float y, char *text, Color color, bool &state);
void bubbleSortButton(float tmp, char bubbleSortText[]);
void startButton(float size, float font, char start[]);
void changeSpeed(char operation, int &value);
void changeSize(char operation, int &value);


// array size
int numberOfPillars = 50;

// sorting speed
int sortingSpeed = 61;


// assorted function declarations
void drawArray(std::vector<std::pair<int, int>> arr);
void randomizeArray(std::vector<std::pair<int, int>> &arr);
Color findColorForPillar(int pillarState);

// array itself
std::vector<std::pair<int, int>> arr(numberOfPillars);
 

int main() {
    // window config:
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Sorting Algorithm Visualziation");
    SetWindowMinSize(minWindowWidth, minWindowHeight);

    // set fps
    SetTargetFPS(FPS);

    // while window not closed, clear background to purple
    while(!WindowShouldClose()) {

        BeginDrawing();

        ClearBackground(BLACK);

        if (shouldShowMenuScreen) {
            showMenuScreen();
        }

        shouldShowStartOptions = false;
        if (bubbleSortPressed) {
            shouldShowStartOptions = true;
        }

        if (shouldShowStartOptions) {
            showStartOptions();
        }

        if(shouldRandomizeArray) {
            randomizeArray(arr);
        }

        if(shouldStartSorting) {
            shouldShowMenuScreen = false;
            shouldShowStartOptions = false;

            SetTargetFPS(sortingSpeed);
            sortArray();

            shouldStartSorting = false;
            shouldShowEndingScreen = true;
        }

        if (shouldShowEndingScreen) {
            showEndingScreen();
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}



// draw pillars from array
void drawArray(std::vector<std::pair<int, int>> arr) {
    float barWidth = (float) GetScreenWidth() / numberOfPillars;
    
    for (int i = 0; i < numberOfPillars; i++) {
        Color color = findColorForPillar(arr[i].second);

        // void DrawRectangleV(Vector2 position, Vector2 size, Color color);
        DrawRectangleV(Vector2{(float) i * barWidth, (float) GetScreenHeight() - arr[i].first},
                       Vector2{barWidth, (float) arr[i].first},
                       color);
    }
}

// randomize array
void randomizeArray(std::vector<std::pair<int, int>> &arr) {
    for (int i = 0; i < numberOfPillars; i++) {
        arr[i] = {GetRandomValue(40, minWindowWidth - 80), NORMAL};
    }
    shouldRandomizeArray = false;

    return;
}

// show menu options
void showMenuScreen() {
    float font = (2.5 * GetScreenWidth()) / 100;
    char  bubbleSortText[] = "Bubble Sort";
    float tmp = MeasureText(bubbleSortText, font) / 2;
    bubbleSortButton(tmp, bubbleSortText);

    drawArray(arr);
}

void showStartOptions() {
    float font = (2.5 * GetScreenWidth() / 100);

    char startText[] = "Start Sorting!";
    float tmp = (27*GetScreenWidth()) / 100;
    startButton(tmp, font, startText);

    tmp += MeasureText(startText, font) + 75;
    char randomizeArrayText[] = "Randomize Array!";
    Button(tmp, GetScreenHeight() / 20 + font * 2, 
        randomizeArrayText, WHITE, shouldRandomizeArray);

    addSpeed = false;
    subSpeed = false;
    addSize = false;
    subSize = false;

    normalSize = false;
    normalSpeed = false;

    tmp = (84.2 * GetScreenWidth()) / 100;
    char timeButtonText[] = "Speed";

    Button(tmp, GetScreenHeight() / 20 + font * 2, 
        timeButtonText, WHITE, normalSpeed);
    
    if (normalSpeed) {
        changeSpeed('/', sortingSpeed);
        return;
    }

    tmp += MeasureText(timeButtonText, font) + 20;
    char speedPlusButtonText[] = "+";
    Button(tmp, GetScreenHeight() / 20 + font * 2, 
        speedPlusButtonText, WHITE, addSpeed);
    
    if (addSpeed) {
        changeSpeed('+', sortingSpeed);
        return;
    }

    tmp += MeasureText(speedPlusButtonText, font) + 20;
    char speedMinusButtonText[] = "-";
    Button(tmp, GetScreenHeight() / 20 + font * 2, 
        speedMinusButtonText, WHITE, subSpeed);
    
    if (subSpeed) {
        changeSpeed('-', sortingSpeed);
        return;
    }

    tmp = (5 * GetScreenWidth()) / 100;
    char sizeButtonText[] = "Size";
    Button(tmp, GetScreenHeight()/20 + font * 2, 
        sizeButtonText, WHITE, normalSize);
    
    if (normalSize) {
        changeSize('/', numberOfPillars);
        return;
    }

    tmp += MeasureText(sizeButtonText, font) + 20;
    char sizePlusButton[] = "+";
    Button(tmp, GetScreenHeight() / 20 + font * 2, 
        sizePlusButton, WHITE, addSize);
    
    if (addSize) {
        changeSize('+', numberOfPillars);
        return;
    }

    tmp += MeasureText(sizePlusButton, font) + 20;
    char sizeMinusButton[] = "-";
    Button(tmp, GetScreenHeight() / 20 + font * 2, 
        sizeMinusButton, WHITE, subSize);

    if (subSize) {
        changeSize('-', numberOfPillars);
        return;
    }
}

void showEndingScreen() {
    drawArray(arr);

    shouldShowMenuScreen = true;
    shouldShowStartOptions = false;
}

void changeSpeed(char operation, int &value) {
    switch (operation) {
        case '-':
            value = value - 10 > 0 ? value - 10 : value;
            break;
        case '+':
            value += 10;
            break;
        default:
            value = 61;
            break;
    }
}

void changeSize(char operation, int& value) {
    switch (operation) {
        case '-':
            if (value > 5) {
                value -= 5;
                for (int i = 0; i < 5; i++) {
                    arr.pop_back();
                }
            }
            break;
        case '+':
            value += 5;
            for (int i = 0; i < 5; i++) {
                arr.push_back({GetRandomValue(40, minWindowWidth - 80), NORMAL});
            }
            break;
        default:
            while (numberOfPillars > 50) {
                numberOfPillars--;
                arr.pop_back();
            }

            while(numberOfPillars < 50) {
                numberOfPillars++;
                arr.push_back({GetRandomValue(40, minWindowWidth - 80), NORMAL});
            }
            break;
    }
    for (int i = 0; i < numberOfPillars; i++) {
        arr[i].second = NORMAL;
    }
    drawArray(arr);
}

void startButton(float size, float font, char start[]) {
    Button(size, GetScreenHeight() / 20 + font * 2, 
        start, WHITE, shouldStartSorting);
    return;
}


void bubbleSortButton(float size, char bubbleSortText[]) {
    Color color;
    if (bubbleSortPressed) {
        color = BLUE;
    } else {
        color = WHITE;
    }
    Button(GetScreenWidth() / 2 - size, GetScreenHeight() / 20, bubbleSortText, color, bubbleSortPressed);
}

// case by case pillar color
Color findColorForPillar(int pillarState) {
    switch (pillarState) {
        case SELECTED:
            return RED;
            break;
        case SORTED:
            return GREEN;
            break;
        default:
            return WHITE;
            break;
    }
}

void sortArray() {
    bubbleSort(arr);
    drawArray(arr);
    return;
}

// bubble sort
void bubbleSort(std::vector<std::pair<int, int>> &arr) {
    bool swapped;
    int endPoint = numberOfPillars;
    do {
        swapped = false;
        for (int i = 0; i < endPoint - 1; i++) {
            arr[i].second = SELECTED;
            if (arr[i] > arr[i + 1]) {
                std::swap(arr[i], arr[i + 1]);
                swapped = true;
            }
            BeginDrawing();
                ClearBackground(BLACK);

                for (int k = numberOfPillars - 1; k >= endPoint; k--) {
                    arr[k].second = SORTED;
                }
                drawArray(arr);
                for (int k = i; k >= 0; i--) {
                    arr[k].second = NORMAL;
                }
            EndDrawing();
        }

        endPoint--;
    } while (swapped);
    for (int i = numberOfPillars - 1; i >= 0; i--) {
        arr[i].second = SORTED;
    }
}

void Button(float x, float y, char *text, Color color, bool &state) {
    float font = (2.5 * GetScreenWidth() / 100);
    Rectangle r1 = {
        .x = x,
        .y = y,
        .width = (float) MeasureText(text, font),
        .height = (float) font,
    };

    if (CheckCollisionPointRec(GetMousePosition(), r1)) {
        DrawText(text, x, y, font, SKYBLUE);

        if (IsMouseButtonPressed(0)) {
            if (state == true) {
                state = false;
            } else {
                state = true;
            }
            return;
        }
    } else {
        DrawText(text, x, y, font, color);
    }
}