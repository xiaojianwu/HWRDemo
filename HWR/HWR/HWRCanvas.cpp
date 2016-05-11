#include <iostream> /* std .. */
#include "HWRCanvas.h" /* class handwritor2 */
#include <QtCore/QDebug>
#include <QtGui/QPainter> /* QPainter */

#include <QCoreApplication>


#include "recognizerfactory.h"


size_t HWRCanvas::m_writor_width_fixed = 400;
size_t HWRCanvas::m_writor_height_fixed = 400;
int HWRCanvas::m_writor_pen_w_fixed = 4;


#define __STLST_AL \
	"background-color: #87cefa; color: #ff0000; font-weight: bold; " \
	"border-radius: 8px"


HWRCanvas::HWRCanvas(int * ret, char * err, QWidget * focus,
	const char * model, QWidget * parent)
	: QWidget(parent), allpage(0), index(0) 
	, m_isDrawing(false)
{

	int i, open_result;
	char errstr[handwritor2_ERR_MIN];
	QSpacerItem * spacer;
	QPalette palette;
	QFont font;

	m_destroyed = false;

	writor_layout = NULL;
	option_layout = NULL;
	candidate_layout = NULL;

	m_recognizer = RecognizerFactory::Get()->getRecognizer(RecognizerFactory::HWR_TYPE_ZINNA);


	QHash<QString, QString> options;
	QString modelPath = QString("%1\\%2").arg(qApp->applicationDirPath()).arg(handwritor2_MODEL_DFT);
	options[AbstractRecognizer::OPTION_KEY_MODEL_PATH] = modelPath;
	options[AbstractRecognizer::OPTION_KEY_CANVAS_HEIGHT] = QString::number(m_writor_height_fixed);
	options[AbstractRecognizer::OPTION_KEY_CANVAS_WIDTH] = QString::number(m_writor_width_fixed); ;
	m_recognizer->init(options);

	/* ready to recognize */
	m_recogtimer.setInterval(2000);
	m_recogtimer.stop();
	QObject::connect(&m_recogtimer, SIGNAL(timeout()), this, SLOT(recognize()));

	/* writor */
	setContentsMargins(0, 0, 0, 0);
	setFixedSize(m_writor_width_fixed, m_writor_height_fixed);
	setAutoFillBackground(true);
	palette.setColor(QPalette::Background, QColor(0xff, 0xff, 0xff));
	setPalette(palette);

	master_layout = new QVBoxLayout(this);

	writor_layout = new QHBoxLayout();
	writor_layout->setContentsMargins(0, 0, 0, 0);

	spacer = new QSpacerItem(HWRCanvas::m_writor_width_fixed - 64 - 10,
		HWRCanvas::m_writor_height_fixed - 64 - 10);
	writor_layout->addItem(spacer);

	option_layout = new QVBoxLayout();
	option_layout->setContentsMargins(0, 0, 0, 0);

	spacer = new QSpacerItem(36, 24);
	option_layout->addItem(spacer);

	up = new QPushButton(tr("<<"), this);
	up->setFixedSize(36, 36);
	font.setPixelSize(14);
	up->setFont(font);
	up->setStyleSheet(__STLST_AL);
	option_layout->addWidget(up);

	spacer = new QSpacerItem(36, 24);
	option_layout->addItem(spacer);

	down = new QPushButton(tr(">>"), this);
	down->setFixedSize(36, 36);
	down->setFont(font);
	down->setStyleSheet(__STLST_AL);
	option_layout->addWidget(down);

	spacer = new QSpacerItem(36, 24);
	this->option_layout->addItem(spacer);

	this->option_layout->setAlignment(this->up,
		Qt::AlignRight | Qt::AlignVCenter);
	this->option_layout->setAlignment(this->down,
		Qt::AlignRight | Qt::AlignVCenter);
	this->option_layout->setStretch(0, 1);
	this->option_layout->setStretch(1, 1);
	this->option_layout->setStretch(2, 1);
	this->option_layout->setStretch(3, 1);
	this->option_layout->setStretch(4, 1);

	/*
	this->option_layout->setStretch(0, 7);
	this->option_layout->setStretch(1, 1);
	*/

	this->candidate_layout = new QHBoxLayout();
	this->candidate_layout->setContentsMargins(5, 0, 5, 0);
	/* ready candidate_btns */
	for (i = 0; i < 10; ++i) {
		candidate_btns[i] = new QPushButton(this);
		candidate_btns[i]->setFixedSize(36, 36);
		candidate_btns[i]->setFont(font);
		candidate_btns[i]->setStyleSheet(__STLST_AL);
		this->candidate_layout->addWidget(candidate_btns[i]);
	}

	this->writor_layout->addLayout(this->option_layout);
	this->writor_layout->setAlignment(this->option_layout, Qt::AlignCenter);

	master_layout->addLayout(this->writor_layout, 0);
	master_layout->addLayout(this->candidate_layout, 1);

	for (i = 0; i < 10; ++i) {
		this->candidate_layout->setAlignment(candidate_btns[i],
			Qt::AlignHCenter);
	}

	for (i = 0; i < 10; ++i) {
		QObject::connect(candidate_btns[i], SIGNAL(clicked()), this,
			SLOT(chooseQchar()));
	}
	QObject::connect(up, SIGNAL(clicked()), this, SLOT(turnpageup()));
	QObject::connect(down, SIGNAL(clicked()), this, SLOT(turnpagedown()));

	this->genbuttonstate();

	return;

} /* handwritor2::handwritor2 */


