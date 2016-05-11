#include "zinnarecognizer.h"



#include <QDebug>

#pragma comment(lib, "libzinnia.lib")


ZinnaRecognizer::ZinnaRecognizer(QObject *parent)
	: AbstractRecognizer(parent)
{

}

ZinnaRecognizer::~ZinnaRecognizer()
{

}


bool ZinnaRecognizer::init(QHash<QString, QString> options)
{

	AbstractRecognizer::init(options);

	m_recognizer = zinnia::Recognizer::create();

	if (!m_recognizer->open(m_modelPath.toStdString().c_str())) {
		qDebug() << m_recognizer->what();
		return false;
	}

	return true;
}

QStringList ZinnaRecognizer::recognize(STROKES strokes)
{
	// Perform the recognition
	QStringList dstr;

	m_character = zinnia::Character::create();
	m_character->clear();

	m_character->set_height(m_canvasHeight);
	m_character->set_width(m_canvasWidth);

	int xOrigin = 0;
	int yOrigin = 0;

	int index = 0;
	foreach(STROKE stroke, strokes) {
		int size = stroke.at(0).size();

		int step = size / 10 + 1;
		for (int i = 0; i < size; i += step) {
			m_character->add(index, stroke.at(0).at(i), stroke.at(1).at(i));
		}
		index++;
	}

	zinnia::Result *result = m_recognizer->classify(*m_character, 100);

	if (!result) {
		qDebug() << QString::fromUtf8(m_recognizer->what());
	}
	else {
		for (size_t i = 0; i < result->size(); ++i) {

			dstr.append(QString::fromUtf8(result->value(i), 3));
			qDebug() << QString::fromUtf8(result->value(i), 3) << "\t" << result->score(i);
		}

		delete result;
	}


	delete m_character;

	emit recognizeResult(dstr);

	return dstr;
}
