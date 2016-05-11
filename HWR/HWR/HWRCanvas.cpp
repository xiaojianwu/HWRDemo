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

		generateCurrentPath();
		update();
	}
}
void HWRCanvas::mouseMoveEvent(QMouseEvent *event)
{
	if (m_isDrawing) // 鼠标按下状态
	{
		handwritingX << event->localPos().x();
		handwritingY << event->localPos().y();

		generateCurrentPath();
		update();
	}

}

void HWRCanvas::mouseReleaseEvent(QMouseEvent *event)
{
	if (!m_isDrawing)
		return;

	m_isDrawing = false;

	generateCurrentPath();
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



void HWRCanvas::generateCurrentPath() 
{
	// TODO 内存泄漏
	//if (m_pCrntPath && m_isDrawing)
	//{
	//	delete m_pCrntPath;
	//	m_pCrntPath = NULL;
	//}
	m_pCrntPath = new QPainterPath();
	if (!handwritingX.empty()) {
		if (handwritingX.size() > 1) {
			for (int i = handwritingX.size() - 2; i < handwritingX.size(); i++) {
				int dx, dy;
				if (i >= 0) {
					if (i == 0) {
						dx = ((handwritingX.at(i + 1) - handwritingX.at(i)) / SMOOTHING);
						dy = ((handwritingY.at(i + 1) - handwritingY.at(i)) / SMOOTHING);
					}
					else if (i == handwritingX.size() - 1) {
						dx = ((handwritingX.at(i) - handwritingX.at(i - 1)) / SMOOTHING);
						dy = ((handwritingY.at(i) - handwritingY.at(i - 1)) / SMOOTHING);
					}
					else {
						dx = ((handwritingX.at(i + 1) - handwritingX.at(i - 1)) / SMOOTHING);
						dy = ((handwritingY.at(i + 1) - handwritingY.at(i - 1)) / SMOOTHING);
					}
				}
			}
		}
	}

	bool first = true;
	for (int i = 0; i < handwritingX.size(); i++) {
		float x = handwritingX.at(i);
		float y = handwritingY.at(i);
		if (first) {
			first = false;
			m_pCrntPath->moveTo(x, y);
		}
		else {
			float prevX = handwritingX.at(i - 1);
			float prevY = handwritingY.at(i - 1);

			m_pCrntPath->cubicTo(prevX, prevY, x, y, x, y);
		}
	}
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