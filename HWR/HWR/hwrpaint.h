#ifndef HWRPAINT_H
#define HWRPAINT_H

#include <QWidget>
#include "ui_hwrpaint.h"

#include <QMouseEvent>

#include <QPainterPath>

#include <QPen>

#include <QPixmap>


class HWRPaint : public QWidget
{
	Q_OBJECT

public:
	HWRPaint(QWidget *parent = 0);
	~HWRPaint();


protected:
	void mouseMoveEvent(QMouseEvent * event);
	void mousePressEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);
	void paintEvent(QPaintEvent * event);


private:
	void setupPainter(QPainter* p);
	void updateCache();
	void clear();
private:
	Ui::HWRPaint ui;

	QVector<QPainterPath>  m_strokes;
	QPainterPath		   m_currentstroke;

	bool m_bIsDrawing;


	QPen m_pen;
	
	QPixmap* m_pCache;
};

#endif // HWRPAINT_H
