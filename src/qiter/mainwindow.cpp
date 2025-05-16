#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include "printers/printers.h"
#include "commons/KiterRegistry.h"
#include "generators/generators.h"
#include "ParameterInputDialog.h"
#include <QProgressDialog>
#include <QFuture>
#include <QThread>
#include <QtConcurrent>
#include <models/BufferSizingResult.h>

template<typename R, typename T>
void MainWindow::create_menu (std::string text,T* receiver, void (T::*slot)() ) {

    // Create menu dynamically
    auto *newMenu = new QMenu(tr(text.c_str()), receiver);
    ui->menubar->addMenu(newMenu);


     // Get the list from KiterRegistry
    std::vector<std::string> actionsNames = KiterRegistry<R>::get_names();

    for (const std::string &actionsName : actionsNames) {
        auto *action = new QAction(QString::fromStdString(actionsName), receiver);
        action->setData(QString::fromStdString(actionsName));
        newMenu->addAction(action);
        connect(action, &QAction::triggered, receiver, slot);
    }

}


MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
        , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    graphWidget = findChild<GraphWidget*>("graphWidget");
    if (!graphWidget) {
        qInfo() << "Error: GraphWidget not found!";
    } else {
        qInfo() << "GraphWidget found successfully.";
    }

    connect(ui->actionLoad, &QAction::triggered, this, &MainWindow::loadGraph);

    create_menu<generator_action_t>("&Generate",  this, &MainWindow::onGenerateActionTriggered);
    create_menu<transformation_action_t>("&Transformations", this, &MainWindow::onTransformationActionTriggered);
    create_menu<buffer_sizing_action_t>("&Buffer Sizing", this, &MainWindow::onBufferSizingActionTriggered);
    create_menu<printer_action_t>("&Printers", this, &MainWindow::onPrinterActionTriggered);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::loadGraph()
{
    qInfo() << "Attempting to open file dialog for XML file selection...";
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open XML File"), "",
                                                    tr("XML Files (*.xml);;All Files (*)"));

    if (fileName.isEmpty()) {
        qInfo() << "No file selected or dialog canceled.";
        return;
    }

    qInfo() << "Selected file:" << fileName;

    qInfo() << "Reading dataflow from file...";
    models::Dataflow* dataflow = printers::readSDF3File(fileName.toStdString());

    if (dataflow == nullptr) {
        QMessageBox::critical(this, tr("Error"), tr("Failed to load the graph from the file."));
        qInfo() << "Failed to load the graph from the file:" << fileName;
        return;
    }

    qInfo() << "Dataflow loaded successfully, updating graph widget...";

    graphWidget->setDataflow(dataflow);
}
void MainWindow::onTransformationActionTriggered()
{
    handleAction<transformation_action_t, void>(
            [this](const transformation_action_t *actionInstance, const parameters_list_t &parameters) {
                // Ensure dataflow is loaded
                models::Dataflow *dataflow = graphWidget->getDataflow();
                if (!dataflow) {
                    QMessageBox::warning(this, tr("No Dataflow"), tr("Please load or generate a dataflow graph first."));
                    throw std::runtime_error("No dataflow loaded");
                }
                // Perform the action
                actionInstance->fun(dataflow, parameters);
            },
            [this]() {
                // Post-action processing
                graphWidget->update(); // Update the graph display
            });
}


void MainWindow::onPrinterActionTriggered()
{
    handleAction<printer_action_t, void>(
            [this](const printer_action_t *actionInstance, const parameters_list_t &parameters) {
                models::Dataflow *dataflow = graphWidget->getDataflow();
                if (!dataflow) {
                    QMessageBox::warning(this, tr("No Dataflow"), tr("Please load or generate a dataflow graph first."));
                    throw std::runtime_error("No dataflow loaded");
                }
                actionInstance->fun(dataflow, parameters);
            },
            []() {
                // Post-action processing
                qInfo() << "Printer executed successfully.";
            });
}


