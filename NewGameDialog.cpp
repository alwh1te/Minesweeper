#include "NewGameDialog.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QIntValidator>
#include <QDialogButtonBox>
#include <QMessageBox>

NewGameDialog::NewGameDialog(QWidget *parent) : QDialog(parent), width(10), height(10), mines(10) {
    QFormLayout *layout = new QFormLayout(this);

    widthEdit = new QLineEdit(QString::number(width), this);
    heightEdit = new QLineEdit(QString::number(height), this);
    minesEdit = new QLineEdit(QString::number(mines), this);

    QIntValidator *sizeValidator = new QIntValidator(2, 100, this);
    QIntValidator *minesValidator = new QIntValidator(1, 9999, this);

    widthEdit->setValidator(sizeValidator);
    heightEdit->setValidator(sizeValidator);
    minesEdit->setValidator(minesValidator);

    layout->addRow(tr("Width:"), widthEdit);
    layout->addRow(tr("Height:"), heightEdit);
    layout->addRow(tr("Mines:"), minesEdit);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &NewGameDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &NewGameDialog::reject);

    layout->addWidget(buttonBox);
    setLayout(layout);
}

int NewGameDialog::getWidth() const {
    return width;
}

int NewGameDialog::getHeight() const {
    return height;
}

int NewGameDialog::getMines() const {
    return mines;
}

void NewGameDialog::accept() {
    bool widthOk, heightOk, minesOk;
    int w = widthEdit->text().toInt(&widthOk);
    int h = heightEdit->text().toInt(&heightOk);
    int m = minesEdit->text().toInt(&minesOk);

    if (widthOk && heightOk && minesOk && w > 0 && h > 0 && m > 0 && m < w * h) {
        width = w;
        height = h;
        mines = m;
        QDialog::accept();
    } else {
        QMessageBox::warning(this, tr("Invalid Input"), tr("Please enter valid numbers. Mines must be less than the total number of cells and greater than 0."));
    }
}
