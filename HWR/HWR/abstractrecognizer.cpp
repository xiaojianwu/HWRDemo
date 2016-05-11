#include "abstractrecognizer.h"

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

	return true;
}

QJsonArray AbstractRecognizer::pointsToJsonArray(XYPOINTS points)
{

	QVariant p;
	p.fromValue(points);


	QJsonArray x;
	QVariantList vx;
	vx.fromVector(points);
	x.fromVariantList(vx);

	return x;
}
