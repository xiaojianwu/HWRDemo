#ifndef ABSTRACTRECOGNIZER_H
#define ABSTRACTRECOGNIZER_H

#include <QObject>

#include <QPainterPath>

#include <QHash>

#include <QVector>
#include <QVariant>
#include <QJsonArray>


typedef QList<float> XYPOINTS;
typedef QList<XYPOINTS> STROKE;
typedef QList<STROKE> STROKES;

class AbstractRecognizer : public QObject
{
	Q_OBJECT

public:
	AbstractRecognizer(QObject *parent);
	~AbstractRecognizer();

	virtual bool init(QHash<QString, QString> options);

	virtual void recognize(STROKES strokes);


	static const QString OPTION_KEY_MODEL_PATH;
	static const QString OPTION_KEY_CANVAS_WIDTH;
	static const QString OPTION_KEY_CANVAS_HEIGHT;
	static const QString OPTION_KEY_PROXY_TYPE;
	static const QString OPTION_KEY_PROXY_IP;
	static const QString OPTION_KEY_PROXY_PORT;

	static QJsonArray pointsToJsonArray(XYPOINTS points);

signals:
	void sigInit(QHash<QString, QString> options);
	void sigRecognize(STROKES strokes);

signals:
	void recognizeResult(QStringList result);

	void error(QString errorStr);

protected:

	QString	m_modelPath;
	float	m_canvasWidth;
	float	m_canvasHeight;

	int		m_proxyType;
	QString m_proxyIp;
	int		m_proxyPort;
	
};

#endif // ABSTRACTRECOGNIZER_H
