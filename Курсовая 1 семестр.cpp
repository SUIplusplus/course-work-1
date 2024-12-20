#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <cstring>
#include <cctype>
#include <vector>
#include <chrono>
using namespace std;
const int maxWords = 50;
const int maxLength = 11;
const int maxInputSize = 1000;

void InputCheck(int& number)
{
    while (cin.fail())
    {
        cin.clear();
        cin.sync();
        cin.ignore(1000, '\n');
        cout << "Введите ЧИСЛО" << endl;
        cin >> number;
    }
}

void RemoveExtraSpaces(char* str)
{
    int n = strlen(str);
    int j = 0;
    int count = 0;

    for (int i = 0; i < n; i++)
    {
        if (str[i] != ' ' || (i > 0 && str[i - 1] != ' '))
        {
            str[j] = str[i];
            j++;
        }
    }
    str[j] = '\0';
}

void RemoveExtraPunctuation(char* str)
{
    int n = strlen(str);
    int j = 0;

    for (int i = 0; i < n; i++)
    {
        if (!ispunct(str[i]) || (i > 0 && !ispunct(str[i - 1])))
        {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}

void FixLetterCase(char* str)
{
    int n = strlen(str);
    for (int i = 1; i < n; i++)
    {
        if (str[i - 1] != ' ' && isupper(str[i]))
            str[i] = tolower(str[i]);
    }
}

void EditText(char* text)
{
    RemoveExtraSpaces(text);
    RemoveExtraPunctuation(text);
    FixLetterCase(text);
}

int TextToWords(const char* text, char(&words)[maxWords][maxLength])
{
    int wordCount = 0;
    char text1[maxInputSize];
    strcpy(text1, text);

    char* token = strtok(text1, " .,:;-?!()[]{}<>|/+=*&^%$#@~`№");
    while (token != nullptr && wordCount < maxWords)
    {
        strcpy(words[wordCount], token);
        wordCount++;
        token = strtok(nullptr, " .,:;-?!()[]{}<>|/+=*&^%$#@~`№");
    }

    return wordCount;
}

void ChangeRegister(char(&words)[maxWords][maxLength], int count)
{
    for (int i = 0; i < count; i++)
        words[i][0] = toupper(words[i][0]);
}

int LinearSearch(char* text, char* pattern)
{
    int n = strlen(text);
    int m = strlen(pattern);

    for (int i = 0; i <= n - m; i++) {
        int j;
        for (j = 0; j < m; j++) {
            if (text[i + j] != pattern[j])
                break;
        }

        if (j == m)
            return i;
    }

    return -1;
}

vector<int> Suffixes(const char* pattern) {
    int length = strlen(pattern);
    vector<int> suffix(length, 0);

    int g, f;
    g = f = length - 1;
    for (int i = length - 2; i >= 0; --i)
    {
        if (i > g && suffix[i + length - 1 - f] < i - g)
            suffix[i] = suffix[i + length - 1 - f];
        else
        {
            if (i < g)
                g = i;
            f = i;
            while (g >= 0 && pattern[g] == pattern[g + length - 1 - f])
                --g;
            suffix[i] = f - g;
        }
    }

    return suffix;
}

vector<int> boyerMoore(const char* text, const char* pattern) {
    int n = strlen(text);
    int m = strlen(pattern);
    std::vector<int> occurrences;

    std::vector<int> shift(256, m);
    for (int i = 0; i < m - 1; ++i) {
        shift[pattern[i]] = m - i - 1;
    }

    std::vector<int> suffix = Suffixes(pattern);

    int i = 0;
    while (i <= n - m) {
        int j = m - 1;
        while (j >= 0 && pattern[j] == text[i + j]) {
            --j;
        }
        if (j < 0) {
            occurrences.push_back(i);
            i += shift[text[i + m]];
        }
        else {
            int x = suffix[j];
            int y = j - x;
            int z = m - 1 - j;
            i += max(shift[text[i + j]] - y, z);
        }
    }

    return occurrences;
}

COORD GetConsoleCursorPosition(HANDLE hConsoleOutput)
{
    CONSOLE_SCREEN_BUFFER_INFO cbsi;
    if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi))
    {
        return cbsi.dwCursorPosition;
    }
    else
    {
        COORD invalid = { 0, 0 };
        return invalid;
    }
}

