#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "messagelistener.h"
#include "memorywidget.h"
#include "memorymodel.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, pMainWidget(nullptr)
	, pConsole(nullptr)
	, pLog(nullptr)
	, pError(nullptr)
	, pDebug(nullptr)
	, pMemoryWidget(nullptr)
	, pMessageListener(nullptr)
{
	ui->setupUi(this);

	pMainWidget = new QTabWidget;
	setCentralWidget(pMainWidget);

	pConsole = new QTextEdit;
	pMainWidget->addTab(pConsole, tr("Console"));

	pLog = new QTextEdit;
	pMainWidget->addTab(pLog, tr("Log"));

	pDebug = new QTextEdit;
	pMainWidget->addTab(pDebug, tr("Debug"));

	pError = new QTextEdit;
	pMainWidget->addTab(pError, tr("Error"));

	pMemoryWidget = new MemoryWidget;
	pMemoryModel = new MemoryModel(this);
	pMemoryWidget->setModel(pMemoryModel);
	pMainWidget->addTab(pMemoryWidget, tr("Memory"));

	pMessageListener = new MessageListener;

	connect(pMessageListener, SIGNAL(onLog(const QString &)), this, SLOT(onLog(const QString &)));
	connect(pMessageListener, SIGNAL(onLeafPrintLog(const QString &)),   this, SLOT(onLeafPrintLog(const QString &)));
	connect(pMessageListener, SIGNAL(onLeafPrintError(const QString &)), this, SLOT(onLeafPrintError(const QString &)));
	connect(pMessageListener, SIGNAL(onLeafPrintDebug(const QString &)), this, SLOT(onLeafPrintDebug(const QString &)));
	connect(pMessageListener, SIGNAL(onAllocation(const PacketAllocationInfo *)), this, SLOT(onAllocation(const PacketAllocationInfo *)));
	connect(pMessageListener, SIGNAL(onFree(const PacketFreeInfo *)), this, SLOT(onFree(const PacketFreeInfo *)));

	pMainWidget->setCurrentWidget(pMemoryWidget);
}

void MainWindow::onLog(const QString &msg)
{
	pConsole->append(msg);
}

void MainWindow::onLeafPrintLog(const QString &msg)
{
	pLog->append(msg);
}

void MainWindow::onLeafPrintError(const QString &msg)
{
	pError->append(msg);
}

void MainWindow::onLeafPrintDebug(const QString &msg)
{
	pDebug->append(msg);
}

void MainWindow::onAllocation(const PacketAllocationInfo *msg)
{
	pMemoryModel->alloc(msg);
}

void MainWindow::onFree(const PacketFreeInfo *msg)
{
	pMemoryModel->free(msg);
}

MainWindow::~MainWindow()
{
	delete ui;
}
