/* ===========================================================================
 *
 *       Filename:  LineStroke.h
 *
 *    Description:  Line Stroke (笔画)
 *
 * ===========================================================================
 */


#if !defined(_LINESTROKE_H_)
#define _LINESTROKE_H_ (1)


#include <QtCore/QPoint> /* QPoint */
#include <QtCore/QVector> /* QVector */


class LineStroke {
public: LineStroke(void) { segments = 0; }
public: LineStroke(const LineStroke& ls) {
	segments = ls.segments;
	foreach(QPoint * point, ls.points) {
		QPoint * p = new QPoint((*point).x(), (*point).y());
		points.append(p);
	} /* foreach */
} /* LineStroke */

public: ~LineStroke(void) {
	foreach(QPoint * point, points) {
		delete point;
	}
	points.clear();
	segments = 0;
} /* ~LineStroke */

public: int segments;/* 包含有的段数目 */
public: QVector <QPoint *> points;/* 包含的用来构成笔画的点 */
};/* class LineStroke */

#endif /* !defined(_LINESTROKE_H_) */


/*
 * end of file LineStroke.h
 */