void FillMatrixA(int* matrix, int N, const int& delay, HANDLE hStdout, COORD destCoord) {
    COORD current = GetConsoleCursorPosition(hStdout);
    int Q = current.Y;
    int num;
    for (int j = 0; j < N; ++j)
    {
        destCoord.X = j * 3 + 1;
        if (j % 2 == 0)
        {
            for (int i = 0; i < N; ++i)
            {
                num = rand() % (N * N + 1);
                *(matrix + i * N + j) = num;
                destCoord.Y = i + Q;
                SetConsoleCursorPosition(hStdout, destCoord);
                cout << num << '\r';
                cout.flush();
                Sleep(delay);
            }
        }
        else
        {
            for (int i = N - 1; i >= 0; --i)
            {
                num = rand() % (N * N + 1);
                *(matrix + i * N + j) = num;
                destCoord.Y = i + Q;
                SetConsoleCursorPosition(hStdout, destCoord);
                cout << num << '\r';
                cout.flush();
                Sleep(delay);
            }
        }
    }
    destCoord.X = 0;
    destCoord.Y = Q + N;
    SetConsoleCursorPosition(hStdout, destCoord);
}

void FillMatrixB(int* matrix, int N, const int& delay, HANDLE hStdout, COORD destCoord) {
    COORD current = GetConsoleCursorPosition(hStdout);
    int Q = current.Y;
    int num, k = 1;
    int top = 0, bottom = N - 1, left = 0, right = N - 1;

    while (k <= N * N)
    {
        destCoord.Y = top + Q;
        for (int j = left; j <= right; ++j)
        {
            num = rand() % (N * N + 1);
            *(matrix + top * N + j) = num;
            destCoord.X = j * 3 + 1;;
            SetConsoleCursorPosition(hStdout, destCoord);
            cout << num << '\r';
            cout.flush();
            Sleep(delay);
            k++;
        }
        top++;

        destCoord.X = right * 3 + 1;
        for (int i = top; i <= bottom; ++i)
        {
            num = rand() % (N * N + 1);
            *(matrix + i * N + right) = num;
            destCoord.Y = i + Q;
            SetConsoleCursorPosition(hStdout, destCoord);
            cout << num << '\r';
            cout.flush();
            Sleep(delay);
            k++;
        }
        right--;

        destCoord.Y = bottom + Q;
        if (top <= bottom)
        {
            for (int j = right; j >= left; --j)
            {
                num = rand() % (N * N + 1);
                *(matrix + bottom * N + j) = num;
                destCoord.X = j * 3 + 1;
                SetConsoleCursorPosition(hStdout, destCoord);
                cout << num << '\r';
                cout.flush();
                Sleep(delay);
                k++;
            }
            bottom--;
        }

        destCoord.X = left * 3 + 1;
        if (left <= right)
        {
            for (int i = bottom; i >= top; --i)
            {
                num = rand() % (N * N + 1);
                *(matrix + i * N + left) = num;
                destCoord.Y = i + Q;
                SetConsoleCursorPosition(hStdout, destCoord);
                cout << num << '\r';
                cout.flush();
                Sleep(delay);
                k++;
            }
            left++;
        }
    }
    destCoord.X = 0;
    destCoord.Y = N + Q;
    SetConsoleCursorPosition(hStdout, destCoord);
}

void ReplaceBlocksA(int* matrix, int* newMatrix, int N)
{
    for (int i = 0; i < N * N; ++i)
    {
        if ((i + 1) % N <= N / 2 && (i + 1) % N != 0 && i + 1 <= N * N / 2)
        {
            *(newMatrix + i + N / 2) = *(matrix + i);
        }
        else if (((i + 1) % N > N / 2 || (i + 1) % N == 0) && i + 1 <= N * N / 2)
        {
            *(newMatrix + i + N * N / 2) = *(matrix + i);
        }
        else if ((i + 1) % N <= N / 2 && (i + 1) % N != 0 && i + 1 > N * N / 2)
        {
            *(newMatrix + i - N * N / 2) = *(matrix + i);
        }
        else if (((i + 1) % N > N / 2 || (i + 1) % N == 0) && i + 1 > N * N / 2)
        {
            *(newMatrix + i - N / 2) = *(matrix + i);
        }
    }
}

