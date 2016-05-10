#if !defined(_handwritor2_H_)
#define _handwritor2_H_ (1)


#include <QtCore/QTimer>
#include <QtGui/QMouseEvent>
#include <QtGui/QPaintEvent>


#	include <QtWidgets/QWidget>
#	include <QtWidgets/QPushButton>
#	include <QtWidgets/QHBoxLayout>
#	include <QtWidgets/QVBoxLayout>


#include <zinnia.h> /* zinnia_character_t .. */
#include "LineStroke.h" /* class LineStroke */


#define handwritor2_MODEL_DFT "handwriting-zh_CN.model"
#define handwritor2_ERR_MIN (64)


class handwritor2 : public QWidget {
	Q_OBJECT

		/* err min handwritor2_ERR_MIN */
public:
	handwritor2(int * ret, char * err, QWidget * focus,
		const char * model = 0, QWidget * parent = 0);
	~handwritor2(void) { destroy(); }
	int destroy(void);
	bool is_destroyed(void) { return m_destroyed; }
	


signals:
	void routestring(QChar c);/* 向其他窗口传递QChar */

	/* evt */
	protected:
	void mouseMoveEvent(QMouseEvent * event);
	void mousePressEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);
	void paintEvent(QPaintEvent * event);

	void genbuttonstate(void);
	void stok2qchar(void);
	void append_stroke(LineStroke& stroke);
	



	protected slots:
	void recognize(void);
	void turnpageup(void);
	void turnpagedown(void);
	void chooseQchar(void);

private:
	QVBoxLayout * master_layout;
	QHBoxLayout * writor_layout;
	QVBoxLayout * option_layout;
	QHBoxLayout * candidate_layout;
	QPushButton* candidate_btns[10];
	QPushButton *up;
	QPushButton *down;
	QTimer m_recogtimer;
	QStringList dstr;
	int index;
	int allpage;

	LineStroke m_currentstroke;
	QVector<LineStroke>  m_strokes;
	zinnia_character_t * m_character;
	zinnia_recognizer_t * m_recognizer;

	static size_t m_writor_width_fixed;
	static size_t m_writor_height_fixed;
	static int m_writor_pen_w_fixed;

	bool m_destroyed;

	bool m_isDrawing;
}; /* class handwritor2 */


#endif /* !defined(_handwritor2_H_) */


/*
 * end of file handwritor2.h
 */
