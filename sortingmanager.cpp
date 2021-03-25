#include "sortingmanager.h"
#include <QMetaEnum>
#include <QtMath>
#include <QTimer>
#include <QThread>
#include <QRandomGenerator>
#include <Windows.h>

#define MAX 40
#define DEFAULT_SIZE 20

SortingManager::SortingManager()
{
    delay = 100;
    delayMutex = new QMutex(QMutex::Recursive);
}

SortingManager::~SortingManager()
{
    delete delayMutex;
}

void SortingManager::setList(QList<int>* values)
{
    dataList = values;
}

void SortingManager::pauseAndUpdate()
{
    int delay_copy;
    emit changedList();
    delayMutex->lock();
    delay_copy = delay;
    delayMutex->unlock();
    Sleep(delay_copy);
}

void SortingManager::SelectionSort()
{
   for (int k = 0; k<dataList->size()-1; k++)
   {
        for (int i = k+1; i<dataList->size(); i++)
        {
           if (dataList->at(k) > dataList->at(i)) {
                dataList->swapItemsAt(k, i);
                pauseAndUpdate();
           }
        }
   }

   emit done();
}

void SortingManager::BubbleSort()
{
   for (int k = dataList->size()-1; k>0; k--)
   {
        for (int i = 0; i<k; i++)
        {
           if (dataList->at(k) < dataList->at(i)) {
                dataList->swapItemsAt(k, i);
                pauseAndUpdate();
           }
        }
   }

   emit done();
}

void SortingManager::InsertionSort()
{
   for (int k = 1; k<dataList->size(); k++)
   {
        int j = k;
        for (int i = k-1; (j-1==i)&(j>0); i--)
        {
           if (dataList->at(j) < dataList->at(i)) {
                dataList->swapItemsAt(i, j);
                pauseAndUpdate();
                j--;
           }
        }
   }

   emit done();
}

void SortingManager::MergeSort(int i, int j, int size)
{
   if (i==j)
       return;
   int iter1=i, iter2;
   QList<int> entries;
   int m = i + (j-i)/2;
   MergeSort(i, m, size/2);
   MergeSort(m+1, j, size/2);
   iter2 = m+1;
   while(iter1 < m+1 && iter2 <= j)
   {
       if (dataList->at(iter1) < dataList->at(iter2))
       {
           entries.append(dataList->at(iter1));
           iter1++;
       } else if ((dataList->at(iter1) > dataList->at(iter2))) {
           entries.append(dataList->at(iter2));
           iter2++;
       } else {
           entries.append(dataList->at(iter2));
           iter2++;
           entries.append(dataList->at(iter1));
           iter1++;
       }
   }

   for(int k = iter1; k<m+1; k++)
   {
       entries.append(dataList->at(k));
   }

   for(int q = iter2; q<=j; q++)
   {
       entries.append(dataList->at(q));
   }

   for(int r = i, iter = 0; r<=j; r++, iter++)
   {
       dataList->replace(r, entries.at(iter));
   }
   pauseAndUpdate();

   if (size == dataList->size())
       emit done();
}

void SortingManager::CountingSort()
{

    QMap<int, int> countingMap;
    for (int i = 0; i<dataList->size(); i++)
    {
        int value = dataList->at(i);
        if (countingMap.contains(value))
        {
            int count = countingMap[value];
            count++;
            countingMap[value] = count;
        } else {
            int newVar = 1;
            countingMap.insert(value, newVar);
        }
    }

    QMapIterator<int, int> i(countingMap);
    i.next();
    int prevVal = i.value();
    while(i.hasNext()) {
        i.next();
        int key = i.key();
        countingMap[key] = countingMap[key] + prevVal;
        prevVal = countingMap[key];
    }
    QList<int> List_copy = *dataList;
    for (int i = 0; i<dataList->size(); i++)
    {
        int stored_val = List_copy.at(i);
        dataList->replace(countingMap[stored_val]-1, stored_val);
        countingMap[stored_val]--;
        pauseAndUpdate();
    }

    emit done();
}