void ReplaceBlocksB(int* matrix, int* newMatrix, int N)
{
    for (int i = 0; i < N * N; ++i)
    {
        if ((i + 1) % N <= N / 2 && (i + 1) % N != 0 && i + 1 <= N * N / 2)
        {
            *(newMatrix + i + N / 2 + N * N / 2) = *(matrix + i);
        }
        else if (((i + 1) % N > N / 2 || (i + 1) % N == 0) && i + 1 <= N * N / 2)
        {
            *(newMatrix + i + N * N / 2 - N / 2) = *(matrix + i);
        }
        else if ((i + 1) % N <= N / 2 && (i + 1) % N != 0 && i + 1 > N * N / 2)
        {
            *(newMatrix + i + N / 2 - N * N / 2) = *(matrix + i);
        }
        else if (((i + 1) % N > N / 2 || (i + 1) % N == 0) && i + 1 > N * N / 2)
        {
            *(newMatrix + i - N / 2 - N * N / 2) = *(matrix + i);
        }
    }
}

void ReplaceBlocksC(int* matrix, int* newMatrix, int N)
{
    for (int i = 0; i < N * N; ++i)
    {
        if ((i + 1) % N <= N / 2 && (i + 1) % N != 0 && i + 1 <= N * N / 2)
        {
            *(newMatrix + i + N * N / 2) = *(matrix + i);
        }
        else if (((i + 1) % N > N / 2 || (i + 1) % N == 0) && i + 1 <= N * N / 2)
        {
            *(newMatrix + i + N * N / 2) = *(matrix + i);
        }
        else if ((i + 1) % N <= N / 2 && (i + 1) % N != 0 && i + 1 > N * N / 2)
        {
            *(newMatrix + i - N * N / 2) = *(matrix + i);
        }
        else if (((i + 1) % N > N / 2 || (i + 1) % N == 0) && i + 1 > N * N / 2)
        {
            *(newMatrix + i - N * N / 2) = *(matrix + i);
        }
    }
}

void ReplaceBlocksD(int* matrix, int* newMatrix, int N)
{
    for (int i = 0; i < N * N; ++i)
    {
        if ((i + 1) % N <= N / 2 && (i + 1) % N != 0 && i + 1 <= N * N / 2)
        {
            *(newMatrix + i + N / 2) = *(matrix + i);
        }
        else if (((i + 1) % N > N / 2 || (i + 1) % N == 0) && i + 1 <= N * N / 2)
        {
            *(newMatrix + i - N / 2) = *(matrix + i);
        }
        else if ((i + 1) % N <= N / 2 && (i + 1) % N != 0 && i + 1 > N * N / 2)
        {
            *(newMatrix + i + N / 2) = *(matrix + i);
        }
        else if (((i + 1) % N > N / 2 || (i + 1) % N == 0) && i + 1 > N * N / 2)
        {
            *(newMatrix + i - N / 2) = *(matrix + i);
        }
    }
}

void InsertSortMatrix(int* matrix, int N)
{
    int key, j;

    for (int i = 1; i != N; ++i)
    {
        key = *(matrix + i);
        j = i - 1;

        while (j >= 0 && key < *(matrix + j))
        {
            *(matrix + j + 1) = *(matrix + j);
            j -= 1;
        }
        *(matrix + j + 1) = key;
    }
}

void Multiply(int* matrix, int N, int X)
{
    for (int i = 0; i < N * N; ++i)
        *(matrix + i) *= X;
}

void Divide(int* matrix, int N, int X)
{
    for (int i = 0; i < N * N; ++i)
        *(matrix + i) /= X;
}

void Add(int* matrix, int N, int X)
{
    for (int i = 0; i < N * N; ++i)
        *(matrix + i) += X;
}

void Subtract(int* matrix, int N, int X)
{
    for (int i = 0; i < N * N; ++i)
        *(matrix + i) -= X;
}

void BubbleSort(int Array[], int N)
{
    bool swapped = true;
    int j = 0;

    while (swapped)
    {
        swapped = false;
        for (int i = 0; i != N - j - 1; ++i)
        {
            if (Array[i] > Array[i + 1])
            {
                swap(Array[i], Array[i + 1]);
                swapped = true;
            }
        }
        j++;
    }
}

