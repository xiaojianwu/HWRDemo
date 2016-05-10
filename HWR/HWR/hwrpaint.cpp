#include "hwrpaint.h"


#include <QPainter>

HWRPaint::HWRPaint(QWidget *parent)
	: QWidget(parent)
	, m_pCache(nullptr)
{
	ui.setupUi(this);
}

HWRPaint::~HWRPaint()
{

}


void HWRPaint::mousePressEvent(QMouseEvent *event) {


	if (!m_bIsDrawing) {
		m_bIsDrawing = true;
		m_strokes.clear();
		update();

		m_currentstroke.moveTo(event->localPos());
	}
}


void HWRPaint::mouseMoveEvent(QMouseEvent *event)
{
	static int count = 0;
	if (m_bIsDrawing) // Êó±ê°´ÏÂ×´Ì¬
	{
		m_currentstroke.moveTo(event->localPos());


		updateCache();
		update();
	}
	//count++;
	//if (count % 10 == 0)
	//	update();
}

void HWRPaint::mouseReleaseEvent(QMouseEvent *event)
{
	if (m_bIsDrawing)
	{
		
		m_strokes.append(m_currentstroke);
		m_bIsDrawing = false;
		updateCache();
		update();
	}
}



void HWRPaint::paintEvent(QPaintEvent* ev) {
	Q_UNUSED(ev);

	QPainter* p = new QPainter();
	p->begin(this);

	setupPainter(p);

	// Draw the cache
	if (NULL != m_pCache) {
		p->drawPixmap(QPoint(), *m_pCache);
	}
	else {
		p->setPen(Qt::NoPen);
		//p->setBrush(QBrush(Qt::black));
		p->drawRect(rect());
		setupPainter(p);
	}

	// Draw the current path
	p->drawPath(m_currentstroke);

	p->end();
}


void HWRPaint::setupPainter(QPainter* p) {
	p->setPen(m_pen);
	p->setBrush(Qt::NoBrush);
	p->setRenderHints(QPainter::Antialiasing, true);
}


void HWRPaint::updateCache() {
	if (NULL != m_pCache) {
		delete m_pCache;
	}
	m_pCache = new QPixmap(rect().width(), rect().height());
	QPainter* p = new QPainter();
	p->begin(m_pCache);
	//p->setBrush(QBrush(Qt::black));
	p->setPen(Qt::black);
	p->drawRect(m_pCache->rect());
	setupPainter(p);

	foreach(QPainterPath path, m_strokes) {
		p->drawPath(path);
	}
	p->end();
}


void HWRPaint::clear() 
{
	//m_currentstroke.
	m_strokes.clear();
	updateCache();
	update();
}