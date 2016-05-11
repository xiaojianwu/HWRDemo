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
