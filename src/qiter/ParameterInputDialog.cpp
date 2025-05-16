//
// Created by toky on 3/12/24.
//

#include "ParameterInputDialog.h"
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "commons/KiterRegistry.h"
#include "generators/generators.h"

ParameterInputDialog::ParameterInputDialog(const QString &generatorName, QWidget *parent)
        : QDialog(parent), generatorName(generatorName)
{
    setWindowTitle(tr("Parameters for %1").arg(generatorName));
    layout = new QVBoxLayout(this);

    QLabel *infoLabel = new QLabel(tr("Enter parameters (name and value):"), this);
    layout->addWidget(infoLabel);

    formLayout = new QFormLayout;

    // Add an initial parameter row
    QLineEdit *paramNameEdit = new QLineEdit(this);
    QLineEdit *paramValueEdit = new QLineEdit(this);
    paramNameEdits.append(paramNameEdit);
    paramValueEdits.append(paramValueEdit);
    formLayout->addRow(paramNameEdit, paramValueEdit);

    layout->addLayout(formLayout);

    // Add "Add Parameter" button
    QPushButton *addParamButton = new QPushButton(tr("Add Parameter"), this);
    connect(addParamButton, &QPushButton::clicked, this, &ParameterInputDialog::onAddParameterClicked);
    layout->addWidget(addParamButton);

    // Add OK and Cancel buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &ParameterInputDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ParameterInputDialog::reject);

    layout->addWidget(buttonBox);
}

void ParameterInputDialog::onAddParameterClicked()
{
    QLineEdit *paramNameEdit = new QLineEdit(this);
    QLineEdit *paramValueEdit = new QLineEdit(this);
    paramNameEdits.append(paramNameEdit);
    paramValueEdits.append(paramValueEdit);
    formLayout->addRow(paramNameEdit, paramValueEdit);
}

void ParameterInputDialog::accept()
{
    // Collect parameters
    for (int i = 0; i < paramNameEdits.size(); ++i) {
        QString paramName = paramNameEdits[i]->text();
        QString paramValue = paramValueEdits[i]->text();
        if (!paramName.isEmpty()) {
            parameters[paramName.toStdString()] = paramValue.toStdString();
        }
    }

    QDialog::accept();
}

parameters_list_t ParameterInputDialog::getParameters() const
{
    return parameters;
}
