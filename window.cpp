#include "window.h"
#include "ui_window.h"
#include <QThread>
#include <QRandomGenerator>

Window::Window(QWidget *parent)
{

    dataList = new QList<int>();
    smThread = new QThread();

    sm = new SortingManager;
    displayArea = new DisplayArea;

    displayArea->setList(dataList);
    sm->setList(dataList);

    sm->generateDefaultList();

    QObject::connect(sm, &SortingManager::changedList,
                        displayArea, &DisplayArea::updatedList, Qt::QueuedConnection);

    QObject::connect(this, &Window::executeAlg,
                            sm, &SortingManager::execute, Qt::QueuedConnection);

    QObject::connect(sm, &SortingManager::done,
                        this, &Window::onFinish, Qt::QueuedConnection);

    QObject::connect(this, &Window::resetList,
                            sm, &SortingManager::reset, Qt::QueuedConnection);

    generateGroupBox();

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(displayArea);
    mainLayout->addWidget(sliderBox);
    setFixedSize(425, 490);
    setLayout(mainLayout);

    setWindowTitle(tr("Algorithm Visualization"));

    sm->moveToThread(smThread);
    smThread->start();
}

Window::~Window(){smThread->exit(); smThread->deleteLater(); delete sm; delete displayArea;}

void Window::handleStartButton()
{
    QString sortingAlg = algorithmListComboBox->currentText();
    SortingManager::FUNCTIONS func = sm->convertStringToEnumType(sortingAlg);
    sizeOfListSlider->setEnabled(false);
    startButton->setEnabled(false);
    emit executeAlg(func);
}

void Window::handleResetButton()
{
    emit resetList();
}

void Window::onFinish()
{
    sizeOfListSlider->setEnabled(true);
    startButton->setEnabled(true);
}

void Window::updateDelayValueLabel(int delay)
{
    QString label = QString("%1 ms").arg(delay);
    delayValueLabel->setText(label);
}

void Window::updateSizeValueLabel(int size)
{
    QString label = QString("%1").arg(size);
    sizeOfListValueLabel->setText(label);
}

void Window::generateGroupBox()
{

    startButton = new QPushButton("Start", this);
    connect(startButton, SIGNAL (released()), this, SLOT (handleStartButton()), Qt::DirectConnection);

    resetButton = new QPushButton("Reset", this);
    connect(resetButton, SIGNAL (released()), this, SLOT (handleResetButton()), Qt::DirectConnection);

    delaySlider = new QSlider(Qt::Horizontal);
    delaySlider->setFocusPolicy(Qt::StrongFocus);
    delaySlider->setTickPosition(QSlider::TicksBothSides);
    delaySlider->setTickInterval(10);
    delaySlider->setSingleStep(1);
    delaySlider->setRange(1, 1000);
    delaySlider->setValue(100);

    connect(delaySlider, &QSlider::valueChanged, sm, &SortingManager::updateDelay, Qt::QueuedConnection);
    connect(delaySlider, &QSlider::valueChanged, this, &Window::updateDelayValueLabel);

    sizeOfListSlider = new QSlider(Qt::Horizontal);
    sizeOfListSlider->setFocusPolicy(Qt::StrongFocus);
    sizeOfListSlider->setTickPosition(QSlider::TicksBothSides);
    sizeOfListSlider->setTickInterval(10);
    sizeOfListSlider->setSingleStep(1);
    sizeOfListSlider->setRange(10, 40);
    sizeOfListSlider->setValue(20);

    connect(sizeOfListSlider, &QSlider::valueChanged, sm, &SortingManager::updateList, Qt::QueuedConnection);
    connect(sizeOfListSlider, &QSlider::valueChanged, this, &Window::updateSizeValueLabel, Qt::DirectConnection);

    sliderBox = new QGroupBox(tr("Controls"));

    algorithmListLabel = new QLabel(tr("Algorithm: "));
    sizeOfListLabel = new QLabel(tr("Number of values:"));
    sizeOfListValueLabel = new QLabel(tr("20"));
    delayLabel = new QLabel(tr("Delay (ms)"));
    delayValueLabel = new QLabel(tr("100"));

    algorithmListComboBox = new QComboBox();

    QList<SortingManager::FUNCTIONS> List = sm->getFunctionList();
    for (int i = 0; i<List.size(); i++)
    {
        QString stringName = sm->convertEnumTypeToString(List.at(i));
        algorithmListComboBox->addItem(stringName);
    }

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setColumnStretch(0, 1);
    gridLayout->setColumnStretch(1, 2);
    gridLayout->setColumnStretch(1, 2);
    gridLayout->addWidget(delayLabel, 0, 0, 1, 1);
    gridLayout->addWidget(delayValueLabel, 0, 1, 1, 1);
    gridLayout->addWidget(delaySlider, 0, 2, 1, 1);
    gridLayout->addWidget(sizeOfListLabel, 1, 0, 1, 1);
    gridLayout->addWidget(sizeOfListValueLabel, 1, 1, 1, 1);
    gridLayout->addWidget(sizeOfListSlider, 1, 2, 1, 1);
    gridLayout->addWidget(algorithmListLabel, 2, 0, 1, 1);
    gridLayout->addWidget(algorithmListComboBox, 2, 2, 1, 1);
    gridLayout->addWidget(startButton, 3, 0, 1, 1);
    gridLayout->addWidget(resetButton, 3, 2, 1, 1);
    sliderBox->setLayout(gridLayout);
}


