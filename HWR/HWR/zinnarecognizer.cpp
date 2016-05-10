#include "zinnarecognizer.h"



#include <QDebug>

const QString ZinnaRecognizer::OPTION_KEY_MODEL_PATH = "model.path";
const QString ZinnaRecognizer::OPTION_KEY_CANVAS_WIDTH = "canvas.width";
const QString ZinnaRecognizer::OPTION_KEY_CANVAS_HEIGHT = "canvas.height";


ZinnaRecognizer::ZinnaRecognizer(QObject *parent)
	: AbstractRecognizer(parent)
{

}

ZinnaRecognizer::~ZinnaRecognizer()
{

}


bool ZinnaRecognizer::init(QHash<QString, QString> options)
{
	m_recognizer = zinnia::Recognizer::create();

	if (!options.contains(OPTION_KEY_MODEL_PATH))
	{
		return false;
	}
	m_modelPath = options[OPTION_KEY_MODEL_PATH];

	if (!options.contains(OPTION_KEY_CANVAS_WIDTH))
	{
		return false;
	}
	m_canvasWidth = options[OPTION_KEY_CANVAS_WIDTH].toFloat();

	if (!options.contains(OPTION_KEY_CANVAS_HEIGHT))
	{
		return false;
	}
	m_canvasHeight = options[OPTION_KEY_CANVAS_HEIGHT].toFloat();


	if (!m_recognizer->open(m_modelPath.toStdString().c_str())) {
		qDebug() << m_recognizer->what();
		return false;
	}

	


	return true;
}

QStringList ZinnaRecognizer::recognize(QList<QPainterPath*> strokes)
{
	// Perform the recognition
	QStringList dstr;

	m_character = zinnia::Character::create();
	m_character->clear();

	m_character->set_height(m_canvasHeight);
	m_character->set_width(m_canvasWidth);

	int xOrigin = 0;
	int yOrigin = 0;

	for (int i = 0; i < strokes.size(); i++) {
		for (int j = 0; j <= 1; j += 1) {
			QPointF cursor = strokes.at(i)->pointAtPercent(j);
			m_character->add(i, cursor.x() - xOrigin, cursor.y() - yOrigin);
		}
	}

	zinnia::Result *result = m_recognizer->classify(*m_character, 100);
	if (!result) {
		qDebug() << m_recognizer->what();
	}
	else {
		for (size_t i = 0; i < result->size(); ++i) {

			dstr.append(QString::fromUtf8(result->value(i), 3));
			qDebug() << QString::fromUtf8(result->value(i), 3) << "\t" << result->score(i);
		}

		delete result;
	}

	
	delete m_character;

	return dstr;
}