void ShakerSort(int Array[], int N)
{
    int start = 0, end = N - 1;
    bool swapped = true;

    while (swapped)
    {
        swapped = false;

        for (int i = start; i != end; ++i)
        {
            if (Array[i] > Array[i + 1])
            {
                swap(Array[i], Array[i + 1]);
                swapped = true;
            }
        }

        if (!swapped)
            break;

        swapped = false;
        --end;

        for (int i = end; i != start; --i)
        {
            if (Array[i] < Array[i - 1])
            {
                std::swap(Array[i], Array[i - 1]);
                swapped = true;
            }
        }

        ++start;
    }
}

void CombSort(int Array[], int N)
{
    bool swapped = true;
    int j = 0;
    float S = N - 1, k = 1.247;

    while (S >= 1)
    {
        for (int i = 0; i + S < N; ++i)
        {
            if (Array[i] > Array[int(i + S)])
                swap(Array[i], Array[int(i + S)]);
        }
        S /= k;
    }

    while (swapped)
    {
        swapped = false;
        for (int i = 0; i != N - j - 1; ++i)
        {
            if (Array[i] > Array[i + 1])
            {
                swap(Array[i], Array[i + 1]);
                swapped = true;
            }
        }
        j++;
    }
}

void InsertSort(int Array[], int N)
{
    int key, j;

    for (int i = 1; i != N; ++i)
    {
        key = Array[i];
        j = i - 1;

        while (j >= 0 && key < Array[j])
        {
            Array[j + 1] = Array[j];
            j -= 1;
        }
        Array[j + 1] = key;
    }
}

void QuickSort(int Array[], int start, int end)
{
    int mid, l = start, r = end;
    mid = Array[(l + r) / 2];

    while (l < r)
    {
        while (Array[l] < mid)
            l++;
        while (Array[r] > mid)
            r--;
        if (l <= r)
        {
            swap(Array[l], Array[r]);
            l++;
            r--;
        }
    }
    if (start < r)
        QuickSort(Array, start, r);
    if (end > l)
        QuickSort(Array, l, end);
}

int BinarySearch(int Array[], int value, int start, int end)
{
    if (end >= start)
    {
        int mid = start + (end - start) / 2;
        if (Array[mid] == value)
            return mid;
        if (Array[mid] > value)
            return BinarySearch(Array, value, start, mid - 1);
        return BinarySearch(Array, value, mid + 1, end);
    }
    return -1;
}

int BinarySearchFirstOccurrence(int Array[], int value, int start, int end)
{
    if (end >= start)
    {
        int mid = start + (end - start) / 2;
        if (Array[mid] == value)
        {
            while (Array[mid - 1] == value && mid - 1 >= 0)
                mid -= 1;
            return mid;
        }
        if (Array[mid] > value)
            return BinarySearchFirstOccurrence(Array, value, start, mid - 1);
        return BinarySearchFirstOccurrence(Array, value, mid + 1, end);
    }
    return start;
}

int BinarySearchLastOccurrence(int Array[], int value, int start, int end)
{
    int mid = start + (end - start) / 2;
    if (end >= start)
    {
        if (Array[mid] == value)
        {
            while (Array[mid + 1] == value && mid + 1 <= end)
                mid += 1;
            return mid;
        }
        if (Array[mid] > value)
            return BinarySearchLastOccurrence(Array, value, start, mid - 1);
        return BinarySearchLastOccurrence(Array, value, mid + 1, end);
    }
    return start - 1;
}

int ElemSearch(int Array[], int value, int N)
{
    int X = -1;
    for (int i = 0; i < N; ++i)
    {
        if (Array[i] == value)
        {
            X = value;
            break;
        }
    }
    return X;
}

int MinSearch(int Array[], int N)
{
    int min = 101;
    for (int i = 0; i < N; ++i)
        min = Array[i] < min ? Array[i] : min;
    return min;
}

int MaxSearch(int Array[], int N)
{
    int max = 0;
    for (int i = 0; i < N; ++i)
        max = Array[i] > max ? Array[i] : max;
    return max;
}

void First()
{
	cout << "int: " << sizeof(int) << endl;
	cout << "short int: " << sizeof(short int) << endl;
	cout << "long int: " << sizeof(long int) << endl;
	cout << "long long int: " << sizeof(long long int) << endl;
	cout << "float: " << sizeof(float) << endl;
	cout << "double: " << sizeof(double) << endl;
	cout << "long double: " << sizeof(long double) << endl;
	cout << "char: " << sizeof(char) << endl;
	cout << "bool: " << sizeof(bool) << endl;
}

