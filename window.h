#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QGridLayout>
 #include <QPushButton>
#include <QSlider>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <sortingmanager.h>
#include <displayarea.h>
namespace Ui {
class Window;
}

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = nullptr);
    ~Window();

    QList<int> *dataList;

public slots:
    void onFinish();

private slots:
    void handleStartButton();
    void handleResetButton();
    void updateDelayValueLabel(int delay);
    void updateSizeValueLabel(int size);

signals:
    void executeAlg(SortingManager::FUNCTIONS func);
    void resetList();

private:
    void generateGroupBox();


    QVBoxLayout *mainLayout;
    QPushButton *startButton, *resetButton;
    QSlider *delaySlider, *sizeOfListSlider;
    QGroupBox *sliderBox;
    QLabel *delayLabel, *sizeOfListLabel, *algorithmListLabel, *delayValueLabel, *sizeOfListValueLabel;
    QComboBox *algorithmListComboBox;
    SortingManager *sm;
    DisplayArea *displayArea;
    QThread* smThread;


};

#endif // WINDOW_H
