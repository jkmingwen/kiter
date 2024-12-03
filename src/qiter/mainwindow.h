#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graphwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loadGraph();
    void onGenerateActionTriggered();
    void onTransformationActionTriggered();
    void onBufferSizingActionTriggered();
    void onPrinterActionTriggered();

private:
    Ui::MainWindow *ui;
    GraphWidget *graphWidget;

    template <typename ActionType, typename ResultType, typename ActionFunction, typename PostAction>
    void handleAction(ActionFunction actionFunction, PostAction postAction) ;
};
#endif // MAINWINDOW_H