void Second()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int color = 9;
	int A;
	int order = sizeof(int) * 8;
	unsigned int mask = 1 << order - 1;
	cout << "Введите число типа int" << endl;
	cin >> A;

	while (cin.fail())
	{
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Введите ЧИСЛО" << endl;
		cin >> A;
	}

	for (int i = 1; i <= order; ++i)
	{
		SetConsoleTextAttribute(hConsole, color);
		putchar(mask & A ? '1' : '0');
		mask >>= 1;
		if (i % 8 == 0 || i == 1)
		{
			putchar(' ');
			color++;
		}
	}
	SetConsoleTextAttribute(hConsole, 7);
	cout << endl;
}

void Third()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int color = 11;
	union
	{
		int tool;
		float F;
	};
	int order = sizeof(float) * 8;
	unsigned int mask = 1 << order - 1;
	cout << "Введите число типа float" << endl;
	cin >> F;

	while (cin.fail())
	{
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Введите ЧИСЛО" << endl;
		cin >> F;
	}

	for (int i = 1; i <= order; ++i)
	{
		SetConsoleTextAttribute(hConsole, color);
		putchar(mask & tool ? '1' : '0');
		mask >>= 1;
		if (i == 9 || i == 1)
		{
			putchar(' ');
			color++;
		}
	}
	SetConsoleTextAttribute(hConsole, 7);
	cout << endl;
}

void Fourth()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int color = 11;
	union {
		int tool[2];
		double D;
	};
	int order = sizeof(D) * 8;
	unsigned int mask = 1;
	mask <<= order - 1;
	cout << "Введите число типа double" << endl;
	cin >> D;

	while (cin.fail())
	{
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Введите ЧИСЛО" << endl;
		cin >> D;
	}

	for (int i = 1; i >= 0; --i)
	{
		for (int j = 0; j < order / 2; ++j)
		{
			SetConsoleTextAttribute(hConsole, color);
			putchar(mask & tool[i] ? '1' : '0');
			mask >>= 1;
			if ((i == 1 && j == 0) || (i == 1 && j == 11))
			{
				putchar(' ');
				color++;
			}
		}
		mask = 1 << order - 1;
	}
	SetConsoleTextAttribute(hConsole, 7);
	cout << endl;
}

void PracticalWork1()
{
	setlocale(0, "");
	int number;

    cout << "Введите номер задания: ";
    cin >> number;

    switch (number)
    {
    case 1:
        First();
        break;
    case 2:
        Second();
        break;
    case 3:
        Third();
        break;
    case 4:
        Fourth();
        break;
    case 666:
        return;
    default:
        cout << "Задания с таким номером нет" << endl;
    }
	
}

