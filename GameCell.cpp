#include "GameCell.h"

GameCell::GameCell(int x, int y, QWidget *parent) :
	QPushButton(parent), posX(x), posY(y), mine(false), number(0), revealed(false), flagged(false)
{
	setFixedSize(30, 30);
	setStyleSheet("background-color: rgb(189, 189, 189);");
	mineIcon = QIcon(":/icons/bomb.png");
	flagIcon = QIcon(":/icons/flag.png");
	emptyIcon = QIcon(":/icons/empty.png");
	cellIcon = QIcon(":/icons/cell.png");
	setIcon(cellIcon);
	setIconSize(QSize(24, 24));
	QFont monoFont("Courier New", 18, QFont::Bold);
	setFont(monoFont);
	setText("");
}

void GameCell::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QStyleOptionButton option;
	option.initFrom(this);
	style()->drawControl(QStyle::CE_PushButton, &option, &painter, this);

	int iconPadding = 7;

	QRect iconRect = QRect(iconPadding, iconPadding, width() - 2 * iconPadding, height() - 2 * iconPadding);
	QRect textRect = QRect(iconPadding, iconPadding, width() - 2 * iconPadding, height() - 2 * iconPadding);

	if (!icon().isNull())
	{
		icon().paint(&painter, iconRect);
	}

	if (!text().isEmpty())
	{
		QFont font = this->font();
		painter.setFont(font);
		painter.setPen(getColorForNumber(number));
		painter.drawText(textRect, Qt::AlignCenter, text());
	}
}

void GameCell::setMine(bool hasMine)
{
	mine = hasMine;
}

bool GameCell::hasMine() const
{
	return mine;
}

void GameCell::setNumber(int num)
{
	number = num;
}

int GameCell::getNumber() const
{
	return number;
}

void GameCell::reveal()
{
	if (revealed)
		return;
	revealed = true;
	if (mine)
	{
		setIcon(mineIcon);
	}
	else if (number > 0)
	{
		setIcon(emptyIcon);
		setText(QString::number(number));
	}
	else
	{
		setIcon(emptyIcon);
	}
	update();
}

bool GameCell::isRevealed() const
{
	return revealed;
}

void GameCell::toggleFlag()
{
	if (revealed)
		return;
	flagged = !flagged;
	if (flagged)
	{
		setIcon(flagIcon);
	}
	else
	{
		setIcon(cellIcon);
	}
	update();
}

bool GameCell::isFlagged() const
{
	return flagged;
}

void GameCell::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		emit cellClicked(posX, posY);
	}
	else if (event->button() == Qt::RightButton)
	{
		emit cellRightClicked(posX, posY);
	}
	else if (event->button() == Qt::MiddleButton || event->button() == Qt::ForwardButton || event->button() == Qt::BackButton)
	{
		emit middleButtonPressed(posX, posY);
	}
}

void GameCell::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::MiddleButton || event->button() == Qt::ForwardButton || event->button() == Qt::BackButton)
	{
		emit middleButtonReleased(posX, posY);
	}
}

void GameCell::setTemporaryIcon(bool temporary)
{
	temporaryIcon = temporary;
	if (temporaryIcon)
	{
		setIcon(emptyIcon);
	}
	else
	{
		setIcon(cellIcon);
	}
}

QColor GameCell::getColorForNumber(int number)
{
	switch (number)
	{
	case 1:
		return QColor(Qt::darkBlue);
	case 2:
		return QColor(Qt::darkGreen);
	case 3:
		return QColor(Qt::darkRed);
	case 4:
		return QColor(Qt::blue);
	case 5:
		return QColor(Qt::red);
	case 6:
		return QColor(Qt::cyan);
	case 7:
		return QColor(Qt::black);
	case 8:
		return QColor(Qt::gray);
	default:
		return QColor(Qt::black);
	}
}
