#pragma once

#ifndef MINESWEEPER_NEWGAMEDIALOG_H
#define MINESWEEPER_NEWGAMEDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QIntValidator>
#include <QLineEdit>
#include <QMessageBox>

class QLineEdit;

class NewGameDialog : public QDialog
{
	Q_OBJECT

  public:
	explicit NewGameDialog(QWidget *parent = nullptr);
	int getWidth() const;
	int getHeight() const;
	int getMines() const;

  private slots:
	void accept() override;

  private:
	QLineEdit *widthEdit;
	QLineEdit *heightEdit;
	QLineEdit *minesEdit;
	int width;
	int height;
	int mines;
};

#endif	  // MINESWEEPER_NEWGAMEDIALOG_H