void PracticalWork2()
{
    srand(time(0));
    setlocale(0, "");

    const int N = 100;
    int Array[N], sortedArray[N], select = 0;
    bool isSorted = false;
    chrono::nanoseconds duration;
    chrono::steady_clock::time_point start, end;

    cout << "Неотсортированный массив:" << endl;
    for (int i = 0; i < N; ++i)
    {
        Array[i] = -99.5 + rand() % 200;
        cout << Array[i] << " ";
    }
    cout << endl;

    cout << "1 - создать новый неотсортированный массив\n" <<
        "2 - отсортировать массив\n" <<
        "3 - найти максимальный и минимальный элементы массива\n" <<
        "4 - найти среднее значение максимального и минимального элементов, а также вывести индексы элементов, равных этому и значению и их количество\n" <<
        "5 - найти количество элементов в отсортированном массиве, которые меньше числа A\n" <<
        "6 - найти количество элементов в отсортированном массиве, которые больше числа B\n" <<
        "7 - вывести информацию о том, есть ли число X в массиве\n" <<
        "8 - поменять местами два элемента\n" <<
        "9 - показать текущее состояние массивов" << endl;

    while (true)
    {
        cin >> select;
        InputCheck(select);
        switch (select)
        {
        case 1:
            cout << "Неотсортированный массив:" << endl;
            for (int i = 0; i < N; ++i)
            {
                Array[i] = -99.5 + rand() % 200;
                cout << Array[i] << " ";
            }
            cout << endl;
            isSorted = false;
            break;
        case 2:
        {
            isSorted = false;
            cout << "1 - Bubble sort\n2 - Shaker sort\n3 - Comb sort\n4 - Insert sort\n5 - Quick sort" << endl;
            for (int i = 0; i < N; ++i)
                sortedArray[i] = Array[i];

            cout << "Введите номер алгоритма" << endl;
            int numberOfAlgorithm;
            cin >> numberOfAlgorithm;
            InputCheck(numberOfAlgorithm);
            while (!isSorted)
            {
                switch (numberOfAlgorithm)
                {
                case 1:
                    isSorted = true;
                    start = chrono::steady_clock::now();
                    BubbleSort(sortedArray, N);
                    end = chrono::steady_clock::now();
                    duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
                    break;

                case 2:
                    isSorted = true;
                    start = chrono::steady_clock::now();
                    ShakerSort(sortedArray, N);
                    end = chrono::steady_clock::now();
                    duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
                    break;

                case 3:
                    isSorted = true;
                    start = chrono::steady_clock::now();
                    CombSort(sortedArray, N);
                    end = chrono::steady_clock::now();
                    duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
                    break;

                case 4:
                    isSorted = true;
                    start = chrono::steady_clock::now();
                    InsertSort(sortedArray, N);
                    end = chrono::steady_clock::now();
                    duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
                    break;

                case 5:
                    isSorted = true;
                    start = chrono::steady_clock::now();
                    QuickSort(sortedArray, 0, N - 1);
                    end = chrono::steady_clock::now();
                    duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
                    break;

                default:
                    cout << "Алгоритма с такими номером нет. Введите другое число." << endl;
                }
            }
            if (isSorted)
            {
                cout << "Отсортированный массив:" << endl;
                for (int i = 0; i != N; ++i)
                    cout << sortedArray[i] << " ";
                cout << endl << "Алгоритм занял " << duration.count() << " нс" << endl;
            }
            break;
        }
        case 3:
        {
            if (isSorted)
            {
                start = chrono::steady_clock::now();
                int min = MinSearch(Array, N);
                end = chrono::steady_clock::now();
                duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
                cout << "Поиск минимального элемента в неотсортированном массиве занял " << duration.count() << " нс" << endl;

                start = chrono::steady_clock::now();
                int max = MaxSearch(Array, N);
                end = chrono::steady_clock::now();
                duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
                cout << "Поиск максимального элемента в неотсортированном массиве занял " << duration.count() << " нс" << endl;

                start = chrono::steady_clock::now();
                min = sortedArray[0];
                end = chrono::steady_clock::now();
                duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
                cout << "Поиск минимального элемента в отсортированном массиве занял " << duration.count() << " нс" << endl;

                start = chrono::steady_clock::now();
                max = sortedArray[N - 1];
                end = chrono::steady_clock::now();
                duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
                cout << "Поиск максимального элемента в отсортированном массиве занял " << duration.count() << " нс" << endl;

                cout << "Максимальный элемент: " << max << "\nМинимальный элемент: " << min << endl;
            }
            else
                cout << "Сначала надо отсортировать массив" << endl;
            break;
        }
        case 4:
        {
            if (isSorted)
            {
                int number = 0, mid;
                double dMid = static_cast<double>((sortedArray[N - 1] + sortedArray[0])) / 2;
                cout << "Точное среднее значение: " << dMid << endl;
                if (dMid > 0)
                    mid = dMid + 0.5;
                else
                    mid = dMid - 0.5;
                cout << "Округленное среднее значение: " << mid << endl;
                cout << "Индексы элементов, равных этому значению в неотсортированном массиве: ";
                start = chrono::steady_clock::now();
                for (int i = 0; i < N; ++i)
                {
                    if (Array[i] == mid)
                    {
                        cout << i << " ";
                        number++;
                    }
                }
                end = chrono::steady_clock::now();
                if (!number)
                    cout << "-" << endl;
                else cout << endl;
                duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
                cout << "Время поиска: " << duration.count() << " нс" << endl;

                number = 0;
                cout << "Индексы элементов, равных этому значению в отсортированном массиве: ";
                int j = 0;
                start = chrono::steady_clock::now();
                while (sortedArray[j] <= mid)
                {
                    if (sortedArray[j] == mid)
                    {
                        cout << j << " ";
                        number++;
                    }
                    j++;
                }
                end = chrono::steady_clock::now();
                if (!number)
                    cout << "-" << endl;
                else cout << endl;
                duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
                cout << "Время поиска: " << duration.count() << " нс" << endl;
                cout << "Количество таких элементов: " << number << endl;
            }
            else
                cout << "Сначала надо отсортировать массив" << endl;
            break;
        }
        case 5:
        {
            if (isSorted)
            {
                int A;
                cout << "Введите число a" << endl;
                cin >> A;
                InputCheck(A);
                int indexA = BinarySearchFirstOccurrence(sortedArray, A, 0, N - 1);
                cout << "Количество элементов, которые меньше a: " << indexA << endl;
            }
            else
                cout << "Сначала надо отсортировать массив" << endl;
            break;
        }
        case 6:
        {
            if (isSorted)
            {
                int B;
                cout << "Введите число B" << endl;
                cin >> B;
                InputCheck(B);
                int indexB = BinarySearchLastOccurrence(sortedArray, B, 0, N - 1);
                cout << "Количество элементов, которые больше b: " << N - indexB - 1 << endl;
            }
            else
                cout << "Сначала надо отсортировать массив" << endl;
            break;
        }
        case 7:
        {
            if (isSorted)
            {
                int X;
                cout << "Введите число X" << endl;
                cin >> X;
                InputCheck(X);
                start = chrono::steady_clock::now();
                int xInArray = BinarySearch(sortedArray, X, 0, N - 1);
                end = chrono::steady_clock::now();
                duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
                if (xInArray == -1)
                    cout << "Числа X нет в массиве" << endl;
                else
                    cout << "Число X есть в массиве" << endl;
                cout << "Алгоритм бинарного поиска занял " << duration.count() << " нс" << endl;
                start = chrono::steady_clock::now();
                xInArray = ElemSearch(Array, X, N);
                end = chrono::steady_clock::now();
                duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
                cout << "Алгоритм перебора занял " << duration.count() << " нс" << endl;
            }
            else
                cout << "Сначала надо отсортировать массив" << endl;
            break;
        }
        case 8:
        {
            int p, q;
            cout << "Введите индекс первого элемента" << endl;
            cin >> p;
            InputCheck(p);

            cout << "Введите индекс второго элемента" << endl;
            cin >> q;
            InputCheck(q);

            start = chrono::steady_clock::now();
            swap(sortedArray[p], sortedArray[q]);
            end = chrono::steady_clock::now();
            duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
            for (int i = 0; i < N; ++i)
                cout << sortedArray[i] << " ";
            cout << endl << "Обмен занял " << duration.count() << " нс" << endl;
            if (p != q)
                isSorted = false;
            break;
        }
        case 9:
            cout << "Неотсортированный массив:" << endl;
            for (int i = 0; i < N; ++i)
                cout << Array[i] << " ";
            cout << endl;
            cout << "Отсортированный массив:" << endl;
            for (int i = 0; i < N; ++i)
                cout << sortedArray[i] << " ";
            cout << endl;
            break;
        case 666:
            return;
        default:
            cout << "Нет команды с таким номером" << endl;
        }
    }
}

