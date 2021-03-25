#ifndef SORTINGMANAGER_H
#define SORTINGMANAGER_H

#include <QObject>
#include <QMutex>

class SortingManager : public QObject
{
    Q_OBJECT

public:

    enum FUNCTIONS {Selection, Bubble, Insertion,
                    Merge, Counting, Radix,
                    Heap, Quick } ;

    Q_ENUM(FUNCTIONS)

    SortingManager();
    ~SortingManager();

    void setList(QList<int>* values);
    void generateDefaultList();
    QList<FUNCTIONS> getFunctionList();
    QString convertEnumTypeToString(FUNCTIONS func);
    FUNCTIONS convertStringToEnumType(QString str);

public slots:
    void execute(FUNCTIONS func);
    void reset();
    void updateDelay(int newDelay);
    void updateList(int value);

signals:
    void changedList();
    void done();

private:
    void SelectionSort();
    void BubbleSort();
    void InsertionSort();
    void MergeSort(int i, int j, int size);
    void CountingSort();
    void RadixSort();
    void HeapSort();
    void QuickSort(int low, int high);
    void heapify(int start, int size);
    int partition(int low, int high);
    void pauseAndUpdate();

    QList<FUNCTIONS> functionList = QList<FUNCTIONS>() << Selection << Bubble
                                                       << Insertion << Merge
                                                       << Counting << Radix
                                                       << Heap << Quick;
    QList<int> *dataList;
    QMutex* delayMutex;
    int* List;
    int delay;

};

#endif // SORTINGMANAGER_H