void MainWindow::onBufferSizingActionTriggered()
{
    handleAction<buffer_sizing_action_t, models::BufferSizingResult>(
            [this](const buffer_sizing_action_t *actionInstance, const parameters_list_t &parameters) -> models::BufferSizingResult {
                models::Dataflow *dataflow = graphWidget->getDataflow();
                if (!dataflow) {
                    QMessageBox::warning(this, tr("No Dataflow"), tr("Please load or generate a dataflow graph first."));
                    throw std::runtime_error("No dataflow loaded");
                }
                return actionInstance->fun(dataflow, parameters);
            },
            [this](const models::BufferSizingResult &res) {
                // Post-action processing
                qInfo() << "Total buffer size:" << res.total_size();
                QMessageBox::information(this, tr("Buffer Sizing Result"),
                                         tr("Total buffer size: %1").arg(res.total_size()));
            });
}



void MainWindow::onGenerateActionTriggered()
{
    handleAction<generator_action_t, models::Dataflow *>(
            [](const generator_action_t *actionInstance, const parameters_list_t &parameters) -> models::Dataflow * {
                return actionInstance->fun(parameters);
            },
            [this](models::Dataflow *newDataflow) {
                if (!newDataflow) {
                    QMessageBox::critical(this, tr("Error"), tr("Failed to generate the graph."));
                    qWarning() << "Failed to generate the graph.";
                    return;
                }
                qInfo() << "Dataflow generated successfully, updating graph widget...";
                graphWidget->setDataflow(newDataflow);
            });
}

template <typename ActionType, typename ResultType, typename ActionFunction, typename PostAction>
void MainWindow::handleAction(ActionFunction actionFunction, PostAction postAction) {
    try {
        auto *actionSender = qobject_cast<QAction *>(sender());
        if (!actionSender) {
            qWarning() << "Sender is not a QAction!";
            return;
        }
        const QString actionName = actionSender->data().toString();
        qInfo() << "Action selected:" << actionName;

        // Show a dialog to input parameters
        ParameterInputDialog dialog(actionName, this);
        if (dialog.exec() != QDialog::Accepted) {
            qInfo() << "Action dialog canceled.";
            return;
        }

        parameters_list_t parameters = dialog.getParameters();

        const ActionType *actionInstance = KiterRegistry<ActionType>::get(actionName.toStdString());
        if (!actionInstance) {
            QMessageBox::critical(this, tr("Error"), tr("Action not found."));
            qWarning() << "Action not found:" << actionName;
            return;
        }

        // Progress dialog setup
        QProgressDialog progressDialog(tr("Processing..."), tr("Cancel"), 0, 0, this);
        progressDialog.setWindowModality(Qt::WindowModal);
        progressDialog.setCancelButton(nullptr); // Prevent cancellation for now
        progressDialog.setMinimumDuration(0);
        progressDialog.show();

        qInfo() << "Performing action...";

        // Run the action asynchronously using QtConcurrent::run
        QFuture<void> future = QtConcurrent::run([=]() {
            if constexpr (std::is_same<ResultType, void>::value) {
                // If ResultType is void
                actionFunction(actionInstance, parameters);
            } else {
                // If ResultType is not void
                ResultType result = actionFunction(actionInstance, parameters);
                QMetaObject::invokeMethod(this, [=]() {
                    postAction(result);
                });
            }
        });

        // Keep the UI responsive and monitor the progress
        QFutureWatcher<void> watcher;
        connect(&watcher, &QFutureWatcher<void>::finished, &progressDialog, &QProgressDialog::accept);
        watcher.setFuture(future);

        // Show the progress dialog until the task is completed
        progressDialog.exec();

        if (progressDialog.wasCanceled()) {
            qInfo() << "Action canceled.";
        } else {
            qInfo() << "Action completed.";
            if constexpr (std::is_same<ResultType, void>::value) {
                postAction();
            }
        }


    } catch (const std::exception &e) {
        QMessageBox::critical(this, tr("Error"), e.what());
        qWarning() << "Exception occurred:" << e.what();
    }
}
