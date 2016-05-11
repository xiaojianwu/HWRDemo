#include "HWRCanvas.h"
#include <QDebug>
#include <QPainter> /* QPainter */

size_t HWRCanvas::CANVAS_WIDTH = 400;
size_t HWRCanvas::CANVAS_HEIGHT = 400;
int HWRCanvas::PEN_SIZE = 4;

HWRCanvas::HWRCanvas(QWidget * parent)
	: QWidget(parent)
	, m_isDrawing(false)
	, m_pCrntPath(NULL)
	, m_recognizer(nullptr)
{
	setContentsMargins(0, 0, 0, 0);
	setFixedSize(CANVAS_WIDTH, CANVAS_HEIGHT);
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

		m_handwritingX.clear();
		m_handwritingY.clear();

		m_currentStroke.clear();

		m_handwritingX << event->localPos().x();
		m_handwritingY << event->localPos().y();

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

		float prevX = m_handwritingX.at(m_handwritingX.size() - 1);
		float prevY = m_handwritingY.at(m_handwritingY.size() - 1);

		m_pCrntPath->cubicTo(prevX, prevY, x, y, x, y);

		m_handwritingX << x;
		m_handwritingY << y;

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

	float prevX = m_handwritingX.at(m_handwritingX.size() - 1);
	float prevY = m_handwritingY.at(m_handwritingY.size() - 1);

	m_pCrntPath->cubicTo(prevX, prevY, x, y, x, y);

	m_lPreviousPath << m_pCrntPath;

	update();

	m_currentStroke << m_handwritingX << m_handwritingY;
	m_strokes << m_currentStroke;

	recognize();

	m_handwritingX.clear();
	m_handwritingY.clear();
}

void HWRCanvas::recognize()
{
	m_isDrawing = false;

	m_recognizer->recognize(m_strokes);
}

void HWRCanvas::paintEvent(QPaintEvent *event)
{
	QPainter* p = new QPainter();

	p->begin(this);

	QPen pen_b;
	pen_b.setStyle(Qt::DotLine);
	p->setPen(pen_b);
	p->drawLine(0, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT / 2);
	p->drawLine(CANVAS_WIDTH / 2, 0, CANVAS_WIDTH / 2, CANVAS_HEIGHT);

	QPen pen;
	pen.setWidth(PEN_SIZE);
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
	m_handwritingX.clear();
	m_handwritingY.clear();

	m_strokes.clear();

	for each (QPainterPath* path in m_lPreviousPath)
	{
		delete path;
	}
	m_lPreviousPath.clear();

	// m_pCrntPath已经被包含在m_lPreviousPath中
	m_pCrntPath = NULL;

	update();
}