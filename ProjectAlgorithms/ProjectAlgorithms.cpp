#include <iostream>
using namespace std;

#define TAB_LENGTH 100

struct FinalResultElement {
    int Count;
    FinalResultElement* next = nullptr;
};

struct ListElement {
    int ElementId;
    int* Result;
    ListElement* next = nullptr;
    FinalResultElement* ResultHead = nullptr;
};

struct Dec1 {
    int** factors;
    int numberOfFactors = 0;
};

bool prime(int a);
void AddToList(FinalResultElement*& head, int count);
void AddToList(ListElement*& head, int* result);
bool resultAlreadyExists(ListElement* list, int* searched);
bool isCorrectResult(ListElement* list, int k);
ListElement* GetNextElement(ListElement* list, int k);
void sortResult(int* result);
bool printResults(int* result, int n, Dec1* tabTst, int numberOfInits, int k, ListElement* list);
void fun(int szukana, Dec1*& tabTst);
void initInput(int* input, int indexToExclude, int* result, int& numberOfInits);
void calculateSums(ListElement* list, int k);
void RemoveElement(ListElement*& list, int elementId);
void RemoveList(FinalResultElement*& list);

bool prime(int a)
{
    for (int i = 2; i < a; i++)
    {
        if (a % i == 0)
            return false;
    }
    return true;
}

void AddToList(FinalResultElement*& head, int count)
{
    if (head == nullptr)
    {
        head = new FinalResultElement;
        head->Count = count;
    }
    else
    {
        FinalResultElement* temp = head;
        while (temp->next != nullptr)
        {
            temp = temp->next;
        }

        FinalResultElement* newElement = new FinalResultElement;
        newElement->Count = count;
        temp->next = newElement;
    }
}

void AddToList(ListElement*& head, int* result)
{
    if (head == nullptr)
    {
        head = new ListElement;
        head->ElementId = 0;
        head->Result = result;
    }
    else
    {
        ListElement* temp = head;
        while (temp->next != nullptr)
        {
            temp = temp->next;
        }

        ListElement* newElement = new ListElement;
        newElement->Result = result;
        newElement->ElementId = temp->ElementId + 1;
        temp->next = newElement;
    }
}

bool resultAlreadyExists(ListElement* list, int* searched)
{
    while (list != nullptr)
    {
        for (int j = 0; j < TAB_LENGTH; j++)
        {
            if (list->Result[j] != searched[j])
                break;
            if (list->Result[j] == -5 && searched[j] == -5)
                return true;
        }
        list = list->next;
    }
    return false;
}

bool isCorrectResult(ListElement* list, int k)
{
    for (int i = 0; i < TAB_LENGTH; i++)
    {
        if (list->Result[i] == -5)
            break;
        if (list->Result[i] < 2 || list->Result[i] > k || !prime(list->Result[i]))
            return false;
    }
    return true;
}

ListElement* GetNextElement(ListElement* list, int k)
{
    ListElement* selectedElement = nullptr;

    while (list != nullptr)
    {
        if (!isCorrectResult(list, k))
        {
            list = list->next;
            continue;
        }

        if (selectedElement == nullptr)
        {
            selectedElement = list;
            list = list->next;
            continue;
        }
        FinalResultElement* selectedResult = selectedElement->ResultHead;
        FinalResultElement* comparedResult = list->ResultHead;

        while (selectedResult != nullptr)
        {
            if (selectedResult->Count < comparedResult->Count)
                break;

            if (selectedResult->Count > comparedResult->Count)
            {
                selectedElement = list;
                break;
            }
            selectedResult = selectedResult->next;
            comparedResult = comparedResult->next;
        }

        list = list->next;
    }

    return selectedElement;
}

void sortResult(int* result)
{
    bool isSorted = false;
    while (!isSorted) {
        isSorted = true;
        for (int i = 0; i < TAB_LENGTH; i++)
        {
            if (result[i + 1] == -5)
                break;
            if (result[i + 1] > result[i]) {
                int temp = result[i];
                result[i] = result[i + 1];
                result[i + 1] = temp;
                isSorted = false;
            }
        }
    }
}

bool printResults(int* result, int n, Dec1* tabTst, int numberOfInits, int k, ListElement* list)
{
    int inits = numberOfInits;

    if (tabTst[n].factors[k][0] == -5)
        return false;

    result[inits++] = tabTst[n].factors[k][0];
    result[inits++] = tabTst[n].factors[k][1];
    result[inits] = -5;

    sortResult(result);

    if (!resultAlreadyExists(list, result))
    {
        return true;
    }
    return false;
}

