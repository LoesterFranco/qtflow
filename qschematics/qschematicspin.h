#ifndef QSCHEMATICSPIN_H
#define QSCHEMATICSPIN_H

#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSimpleTextItem>
#include <QDebug>

class QSchematicsPin : public QGraphicsItem
{
public:
	QSchematicsPin(QString name, int index, int x, int y, int length, QString orient, int tw, int th, int a, int b, QString mode, QGraphicsItem* p);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	QRectF boundingRect() const;

private:
	QLineF drawLine(int x, int y, int length, QString orient);

	QRectF m_externalRect;
	QGraphicsLineItem *m_pinLine;
	QGraphicsSimpleTextItem *m_pinLabel;
	QGraphicsRectItem *m_pinContact;
};

#endif // QSCHEMATICSPIN_H