void SortingManager::RadixSort()
{
    int power = 0;
    while(true) {
        int divisor = qFloor(qPow(10, power));
        QMap<int, int> *countingMap = new QMap<int, int>();
        for (int i = 0; i<dataList->size(); i++)
        {
            int value = dataList->at(i);
            value = qFloor(value/divisor);
            value = value % 10;
            if (countingMap->contains(value))
            {
                int count = countingMap->value(value);
                count++;
                countingMap->remove(value);
                countingMap->insert(value, count);
            } else {
                int newVar = 1;
                countingMap->insert(value, newVar);
            }
        }

        if (countingMap->contains(0) && countingMap->value(0) == dataList->size()) {
            delete countingMap;
            break;
        }

        QMapIterator<int, int> i(*countingMap);
        i.toBack();
        int prevVal = dataList->size();
        while(i.hasPrevious()) {
            i.previous();
            int key = i.key();
            prevVal = prevVal-countingMap->value(key);
            countingMap->remove(key);
            countingMap->insert(key, prevVal);
        }


        QList<int> List_copy = *dataList;
        for (int i = 0; i<dataList->size(); i++)
        {
            int stored_val = List_copy.at(i);
            int stored_val_div = qFloor(stored_val/divisor);
            stored_val_div = stored_val_div % 10;
            dataList->replace(countingMap->value(stored_val_div), stored_val);
            int count = countingMap->value(stored_val_div)+1;
            countingMap->remove(stored_val_div);
            countingMap->insert(stored_val_div, count);
            pauseAndUpdate();
        }
        delete countingMap;
        power++;
    }

    emit done();
}

void SortingManager::HeapSort()
{
    for (int i = dataList->size()/2 - 1; i >= 0; i--)
    {
        heapify(i, dataList->size());
    }

    for (int i = dataList->size()-1; i>0; i--)
    {
        dataList->swapItemsAt(0, i);
        pauseAndUpdate();
        heapify(0, i);
    }

    emit done();
}


void SortingManager::heapify(int start, int size)
{
    int l = start;
    int left = 2*start+1;
    int right = 2*start+2;


    if (left < size && dataList->at(left) > dataList->at(l)) {
        l = left;
    }

    if (right < size && dataList->at(right) > dataList->at(l)) {
        l = right;
    }

    if (l != start) {
       dataList->swapItemsAt(start, l);
       pauseAndUpdate();
       heapify(l, size);
    }
}

void SortingManager::QuickSort(int low, int high)
{
    if (low < high)
    {
        int pivot = partition(low, high);

        QuickSort(low, pivot-1);
        QuickSort(pivot+1, high);
    }
}

int SortingManager::partition(int low, int high)
{

    int pivot = dataList->at(low);
    int pivot_pos = high+1;
    for (int i = high; i>=low+1; i--)
    {
        if (pivot < dataList->at(i))
        {
            pivot_pos--;
            dataList->swapItemsAt(pivot_pos, i);
            pauseAndUpdate();
        }
    }

    dataList->swapItemsAt(low, pivot_pos-1);
    pauseAndUpdate();
    return pivot_pos-1;
}

void SortingManager::reset()
{
    int size = dataList->size();
    dataList->clear();
    for (int i = 0; i<size; i++) {
        int number = QRandomGenerator::global()->bounded(1, MAX);
        dataList->append(number);
    }

    emit changedList();
}

void SortingManager::generateDefaultList()
{
    for (int i = 0; i<DEFAULT_SIZE; i++)
    {
        int number = QRandomGenerator::global()->bounded(1, MAX);
        dataList->append(number);
    }
}

void SortingManager::updateDelay(int newDelay)
{
    delayMutex->lock();
    delay = newDelay;
    delayMutex->unlock();
}

void SortingManager::updateList(int value)
{
    while (value > dataList->size()) {
        int number = QRandomGenerator::global()->bounded(1, MAX);
        dataList->append(number);
    }

    while (value < dataList->size()) {
        dataList->pop_back();
    }

    emit changedList();
}

void SortingManager::execute(FUNCTIONS func)
{
    switch (func)
    {
        case Bubble:
            BubbleSort();
            break;
        case Selection:
            SelectionSort();
            break;
        case Insertion:
            InsertionSort();
            break;
        case Merge:
            MergeSort(0, dataList->size()-1, dataList->size());
            break;
       case Counting:
            CountingSort();
            break;
       case Radix:
            RadixSort();
            break;
       case Heap:
            HeapSort();
            break;
       case Quick:
            QuickSort(0, dataList->size()-1);
            emit done();
            break;
    }
}

QList<SortingManager::FUNCTIONS> SortingManager::getFunctionList()
{
    return functionList;
}
QString SortingManager::convertEnumTypeToString(FUNCTIONS func)
{
    return QString(QMetaEnum::fromType<SortingManager::FUNCTIONS>().valueToKey(func));
}
SortingManager::FUNCTIONS SortingManager::convertStringToEnumType(QString str)
{
    int ret = QMetaEnum::fromType<SortingManager::FUNCTIONS>().keyToValue(str.toStdString().c_str());
    return FUNCTIONS(ret);
}