void fun(int szukana, Dec1*& tabTst)
{
    if (tabTst[szukana].numberOfFactors != 0)
        return;
    for (int i = szukana - 1; i >= 2; i--)
    {
        if (!prime(i) || szukana - i < 2)
            continue;

        if (tabTst[szukana].factors[0][0] == szukana - i && tabTst[szukana].factors[0][1] == i)
            continue;

        tabTst[szukana].factors[tabTst[szukana].numberOfFactors][0] = i;
        tabTst[szukana].factors[tabTst[szukana].numberOfFactors][1] = szukana - i;

        //              tabTst[szukana].factors[tabTst[szukana].numberOfFactors + 1][0] = -5;
        tabTst[szukana].numberOfFactors++;

    }

    for (int i = 0; i < tabTst[szukana].numberOfFactors; i++)
    {
        if (tabTst[szukana].factors[i][0] > 3)
            fun(tabTst[szukana].factors[i][0], tabTst);
        if (tabTst[szukana].factors[i][1] > 3)
            fun(tabTst[szukana].factors[i][1], tabTst);
    }
}

void initInput(int* input, int indexToExclude, int* result, int& numberOfInits)
{
    for (int i = 0; i < TAB_LENGTH; i++)
    {
        if (i == indexToExclude)
            continue;
        if (result[i] == -5)
            return;

        input[numberOfInits++] = result[i];
        input[numberOfInits] = -5;
    }
}

void calculateSums(ListElement* list, int k)
{
    while (list != nullptr)
    {
        for (int i = k; i >= 2; i--)
        {
            int count = 0;
            if (!prime(i))
            {
                continue;
            }
            for (int j = 0; j < TAB_LENGTH; j++)
            {
                if (list->Result[j] == i)
                {
                    count++;
                }
            }
            AddToList(list->ResultHead, count);
        }
        list = list->next;
    }
}


void RemoveElement(ListElement*& list, int elementId)
{
    if (list->ElementId == elementId)
    {
        ListElement* elementToRemove = list;
        list = list->next;

        RemoveList(elementToRemove->ResultHead);
        delete[]elementToRemove->Result;
        delete elementToRemove;
        return;
    }

    ListElement* copyList = list;
    while (copyList->next->ElementId != elementId)
    {
        copyList = copyList->next;
    }

    ListElement* elementToRemove = copyList->next;
    copyList->next = elementToRemove->next;

    RemoveList(elementToRemove->ResultHead);

    delete[]elementToRemove->Result;
    delete elementToRemove;
}

void RemoveList(FinalResultElement*& list)
{
    while (list != nullptr)
    {
        FinalResultElement* elementToRemove = list;
        list = list->next;
        delete elementToRemove;
    }
}

int main()
{
    int number_of_lines = 0;
    cin >> number_of_lines;
    for (int number_of_line = 0; number_of_line < number_of_lines; number_of_line++)
    {
        int n = 0;
        int k = 0;
        cin >> n >> k;

        ListElement* list = nullptr;

        if (!prime(k))
            continue;
        if (n == k && n > 1 && k > 1)
        {
            cout << k << endl;
            continue;
        }

        Dec1* tabTst = new Dec1[n - k + 1];
        for (int i = 0; i < n - k + 1; i++)
        {
            tabTst[i].factors = new int* [255];
            for (int j = 0; j < 255; j++)
            {
                tabTst[i].factors[j] = new int[2];
            }
        }

        for (int i = 0; i < 4; i++)
        {
            tabTst[i].numberOfFactors = 0;
        }
        fun(n - k, tabTst);

        int* tempResult = new int[TAB_LENGTH];
        tempResult[0] = n - k;
        tempResult[1] = -5;
        AddToList(list, tempResult);

        ListElement* copyList = list;
        while (copyList != nullptr)
        {
            for (int j = 0; j < TAB_LENGTH; j++)
            {
                if (copyList->Result[j] == -5)
                    break;

                if (copyList->Result[j] > 3)
                {
                    for (int l = 0; l < tabTst[copyList->Result[j]].numberOfFactors; l++)
                    {
                        int numberOfInits = 0;
                        int* result = new int[TAB_LENGTH];
                        initInput(result, j, copyList->Result, numberOfInits);

                        if (printResults(result, copyList->Result[j], tabTst, numberOfInits, l, list))
                            AddToList(list, result);
                    }
                }
            }
            copyList = copyList->next;
        }

        calculateSums(list, k);

        ListElement* nextElement = GetNextElement(list, k);

        while (nextElement != nullptr)
        {
            for (int j = 0; j < TAB_LENGTH; j++)
            {
                if (nextElement->Result[j] == -5)
                {
                    cout << endl;
                    break;
                }

                if (j == 0)
                    cout << k << "+";

                cout << nextElement->Result[j];

                if (nextElement->Result[j + 1] != -5)
                    cout << "+";
            }

            RemoveElement(list, nextElement->ElementId);
            nextElement = GetNextElement(list, k);


        }
        for (int i = 0; i < n - k + 1; i++)
        {
            delete[]tabTst[i].factors;
        }
        delete[] tabTst;
    }
}