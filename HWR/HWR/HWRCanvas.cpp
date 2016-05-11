#include "HWRCanvas.h"
#include <QDebug>
#include <QPainter> /* QPainter */

size_t HWRCanvas::m_writor_width_fixed = 400;
size_t HWRCanvas::m_writor_height_fixed = 400;
int HWRCanvas::m_writor_pen_w_fixed = 4;

HWRCanvas::HWRCanvas(QWidget * parent)
	: QWidget(parent)
	, m_isDrawing(false)
	, m_pCrntPath(NULL)
	, m_recognizer(nullptr)
{
	setContentsMargins(0, 0, 0, 0);
	setFixedSize(m_writor_width_fixed, m_writor_height_fixed);
	setAutoFillBackground(true);

	QPalette palette;
	palette.setColor(QPalette::Background, QColor(0xff, 0xff, 0xff));
	setPalette(palette);
}
void HWRCanvas::setRecognizer(AbstractRecognizer* recognizer)
{
	m_recognizer = recognizer;
}


void HWRCanvas::mousePressEvent(QMouseEvent *event) 
{
	if (!m_isDrawing)
	{
		m_isDrawing = true;

		handwritingX.clear();
		handwritingY.clear();

		w.clear();

		handwritingX << event->localPos().x();
		handwritingY << event->localPos().y();

		m_pCrntPath = new QPainterPath();
		m_pCrntPath->moveTo(event->localPos().x(), event->localPos().y());
		update();
	}
}
void HWRCanvas::mouseMoveEvent(QMouseEvent *event)
{
	if (m_isDrawing) // 鼠标按下状态
	{
		float x = event->localPos().x();
		float y = event->localPos().y();

		float prevX = handwritingX.at(handwritingX.size() - 1);
		float prevY = handwritingY.at(handwritingY.size() - 1);

		m_pCrntPath->cubicTo(prevX, prevY, x, y, x, y);

		handwritingX << x;
		handwritingY << y;

		update();
	}

}

void HWRCanvas::mouseReleaseEvent(QMouseEvent *event)
{
	if (!m_isDrawing)
		return;

	m_isDrawing = false;

	float x = event->localPos().x();
	float y = event->localPos().y();

	float prevX = handwritingX.at(handwritingX.size() - 1);
	float prevY = handwritingY.at(handwritingY.size() - 1);

	m_pCrntPath->cubicTo(prevX, prevY, x, y, x, y);

	m_lPreviousPath << m_pCrntPath;

	update();

	w << handwritingX << handwritingY;
	trace << w;

	recognize();

	handwritingX.clear();
	handwritingY.clear();
}

void HWRCanvas::recognize()
{
	m_isDrawing = false;

	m_recognizer->recognize(trace);

	update();
}

void HWRCanvas::paintEvent(QPaintEvent *event)
{
	QPainter* p = new QPainter();

	p->begin(this);

	QPen pen_b;
	pen_b.setStyle(Qt::DotLine);
	p->setPen(pen_b);
	p->drawLine(0, m_writor_height_fixed / 2, m_writor_width_fixed, m_writor_height_fixed / 2);
	/* | */
	p->drawLine(m_writor_width_fixed / 2, 0, m_writor_width_fixed / 2, m_writor_height_fixed);

	QPen pen;
	pen.setWidth(m_writor_pen_w_fixed);
	p->setPen(pen);

	foreach(QPainterPath* path, m_lPreviousPath) {
		if (NULL != path) {
			p->drawPath(*path);
		}
	}

	// Draw the current path
	if (NULL != m_pCrntPath) {
		p->drawPath(*m_pCrntPath);
	}
	p->end();
}

void HWRCanvas::clear() {
	handwritingX.clear();
	handwritingY.clear();

	trace.clear();

	for each (QPainterPath* path in m_lPreviousPath)
	{
		delete path;
	}
	m_lPreviousPath.clear();


	m_pCrntPath = NULL;

	update();
}