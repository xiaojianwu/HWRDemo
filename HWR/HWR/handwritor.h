/* ===========================================================================
 *
 *       Filename:  handwritor.h
 *
 *    Description:  handwritor (qt)
 *
 * - Mode: 644
 * - NOTE. NOT use "dos"
 * - Updated: TODO
 *
 *        Version:  1.0.0
 *        Created:  2015-04-16 10:13:08
 *       Revision:  1.0.0
 *       Compiler:  moc/g++
 *
 *         Author:  Yui Wong, email: yuiwong@126.com
 *   Organization:  ""
 *        License:  LGPLv3
 *
 *  This file is part of zinnia-handwriting.
 *
 *  zinnia-handwriting
 *  is free software: you can redistribute it
 *  and/or modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  zinnia-handwriting
 *  is distributed in the hope that it will be
 *  useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with zinnia-handwriting.
 *  If not, see <http://www.gnu.org/licenses/>.
 *
 * ===========================================================================
 */


#if !defined(_HANDWRITOR_H_)
#define _HANDWRITOR_H_ (1)


#include <QtCore/QTimer>
#include <QtGui/QMouseEvent>
#include <QtGui/QPaintEvent>


#	include <QtWidgets/QWidget>
#	include <QtWidgets/QPushButton>
#	include <QtWidgets/QHBoxLayout>
#	include <QtWidgets/QVBoxLayout>


#include <zinnia.h> /* zinnia_character_t .. */
#include "LineStroke.h" /* class LineStroke */


typedef enum _hw_state_t {
	Press,
	Move,
	Release,
	Choose,/* 选择阶段 */
} hw_state_t;


#define HANDWRITOR_MODEL_DFT "handwriting-zh_CN.model"
#define HANDWRITOR_ERR_MIN (64)


class handwritor : public QWidget {
	Q_OBJECT

		/* err min HANDWRITOR_ERR_MIN */
public:
	handwritor(int * ret, char * err, QWidget * focus,
		const char * model = 0, QWidget * parent = 0);
	~handwritor(void) { destroy(); }
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
	QTimer recogtimer;
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

	hw_state_t m_mouse_state;
}; /* class handwritor */


#endif /* !defined(_HANDWRITOR_H_) */


/*
 * end of file handwritor.h
 */
