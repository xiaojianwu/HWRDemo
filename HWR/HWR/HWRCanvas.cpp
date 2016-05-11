#include "HWRCanvas.h"
#include <QDebug>
#include <QPainter> /* QPainter */

#include <QCoreApplication>


#include "recognizerfactory.h"


size_t HWRCanvas::m_writor_width_fixed = 400;
size_t HWRCanvas::m_writor_height_fixed = 400;
int HWRCanvas::m_writor_pen_w_fixed = 4;

#define CANDIDATE_COUNT 5


#define __STLST_AL \
	"background-color: #87cefa; color: #ff0000; font-weight: bold; " \
	"border-radius: 8px"


HWRCanvas::HWRCanvas(int hwrType, QWidget * parent)
	: QWidget(parent), allpage(0), index(0)
	, m_isDrawing(false)
	, m_pCrntPath(NULL)
{

	int i, open_result;
	char errstr[handwritor2_ERR_MIN];
	QSpacerItem * spacer;
	QPalette palette;
	QFont font;

	writor_layout = NULL;
	option_layout = NULL;
	candidate_layout = NULL;

	m_recognizer = RecognizerFactory::Get()->getRecognizer((RecognizerFactory::HWR_TYPE)hwrType);

	connect(m_recognizer, SIGNAL(recognizeResult(QStringList)), this, SLOT(onRecognizeResult(QStringList)));


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
	option_layout->addItem(spacer);


	m_btnClear = new QPushButton(tr("CLR"), this);
	m_btnClear->setFixedSize(36, 36);
	m_btnClear->setFont(font);
	m_btnClear->setStyleSheet(__STLST_AL);
	option_layout->addWidget(m_btnClear);

	spacer = new QSpacerItem(36, 24);
	option_layout->addItem(spacer);

	option_layout->setAlignment(up,
		Qt::AlignRight | Qt::AlignVCenter);
	option_layout->setAlignment(down,
		Qt::AlignRight | Qt::AlignVCenter);

	option_layout->setAlignment(m_btnClear,
		Qt::AlignRight | Qt::AlignVCenter);
	option_layout->setStretch(0, 1);
	option_layout->setStretch(1, 1);
	option_layout->setStretch(2, 1);
	option_layout->setStretch(3, 1);
	option_layout->setStretch(4, 1);

	/*
	option_layout->setStretch(0, 7);
	option_layout->setStretch(1, 1);
	*/

	candidate_layout = new QHBoxLayout();
	candidate_layout->setContentsMargins(5, 0, 5, 0);
	/* ready candidate_btns */
	for (i = 0; i < CANDIDATE_COUNT; ++i) {
		candidate_btns[i] = new QPushButton(this);
		candidate_btns[i]->setFixedSize(36, 36);
		candidate_btns[i]->setFont(font);
		candidate_btns[i]->setStyleSheet(__STLST_AL);
		candidate_layout->addWidget(candidate_btns[i]);
	}

	writor_layout->addLayout(option_layout);
	writor_layout->setAlignment(option_layout, Qt::AlignCenter);

	master_layout->addLayout(writor_layout, 0);
	master_layout->addLayout(candidate_layout, 1);

	for (i = 0; i < CANDIDATE_COUNT; ++i) {
		candidate_layout->setAlignment(candidate_btns[i],
			Qt::AlignHCenter);
	}

	QObject::connect(up, SIGNAL(clicked()), this, SLOT(turnpageup()));
	QObject::connect(down, SIGNAL(clicked()), this, SLOT(turnpagedown()));
	QObject::connect(m_btnClear, SIGNAL(clicked()), this, SLOT(clear()));

	

	genbuttonstate();

	return;

}


int HWRCanvas::destroy(void) 
{

	if (candidate_layout) {
		QHBoxLayout * d = candidate_layout;
		candidate_layout = NULL;
		delete d;
	}

	if (option_layout) {
		QVBoxLayout * d = option_layout;
		option_layout = NULL;
		delete d;
	}

	if (writor_layout) {
		QHBoxLayout * d = writor_layout;
		writor_layout = NULL;
		delete d;
	}

	if (master_layout) {
		QVBoxLayout * d = master_layout;
		master_layout = NULL;
		delete d;
	}
	return 0;
}


void HWRCanvas::mousePressEvent(QMouseEvent *event) 
{
	if (!m_isDrawing)
	{
		m_isDrawing = true;
		m_recogtimer.stop();

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

	handwritingX.clear();
	handwritingY.clear();

	m_recogtimer.start();
}


void HWRCanvas::onRecognizeResult(QStringList list)
{
	m_resultList = list;
	allpage = ((m_resultList.count() % CANDIDATE_COUNT) == 0) ? (m_resultList.count()) / CANDIDATE_COUNT : (m_resultList.count()) / CANDIDATE_COUNT + 1;
	genbuttonstate();
}

void HWRCanvas::genbuttonstate(void) {
	up->hide();
	down->hide();
	for (int i = 0; i < CANDIDATE_COUNT; i++) {
		candidate_btns[i]->hide();
		if (index < allpage) {
			if (index * CANDIDATE_COUNT + i < m_resultList.count()) {
				candidate_btns[i]->setText(m_resultList[index * CANDIDATE_COUNT + i]);
				candidate_btns[i]->show();
				up->show();
				down->show();
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

void HWRCanvas::recognize()
{
	m_recogtimer.stop();

	m_isDrawing = false;

	index = 0;

	m_resultList = m_recognizer->recognize(trace);

	trace.clear();

	//向上取整,实现方法
	allpage = ((m_resultList.count() % CANDIDATE_COUNT) == 0) ? (m_resultList.count()) / CANDIDATE_COUNT : (m_resultList.count()) / CANDIDATE_COUNT + 1;
	genbuttonstate();
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

	for each (QPainterPath* path in m_lPreviousPath)
	{
		delete path;
	}
	m_lPreviousPath.clear();


	m_pCrntPath = NULL;

	update();
}