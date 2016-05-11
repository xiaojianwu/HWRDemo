#if !defined(_handwritor2_H_)
#define _handwritor2_H_ (1)


#include <QtCore/QTimer>
#include <QMouseEvent>
#include <QPaintEvent>


#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>


#include <QList>

#include <QPen>

#include <QPainterPath>

#include "abstractrecognizer.h"


#define handwritor2_MODEL_DFT "handwriting-zh_CN.model"
#define handwritor2_ERR_MIN (64)

#define SMOOTHING			6

class HWRCanvas : public QWidget {
	Q_OBJECT

		/* err min handwritor2_ERR_MIN */
public:
	HWRCanvas(QWidget * parent = 0);
	~HWRCanvas(void) { destroy(); }
	int destroy(void);


	/* evt */
protected:
	void mouseMoveEvent(QMouseEvent * event);
	void mousePressEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);
	void paintEvent(QPaintEvent * event);

	void genbuttonstate(void);



protected slots:
	void recognize(void);
	void turnpageup(void);
	void turnpagedown(void);
	void clear();


private:
	void generateCurrentPath();

private:
	QVBoxLayout * master_layout;
	QHBoxLayout * writor_layout;
	QVBoxLayout * option_layout;
	QHBoxLayout * candidate_layout;
	QPushButton* candidate_btns[10];
	QPushButton *up;
	QPushButton *down;
	QPushButton *m_btnClear;


	QTimer m_recogtimer;
	QStringList dstr;
	int index;
	int allpage;


	QList<QPainterPath>  m_paths;
	QPainterPath		   m_currcentPath;

	XYPOINTS handwritingX;
	XYPOINTS handwritingY;

	STROKE w;
	STROKES trace;



	QPainterPath* m_pCrntPath;
	QList<QPainterPath*> m_lPreviousPath;

	QPen m_pen;
	QBrush m_brush;


	AbstractRecognizer* m_recognizer;

	static size_t m_writor_width_fixed;
	static size_t m_writor_height_fixed;
	static int m_writor_pen_w_fixed;

	//bool m_destroyed;

	bool m_isDrawing;
}; /* class handwritor2 */


#endif /* !defined(_handwritor2_H_) */


/*
 * end of file handwritor2.h
 */