void PracticalWork3() {
    setlocale(0, "");
    srand(time(0));
    HANDLE hStdout;
    COORD destCoord;
    destCoord.X = 0;
    destCoord.Y = 0;
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    int N, delay = 10;

    cout << "Введите порядок матрицы (6, 8 или 10): ";
    cin >> N;
    InputCheck(N);

    int newMatrix6[6][6];
    int matrix6[6][6];

    int newMatrix8[8][8];
    int matrix8[8][8];

    int newMatrix10[10][10];
    int matrix10[10][10];

    int* matrix;
    int* newMatrix;

    switch (N)
    {
    case 6:
        matrix = *matrix6;
        newMatrix = *newMatrix6;
        break;
    case 8:
        matrix = *matrix8;
        newMatrix = *newMatrix8;
        break;
    case 10:
        matrix = *matrix10;
        newMatrix = *newMatrix10;
        break;
    default:
        cout << "Порядок матрицы должен быть 6, 8 или 10." << endl;
        return;
    }

    int choice;
    cout << "Введите номер схемы заполнения матрицы: ";
    cin >> choice;
    InputCheck(choice);

    switch (choice)
    {
    case 1:
        FillMatrixA(matrix, N, delay, hStdout, destCoord);
        break;
    case 2:
        FillMatrixB(matrix, N, delay, hStdout, destCoord);
        break;
    default:
        cout << "Нет такого варианта" << endl;
    }

    cout << "Введите номер схемы перестановки блоков матрицы: ";
    cin >> choice;
    InputCheck(choice);

    switch (choice)
    {
    case 1:
        ReplaceBlocksA(matrix, newMatrix, N);
        break;
    case 2:
        ReplaceBlocksB(matrix, newMatrix, N);
        break;
    case 3:
        ReplaceBlocksC(matrix, newMatrix, N);
        break;
    case 4:
        ReplaceBlocksD(matrix, newMatrix, N);
        break;
    default:
        cout << "Нет такого варианта" << endl;
    }

    int i = 0;
    for (int* ptr = newMatrix; ptr < newMatrix + N * N; ++ptr)
    {
        cout << *ptr << " ";
        if ((i + 1) % N == 0)
            cout << "\n";
        i++;
    }

    InsertSortMatrix(newMatrix, N * N);
    cout << "Отсортированная матрица: " << endl;
    i = 0;
    for (int* ptr = newMatrix; ptr < newMatrix + N * N; ++ptr)
    {
        cout << *ptr << " ";
        if ((i + 1) % N == 0)
            cout << "\n";
        i++;
    }

    int X;
    cout << "Введите число: ";
    cin >> X;
    InputCheck(X);
    cout << "1 - сложение\n2 - вычитание\n3 - умножение\n4 - деление" << endl;
    cout << "Введите номер операции: ";
    cin >> choice;
    InputCheck(choice);

    switch (choice)
    {
    case 1:
        Add(newMatrix, N, X);
        break;
    case 2:
        Subtract(newMatrix, N, X);
        break;
    case 3:
        Multiply(newMatrix, N, X);
        break;
    case 4:
        Divide(newMatrix, N, X);
        break;
    default:
        cout << "Нет такого варианта." << endl;
    }

    i = 0;
    for (int* ptr = newMatrix; ptr < newMatrix + N * N; ++ptr)
    {
        cout << *ptr << " ";
        if ((ptr - newMatrix + 1) % N == 0)
            cout << "\n";
        i++;
    }
}

