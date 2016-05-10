#include <iostream> /* std .. */
#include "handwritor.h" /* class handwritor */
#include <QtCore/QDebug>
#include <QtGui/QPainter> /* QPainter */

#include <QCoreApplication>


size_t handwritor::m_writor_width_fixed = 400;
size_t handwritor::m_writor_height_fixed = 400;
int handwritor::m_writor_pen_w_fixed = 4;


#define __STLST_AL \
	"background-color: #87cefa; color: #ff0000; font-weight: bold; " \
	"border-radius: 8px"


handwritor::handwritor(int * ret, char * err, QWidget * focus,
	const char * model, QWidget * parent)
	: QWidget(parent), allpage(0), index(0) {
	int i, open_result;
	char errstr[HANDWRITOR_ERR_MIN];
	QSpacerItem * spacer;
	QPalette palette;
	QFont font;

	m_destroyed = false;

	writor_layout = NULL;
	option_layout = NULL;
	candidate_layout = NULL;

	m_recognizer = NULL;
	m_recognizer = zinnia_recognizer_new();/* create recognizer engine */
	/* open model */
	if (model) {
		open_result = zinnia_recognizer_open(m_recognizer, model);
	}
	else {
		QString modelPath = QString("%1\\%2").arg(qApp->applicationDirPath()).arg(HANDWRITOR_MODEL_DFT);
		open_result = zinnia_recognizer_open(m_recognizer, modelPath.toStdString().c_str());
	}
	if (!open_result) {
		memset(errstr, 0, HANDWRITOR_ERR_MIN);
		strncpy(errstr, zinnia_recognizer_strerror(m_recognizer),
			HANDWRITOR_ERR_MIN - 1);
		if (err) {
			strncpy(err, errstr, HANDWRITOR_ERR_MIN - 1);
		}
		if (ret) {
			*ret = -1;/* zinnia_recognizer_open fail */
		}
		std::cerr << errstr << std::endl;
		goto fail_return;
	} /* ! open_result */

	/* ready to recognize */
	recogtimer.setInterval(1000);
	recogtimer.stop();
	QObject::connect(&recogtimer, SIGNAL(timeout()), this, SLOT(recognize()));

	/* writor */
	setContentsMargins(0, 0, 0, 0);
	setFixedSize(m_writor_width_fixed, m_writor_height_fixed);
	setAutoFillBackground(true);
	palette.setColor(QPalette::Background, QColor(0xff, 0xff, 0xff));
	setPalette(palette);

	master_layout = new QVBoxLayout(this);

	writor_layout = new QHBoxLayout();
	writor_layout->setContentsMargins(0, 0, 0, 0);

	spacer = new QSpacerItem(handwritor::m_writor_width_fixed - 64 - 10,
		handwritor::m_writor_height_fixed - 64 - 10);
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

fail_return:
	this->destroy();

} /* handwritor::handwritor */


int handwritor::destroy(void) {
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

	if (this->m_recognizer) {
		zinnia_recognizer_t * d = this->m_recognizer;
		this->m_recognizer = NULL;
		(void)zinnia_recognizer_close(d);
		(void)zinnia_recognizer_destroy(d);
	}

	this->m_destroyed = true;
	return 0;
} /* handwritor::destroy */


void handwritor::mousePressEvent(QMouseEvent *event) {
	if (m_mouse_state == Choose)
	{
		m_strokes.clear();
		update();
	}
	m_mouse_state = Press;
	recogtimer.stop();

	foreach(QPoint* point, m_currentstroke.points)
		delete point;
	m_currentstroke.points.clear();

	m_currentstroke.segments = 1;
	QPoint* point = new QPoint;
	point->setX(event->x());
	point->setY(event->y());
	m_currentstroke.points.append(point);
}
void handwritor::mouseMoveEvent(QMouseEvent *event)
{
	static int count = 0;
	if (m_mouse_state == (Press | Move)) // 鼠标按下状态
	{
		QPoint* point = new QPoint;
		point->setX(event->x());
		point->setY(event->y());
		m_currentstroke.points.append(point);
		m_currentstroke.segments++;
	}
	m_mouse_state = Move;
	count++;
	if (count % 10 == 0)
		update();
}

void handwritor::mouseReleaseEvent(QMouseEvent *event)
{
	if (m_mouse_state != Move)
		return;
	append_stroke(m_currentstroke);
	update();
	m_mouse_state = Release;

	m_currentstroke.segments = 0;
	foreach(QPoint* point, m_currentstroke.points)
		delete point;
	m_currentstroke.points.clear();

	recogtimer.start();
}

void handwritor::append_stroke(LineStroke& stroke)
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

void handwritor::genbuttonstate(void) {
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

void handwritor::turnpageup()
{
	if (index > 0)
		index--;
	genbuttonstate();
}

void handwritor::turnpagedown()
{
	if (index < allpage - 1)
		index++;
	genbuttonstate();
}

void handwritor::chooseQchar(void) {
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
} /* handwritor::chooseQchar */

void handwritor::stok2qchar()
{
	zinnia_result_t *result;
	LineStroke * l;
	m_character = zinnia_character_new();

	dstr.clear();
	zinnia_character_clear(m_character);
	zinnia_character_set_width(m_character, m_writor_width_fixed);
	zinnia_character_set_height(m_character, m_writor_height_fixed);

	for (int i = 0; i < m_strokes.size(); i++)
	{
		l = &m_strokes[i];

		for (int j = 0; j < (l->segments); j += (l->segments / 10 + 1))
		{
			l->points[j]->x(), l->points[j]->y();
			//   if(j>2)
			//       j=l->segments-1;
			zinnia_character_add(m_character, i, l->points[j]->x(), l->points[j]->y());
		}
	}

	result = zinnia_recognizer_classify(m_recognizer, m_character, 100);
	if (result == NULL)
	{
		fprintf(stderr, "%s\n", zinnia_recognizer_strerror(m_recognizer));
		zinnia_character_destroy(m_character);
		//zinnia_recognizer_destroy(recognizer);
		return;
	}

	for (int i = 0; i < zinnia_result_size(result); ++i)
	{

		dstr.append(QString::fromUtf8(zinnia_result_value(result, i), 3));

		// strcpy(c->charactor,zinnia_result_value(result, i));

		fprintf(stdout, "%s\t%f\n", zinnia_result_value(result, i),
			zinnia_result_score(result, i));
	}

	zinnia_result_destroy(result);
	zinnia_character_destroy(m_character);
	return;
}

void handwritor::recognize()
{
	recogtimer.stop();
	m_mouse_state = Choose;
	stok2qchar();
	m_strokes.clear();
	index = 0;

	//向上取整,实现方法
	allpage = ((dstr.count() % 10) == 0) ? (dstr.count()) / 10 : (dstr.count()) / 10 + 1;
	genbuttonstate();
	update();
}

void handwritor::paintEvent(QPaintEvent *event)
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
	painter.drawLine(0, handwritor::m_writor_height_fixed / 2,
		handwritor::m_writor_width_fixed, handwritor::m_writor_height_fixed / 2);
	/* | */
	painter.drawLine(handwritor::m_writor_width_fixed / 2, 0,
		handwritor::m_writor_width_fixed / 2, handwritor::m_writor_height_fixed);

	QPen pen;
	pen.setWidth(handwritor::m_writor_pen_w_fixed);
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
 * end of file handwritor.cpp
 */
