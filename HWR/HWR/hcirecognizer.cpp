#include "hcirecognizer.h"

#include <QDebug>
#include <QTime>
#include <string>

#include <hci_sys.h>

#include <hci_hwr.h>

#include <time.h>



#pragma comment(lib, "hci_sys.lib")
#pragma comment(lib, "hci_hwr.lib")

HCIRecognizer::HCIRecognizer(QObject *parent)
	: AbstractRecognizer(parent)
{
	qRegisterMetaType<QHash<QString, QString>>("QHash<QString, QString>");
	qRegisterMetaType<STROKES>("STROKES");
	connect(this, SIGNAL(sigInit(QHash<QString, QString>)), this, SLOT(onInit(QHash<QString, QString>)));
	connect(this, SIGNAL(sigRecognize(STROKES)), this, SLOT(onRecognize(STROKES)));
}

HCIRecognizer::~HCIRecognizer()
{
	HCI_ERR_CODE err_code = HCI_ERR_NONE;
	
	hci_hwr_session_stop(m_nSessionId);
	qDebug() << "hci_hwr_session_stop success\n";

	//HWR反初始化
	hci_hwr_release();


	//SYS反初始化
	err_code = hci_release();
	qDebug() << "hci_release\n";
}

bool HCIRecognizer::CheckAndUpdateAuth()
{
	//获取过期时间
	int64 expire_time;
	int64 current_time = (int64)time(NULL);
	HCI_ERR_CODE err_code = hci_get_auth_expire_time(&expire_time);
	if (err_code == HCI_ERR_NONE)
	{
		//获取成功则判断是否过期
		if (expire_time > current_time)
		{
			//没有过期
			qDebug() << "auth can use continue\n";
			return true;
		}
	}

	//获取过期时间失败或已经过期
	//手动调用更新授权
	//HCI_ERR_CODE err_code;
	err_code = hci_check_auth();
	if (err_code == HCI_ERR_NONE)
	{
		//更新成功
		qDebug() << "check auth success \n";
		return true;
	}
	else
	{
		//更新失败
		qDebug() << QString("check auth return (%1:%2)\n").arg(err_code).arg(hci_get_error_info(err_code));
		return false;
	}
}


void HCIRecognizer::onInit(QHash<QString, QString> options)
{
	std::string appkey = "ed5d5407"; //  options["appKey"].toStdString();
	std::string developerKey = "5ef9e5b01c5f6c7490777ca25469a793"; // options["developerKey"].toStdString();
	std::string cloudUrl = "test.api.hcicloud.com:8888"; // options["cloudUrl"].toStdString();
	std::string authpath = "d:\\hcitest\\auth"; // options["authpath"].toStdString();
	std::string logfilepath = "d:\\hcitest\\log";// options["logfilepath"].toStdString();


	std::string capkey = "hwr.cloud.freewrite";// options["capkey"].toStdString(); //需要运行的灵云能力

	std::string datapath = "d:\\hcitest\\data";// options["datapath"].toStdString();


											   //SYS初始化
	HCI_ERR_CODE err_code = HCI_ERR_NONE;
	//配置串是由"字段=值"的形式给出的一个字符串，多个字段之间以','隔开。字段名不分大小写。
	std::string init_config = "";
	init_config += "appKey=" + appkey;              //灵云应用序号
	init_config += ",developerKey=" + developerKey; //灵云开发者密钥
	init_config += ",cloudUrl=" + cloudUrl;         //灵云云服务的接口地址
	init_config += ",authpath=" + authpath;         //授权文件所在路径，保证可写
	init_config += ",logfilepath=" + logfilepath;   //日志的路径
	init_config += ",loglevel=5";								     //日志的等级
	init_config += ",logfilesize=512";								 //日志文件的大小
																	 //其他配置使用默认值，不再添加，如果想设置可以参考开发手册
	err_code = hci_init(init_config.c_str());
	if (err_code != HCI_ERR_NONE)
	{
		QString msg = QString("hci_init return (%1:%2)\n").arg(err_code).arg(hci_get_error_info(err_code));

		qDebug() << msg;
		emit error(msg);
		return;
	}


	//检测授权，如果过期则更新授权
	if (!CheckAndUpdateAuth())
	{
		QString msg = "CheckAndUpdateAuth failed\n";
		hci_release();
		qDebug() << msg;
		emit error(msg);
		return;
	}

	//ASR初始化
	std::string hwr_config = "";
	hwr_config += "initcapkeys=" + capkey;        //建议在initcapkeys中添加需要使用的所有能力以提高第一次识别的效率
												  // 多个能力以';'隔开 
	hwr_config += ",datapath=" + datapath;        //如果是本地能力，此处指定本地能力依赖资源
	err_code = hci_hwr_init(hwr_config.c_str());
	if (err_code != HCI_ERR_NONE)
	{
		QString msg = QString("hci_hwr_init return (%1:%2) \n").arg(err_code).arg(hci_get_error_info(err_code));

		qDebug() << msg;
		emit error(msg);
		return;
	}
	qDebug() << "hci_hwr_init success \n";


	//session start
	m_nSessionId = -1;
	std::string session_config = "capkey=" + capkey;
	qDebug() << QString("hci_hwr_session_start config[ %1 ]\n").arg(session_config.c_str());
	err_code = hci_hwr_session_start(session_config.c_str(), &m_nSessionId);
	if (err_code != HCI_ERR_NONE)
	{
		QString msg = QString("hci_hwr_session_start return (%1:%2)\n").arg(err_code).arg(hci_get_error_info(err_code));

		qDebug() << msg;
		emit error(msg);

		return;
	}
	qDebug() << "hci_hwr_session_start success\n";
}

void HCIRecognizer::onRecognize(STROKES strokes)
{
	QVector<short> shortData = pack(strokes);

	qDebug() << "strokes =" << shortData;

	QStringList resultList;


	HCI_ERR_CODE err_code = HCI_ERR_NONE;

	//hwr recog
	HWR_RECOG_RESULT hwrRecogResult;
	std::string recog_config = "";

	QTime time;
	time.start();

	err_code = hci_hwr_recog(m_nSessionId, shortData.data(), shortData.size() * sizeof(short), recog_config.c_str(), &hwrRecogResult);
	//err_code = hci_hwr_recog(nSessionId, g_sGestureStrokeData, sizeof(g_sGestureStrokeData), recog_config.c_str(), &hwrRecogResult); //笔势识别

	qDebug() << QString("Recog time: %1\n").arg(time.elapsed());

	if (err_code == HCI_ERR_NONE)
	{
		qDebug() << "hci_hwr_recog success\n";

		for (int index = 0; index < (int)hwrRecogResult.uiResultItemCount; ++index)
		{
			HWR_RECOG_RESULT_ITEM& item = hwrRecogResult.psResultItemList[index];
			resultList << QString::fromUtf8(item.pszResult);
		}
		hci_hwr_free_recog_result(&hwrRecogResult);

		emit recognizeResult(resultList);
	}
	else
	{
		qDebug() << QString("hci_hwr_recog return (%1:%2)\n").arg(err_code).arg(hci_get_error_info(err_code));
	}
}

QVector<short> HCIRecognizer::pack(STROKES strokes)
{
	QVector<short> data;
	for each (STROKE stroke in strokes)
	{
		for (int i = 0; i < stroke[0].size() - 1; i++)
		{
			data << stroke[0].at(i) << stroke[1].at(i);
		}
		data << -1 << 0;
	}

	data << -1 << -1;	

	return data;
}