int HWRCanvas::destroy(void) {
	if (m_destroyed) {
		return -1;
	}

	if (this->candidate_layout) {
		QHBoxLayout * d = this->candidate_layout;
		this->candidate_layout = NULL;
		delete d;
	}

	if (this->option_layout) {
		QVBoxLayout * d = this->option_layout;
		this->option_layout = NULL;
		delete d;
	}

	if (this->writor_layout) {
		QHBoxLayout * d = this->writor_layout;
		this->writor_layout = NULL;
		delete d;
	}

	if (this->master_layout) {
		QVBoxLayout * d = this->master_layout;
		this->master_layout = NULL;
		delete d;
	}

	this->m_destroyed = true;
	return 0;
} /* handwritor2::destroy */


void HWRCanvas::mousePressEvent(QMouseEvent *event) {
	if (!m_isDrawing)
	{
		m_strokes.clear();
		update();
	}
	m_isDrawing = true;
	m_recogtimer.stop();

	handwritingX.clear();
	handwritingY.clear();

	w.clear();
	

	handwritingX << event->localPos().x();
	handwritingY << event->localPos().y();

	foreach(QPoint* point, m_currentstroke.points)
		delete point;
	m_currentstroke.points.clear();

	m_currentstroke.segments = 1;
	QPoint* point = new QPoint;
	point->setX(event->x());
	point->setY(event->y());
	m_currentstroke.points.append(point);
}
void HWRCanvas::mouseMoveEvent(QMouseEvent *event)
{
	static int count = 0;
	if (m_isDrawing) // 鼠标按下状态
	{
		QPoint* point = new QPoint;
		point->setX(event->x());
		point->setY(event->y());
		m_currentstroke.points.append(point);
		m_currentstroke.segments++;


		handwritingX << event->localPos().x();
		handwritingY << event->localPos().y();
	}
	count++;
	if (count % 10 == 0)
		update();
}

void HWRCanvas::mouseReleaseEvent(QMouseEvent *event)
{
	if (!m_isDrawing)
		return;
	append_stroke(m_currentstroke);
	update();

	w << handwritingX << handwritingY;

	trace << w;
	

	m_currentstroke.segments = 0;
	foreach(QPoint* point, m_currentstroke.points)
		delete point;
	m_currentstroke.points.clear();

	m_recogtimer.start();
}

