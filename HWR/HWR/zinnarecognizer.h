#ifndef ZINNARECOGNIZER_H
#define ZINNARECOGNIZER_H

#include "abstractrecognizer.h"

#include <zinnia.h>

class ZinnaRecognizer : public AbstractRecognizer
{
	Q_OBJECT

public:
	ZinnaRecognizer(QObject *parent);
	~ZinnaRecognizer();

	virtual bool init(QHash<QString, QString> options);

	virtual QStringList recognize(QList<QPainterPath*> strokes);


	static const QString OPTION_KEY_MODEL_PATH;
	static const QString OPTION_KEY_CANVAS_WIDTH;
	static const QString OPTION_KEY_CANVAS_HEIGHT;

private:
	QString	m_modelPath;
	float	m_canvasWidth;
	float	m_canvasHeight;

	zinnia::Recognizer *m_recognizer;
	zinnia::Character *m_character;
	
};

#endif // ZINNARECOGNIZER_H
