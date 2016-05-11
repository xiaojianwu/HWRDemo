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
	QList<QPainterPath>  m_paths;
	QPainterPath		   m_currcentPath;

	XYPOINTS m_handwritingX;
	XYPOINTS m_handwritingY;

	STROKE m_currentStroke;
	STROKES m_strokes;

	QPainterPath* m_pCrntPath;
	QList<QPainterPath*> m_lPreviousPath;

	AbstractRecognizer* m_recognizer;

	static size_t CANVAS_WIDTH;
	static size_t CANVAS_HEIGHT;
	static int PEN_SIZE;

	bool m_isDrawing;
};
#endif /* !defined(_HWR_CANVAS_H_) */