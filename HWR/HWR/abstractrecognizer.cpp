#include "abstractrecognizer.h"

#include <QDebug>

AbstractRecognizer::AbstractRecognizer(QObject *parent)
	: QObject(parent)
{

}

AbstractRecognizer::~AbstractRecognizer()
{

}


const QString AbstractRecognizer::OPTION_KEY_MODEL_PATH = "model.path";
const QString AbstractRecognizer::OPTION_KEY_CANVAS_WIDTH = "canvas.width";
const QString AbstractRecognizer::OPTION_KEY_CANVAS_HEIGHT = "canvas.height";

const QString AbstractRecognizer::OPTION_KEY_PROXY_TYPE = "proxy.type";
const QString AbstractRecognizer::OPTION_KEY_PROXY_IP = "proxy.ip";
const QString AbstractRecognizer::OPTION_KEY_PROXY_PORT = "proxy.port";

void AbstractRecognizer::recognize(STROKES strokes)
{
	emit sigRecognize(strokes);
}


bool AbstractRecognizer::init(QHash<QString, QString> options)
{
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


	emit sigInit(options);

	return true;
}

QJsonArray AbstractRecognizer::pointsToJsonArray(XYPOINTS points)
{
	QJsonArray x;

	for each (float point in points)
	{
		x.append(point);
	}

	return x;
}
