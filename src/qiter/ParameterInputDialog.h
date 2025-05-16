//
// Created by toky on 3/12/24.
//

#ifndef KITER_PARAMETERINPUTDIALOG_H
#define KITER_PARAMETERINPUTDIALOG_H

#include <QDialog>
#include <QMap>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QFormLayout>
#include "printers/printers.h" // For parameters_list_t

class ParameterInputDialog : public QDialog
{
Q_OBJECT

public:
    explicit ParameterInputDialog(const QString &generatorName, QWidget *parent = nullptr);
    parameters_list_t getParameters() const;

private slots:
    void accept() override;
    void onAddParameterClicked();

private:
    QString generatorName;
    parameters_list_t parameters;
    QList<QLineEdit*> paramNameEdits;
    QList<QLineEdit*> paramValueEdits;
    QVBoxLayout *layout;
    QFormLayout *formLayout;
};

#endif //KITER_PARAMETERINPUTDIALOG_H
