#if !defined(_HWR_CANVAS_H_)
#define _HWR_CANVAS_H_ (1)


#include <QTimer>
#include <QMouseEvent>
#include <QPaintEvent>


#include <QWidget>

#include <QList>

#include <QPen>

#include <QPainterPath>

#include "abstractrecognizer.h"

#define SMOOTHING			6

class HWRCanvas : public QWidget {
	Q_OBJECT

public:
	HWRCanvas(QWidget * parent = 0);
	~HWRCanvas(void) { }

	void setRecognizer(AbstractRecognizer* recognizer);

	void clear();
protected:
	void mouseMoveEvent(QMouseEvent * event);
	void mousePressEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);
	void paintEvent(QPaintEvent * event);

protected slots:
	void recognize(void);

	
private:
	void generateCurrentPath();

private:
	QList<QPainterPath>  m_paths;
	QPainterPath		   m_currcentPath;

	XYPOINTS handwritingX;
	XYPOINTS handwritingY;

	STROKE w;
	STROKES trace;

	QPainterPath* m_pCrntPath;
	QList<QPainterPath*> m_lPreviousPath;

	AbstractRecognizer* m_recognizer;

	static size_t m_writor_width_fixed;
	static size_t m_writor_height_fixed;
	static int m_writor_pen_w_fixed;

	bool m_isDrawing;
};
#endif /* !defined(_HWR_CANVAS_H_) */