void HWRCanvas::append_stroke(LineStroke& stroke)
{
	LineStroke s;
	s.segments = stroke.segments;
	if (stroke.segments)
	{
		for (int i = 0; i < stroke.points.count(); i++)
		{
			QPoint* point = new QPoint;
			point->setX(stroke.points[i]->x());
			point->setY(stroke.points[i]->y());
			s.points.append(point);
		}
		m_strokes.append(stroke);
	}
}

void HWRCanvas::genbuttonstate(void) {
	this->up->hide();
	this->down->hide();
	for (int i = 0; i < 10; i++) {
		candidate_btns[i]->hide();
		if (index < allpage) {
			if (index * 10 + i < dstr.count()) {
				candidate_btns[i]->setText(dstr[index * 10 + i]);
				candidate_btns[i]->show();
				this->up->show();
				this->down->show();
			}
		}
	}

}

void HWRCanvas::turnpageup()
{
	if (index > 0)
		index--;
	genbuttonstate();
}

void HWRCanvas::turnpagedown()
{
	if (index < allpage - 1)
		index++;
	genbuttonstate();
}

void HWRCanvas::chooseQchar(void) {
	static QString chose;

	/*
	QPushButton* push=static_cast<QPushButton *>(sender());
	emit routestring(push->text()[0]);
	*/

	QPushButton * b = static_cast<QPushButton *>(sender());
	chose.clear();
	chose = b->text();
	emit routestring(chose[0]);
	// std::cout << "SELECTED:" << chose.toLocal8Bit().data() << std::endl;
	fprintf(stdout, "SELECTED: %s\n", chose.toUtf8().data());
	fflush(stdout);

	this->up->hide();
	this->down->hide();
	for (int i = 0; i < 10; i++) {
		candidate_btns[i]->hide();
	}
} /* handwritor2::chooseQchar */

void HWRCanvas::recognize()
{
	m_recogtimer.stop();

	m_isDrawing = false;

	m_strokes.clear();
	index = 0;

	dstr = m_recognizer->recognize(trace);

	trace.clear();

	//向上取整,实现方法
	allpage = ((dstr.count() % 10) == 0) ? (dstr.count()) / 10 : (dstr.count()) / 10 + 1;
	genbuttonstate();
	update();
}

void HWRCanvas::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

	/*
	painter.drawLine(0, 125, 250, 125);
	painter.drawLine(125, 0, 125, 250);
	*/
	/* - */
	QPen pen_b;
	pen_b.setStyle(Qt::DotLine);
	painter.setPen(pen_b);
	painter.drawLine(0, HWRCanvas::m_writor_height_fixed / 2,
		HWRCanvas::m_writor_width_fixed, HWRCanvas::m_writor_height_fixed / 2);
	/* | */
	painter.drawLine(HWRCanvas::m_writor_width_fixed / 2, 0,
		HWRCanvas::m_writor_width_fixed / 2, HWRCanvas::m_writor_height_fixed);

	QPen pen;
	pen.setWidth(HWRCanvas::m_writor_pen_w_fixed);
	painter.setPen(pen);

	// painter.save();
	// painter.restore();

	LineStroke *cl;
	// 已经录入的笔画
	for (int i = 0; i < m_strokes.count(); i++) {
		cl = &m_strokes[i];

		for (int j = 0; j + 4 < cl->segments; j += 4) {
			painter.drawLine((float)cl->points[j]->x(),
				(float)cl->points[j]->y(),
				(float)cl->points[j + 4]->x(),
				(float)cl->points[j + 4]->y());
		}
	}

	// 当下笔画
	if (m_currentstroke.segments &&m_currentstroke.points[0]) {
		for (int j = 0; j + 4 < m_currentstroke.segments; j += 4) {
			painter.drawLine((float)m_currentstroke.points[j]->x(),
				(float)m_currentstroke.points[j]->y(),
				(float)m_currentstroke.points[j + 4]->x(),
				(float)m_currentstroke.points[j + 4]->y());
		}
	}
}


/*
 * end of file handwritor2.cpp
 */
