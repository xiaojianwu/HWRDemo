#ifndef ABSTRACTRECOGNIZER_H
#define ABSTRACTRECOGNIZER_H

#include <QObject>

#include <QPainterPath>

#include <QHash>

#include <QVector>
#include <QVariant>
#include <QJsonArray>


typedef QVector<QVariant> XYPOINTS;
typedef QVector<XYPOINTS> STROKE;
typedef QVector<STROKE> STROKES;

class AbstractRecognizer : public QObject
{
	Q_OBJECT

public:
	AbstractRecognizer(QObject *parent);
	~AbstractRecognizer();

	virtual bool init(QHash<QString, QString> options) = 0;

	virtual QStringList recognize(STROKES strokes) = 0;


	static const QString OPTION_KEY_MODEL_PATH;
	static const QString OPTION_KEY_CANVAS_WIDTH;
	static const QString OPTION_KEY_CANVAS_HEIGHT;

	static QJsonArray pointsToJsonArray(XYPOINTS points);

protected:

	QString	m_modelPath;
	float	m_canvasWidth;
	float	m_canvasHeight;
	
};

#endif // ABSTRACTRECOGNIZER_H
