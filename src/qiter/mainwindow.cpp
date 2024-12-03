#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include "printers/printers.h"
#include "commons/KiterRegistry.h"
#include "generators/generators.h"
#include "ParameterInputDialog.h"

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

    // Create Generate menu dynamically
    QMenu *generateMenu = new QMenu(tr("&Generate"), this);
    ui->menubar->addMenu(generateMenu);

    // Get the list of generators from KiterRegistry
    std::vector<std::string> generatorNames = KiterRegistry<generator_t>::get_names();

    for (const std::string &generatorName : generatorNames) {
        QAction *action = new QAction(QString::fromStdString(generatorName), this);
        action->setData(QString::fromStdString(generatorName));
        generateMenu->addAction(action);
        connect(action, &QAction::triggered, this, &MainWindow::onGenerateActionTriggered);
    }

    // Create Transformations menu dynamically
    QMenu *transformationsMenu = new QMenu(tr("&Transformations"), this);
    ui->menubar->addMenu(transformationsMenu);

    // Get the list of transformations from KiterRegistry
    std::vector<std::string> transformationNames = KiterRegistry<transformation_t>::get_names();

    for (const std::string &transformationName : transformationNames) {
        QAction *action = new QAction(QString::fromStdString(transformationName), this);
        action->setData(QString::fromStdString(transformationName));
        transformationsMenu->addAction(action);
        connect(action, &QAction::triggered, this, &MainWindow::onTransformationActionTriggered);
    }

    // Create Buffer Sizing menu dynamically
    QMenu *bufferSizingMenu = new QMenu(tr("&Buffer Sizing"), this);
    ui->menubar->addMenu(bufferSizingMenu);

    // Get the list of buffer sizings from KiterRegistry
    std::vector<std::string> bufferSizingNames = KiterRegistry<buffer_sizing_t>::get_names();

    for (const std::string &bufferSizingName : bufferSizingNames) {
        QAction *action = new QAction(QString::fromStdString(bufferSizingName), this);
        action->setData(QString::fromStdString(bufferSizingName));
        bufferSizingMenu->addAction(action);
        connect(action, &QAction::triggered, this, &MainWindow::onBufferSizingActionTriggered);
    }

    // Create Printers menu dynamically
    QMenu *printersMenu = new QMenu(tr("&Printers"), this);
    ui->menubar->addMenu(printersMenu);

    // Get the list of printers from KiterRegistry
    std::vector<std::string> printerNames = KiterRegistry<printer_t>::get_names();

    for (const std::string &printerName : printerNames) {
        QAction *action = new QAction(QString::fromStdString(printerName), this);
        action->setData(QString::fromStdString(printerName));
        printersMenu->addAction(action);
        connect(action, &QAction::triggered, this, &MainWindow::onPrinterActionTriggered);
    }
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
    handleAction<transformation_t, void>(
            [this](const transformation_t *actionInstance, const parameters_list_t &parameters) {
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
    handleAction<printer_t, void>(
            [this](const printer_t *actionInstance, const parameters_list_t &parameters) {
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
    handleAction<buffer_sizing_t, BufferSizingResult>(
            [this](const buffer_sizing_t *actionInstance, const parameters_list_t &parameters) -> BufferSizingResult {
                models::Dataflow *dataflow = graphWidget->getDataflow();
                if (!dataflow) {
                    QMessageBox::warning(this, tr("No Dataflow"), tr("Please load or generate a dataflow graph first."));
                    throw std::runtime_error("No dataflow loaded");
                }
                return actionInstance->fun(dataflow, parameters);
            },
            [this](const BufferSizingResult &res) {
                // Post-action processing
                qInfo() << "Total buffer size:" << res.total_size();
                QMessageBox::information(this, tr("Buffer Sizing Result"),
                                         tr("Total buffer size: %1").arg(res.total_size()));
            });
}



void MainWindow::onGenerateActionTriggered()
{
    handleAction<generator_t, models::Dataflow *>(
            [](const generator_t *actionInstance, const parameters_list_t &parameters) -> models::Dataflow * {
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
        QAction *actionSender = qobject_cast<QAction *>(sender());
        if (!actionSender) {
            qWarning() << "Sender is not a QAction!";
            return;
        }
        QString actionName = actionSender->data().toString();
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

        qInfo() << "Performing action...";

        // Run the action on the main thread
        if constexpr (std::is_same<ResultType, void>::value) {
            // If ResultType is void, call the function without assigning to a variable
            actionFunction(actionInstance, parameters);
            qInfo() << "Action completed.";
            // Process the result using the provided post-action function
            postAction();
        } else {
            // If ResultType is not void, assign the result to a variable
            ResultType result = actionFunction(actionInstance, parameters);
            qInfo() << "Action completed.";
            // Process the result using the provided post-action function
            postAction(result);
        }


    } catch (const std::exception &e) {
        QMessageBox::critical(this, tr("Error"), e.what());
        qWarning() << "Exception occurred:" << e.what();
    }
}