void PracticalWork4()
{
    setlocale(0, "");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    char text[maxInputSize];
    char words[maxWords][maxLength];

    cout << "Введите последовательность слов, заканчивающуюся точкой:\n";
    cin.ignore(1000, '\n');
    cin.get(text, maxInputSize);

    EditText(text);

    cout << "Отредактированный текст:\n";
    cout << text;

    int count = TextToWords(text, words);

    cout << endl << "Количество слов: " << count << endl;
    cout << "Слова в обратном порядке: " << endl;
    for (int i = count - 1; i >= 0; --i)
        cout << words[i] << " ";
    cout << endl;

    ChangeRegister(words, count);

    for (int i = 0; i < count; i++)
        cout << words[i] << " ";
    cout << endl;

    char pattern1[maxLength];
    char pattern2[maxLength];

    cout << "Алгоритм линейного поиска\nВведите подстроку: ";
    cin.ignore(1000, '\n');
    cin.get(pattern1, maxLength);

    int result = LinearSearch(text, pattern1);

    if (result == -1)
        cout << "Подстрока не найдена" << endl;
    else
        cout << "Подстрока найдена в позиции: " << result << endl;

    cout << "Алгоритм Бойера-Мура\nВведите подстроку: ";
    cin.ignore(1000, '\n');
    cin.get(pattern2, maxLength);

    vector<int> occurrences = boyerMoore(text, pattern2);

    if (!occurrences.empty())
    {
        cout << "Подстрока найдена на позициях:";
        for (int i = 0; i < occurrences.size(); ++i)
            cout << occurrences[i] << " ";
    }
    else
        cout << "Подстрока не найдена";
    cout << endl;
}

int main()
{
    setlocale(0, "");

    int choice;
    while (1)
    {
        cout << "Введите номер практической работы: ";
        cin >> choice;
        InputCheck(choice);

        switch (choice)
        {
        case 1:
            PracticalWork1();
            break;
        case 2:
            PracticalWork2();
            break;
        case 3:
            PracticalWork3();
            break;
        case 4:
            PracticalWork4();
            break;
        case 666:
            return 0;
        default:
            cout << "Нет практической работы с таким номером";
        }
    }
}