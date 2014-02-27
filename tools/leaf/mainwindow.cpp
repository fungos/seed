#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>

#include "messagelistener.h"
#include "memorywidget.h"
#include "memorymodel.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, pDefaultStatusBar(nullptr)
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
	connect(pMessageListener, SIGNAL(onPrintLog(const QString &)),   this, SLOT(onPrintLog(const QString &)));
	connect(pMessageListener, SIGNAL(onPrintError(const QString &)), this, SLOT(onPrintError(const QString &)));
	connect(pMessageListener, SIGNAL(onPrintDebug(const QString &)), this, SLOT(onPrintDebug(const QString &)));
	connect(pMessageListener, SIGNAL(onAllocation(const PacketAllocationInfo *)), this, SLOT(onAllocation(const PacketAllocationInfo *)));
	connect(pMessageListener, SIGNAL(onFree(const PacketFreeInfo *)), this, SLOT(onFree(const PacketFreeInfo *)));
	connect(pMessageListener, SIGNAL(onStart()), this, SLOT(onStart()));
	connect(pMessageListener, SIGNAL(onStop()), this, SLOT(onStop()));
	connect(pMainWidget, SIGNAL(currentChanged(int)), this, SLOT(onTabChanged(int)));

	pMainWidget->setCurrentWidget(pMemoryWidget);
	mTitle = tr("Seed/Leaf Inspector");
	this->setWindowTitle(mTitle);
}

void MainWindow::onTabChanged(int index)
{
	// FIXME: create a widget with contents and statusbar, we may add more things after.
	auto widget = pMainWidget->widget(index);

	if (widget == pMemoryWidget)
	{
		setStatusBar(pMemoryWidget->statusBar());
	}
	else
	{
		auto defaultStatusBar = new QStatusBar;
		defaultStatusBar->addWidget(new QLabel(tr("Hello")));
		setStatusBar(defaultStatusBar);
	}
}

void MainWindow::resetWidgets()
{
	pLog->clear();
	pError->clear();
	pDebug->clear();
	pConsole->clear();
	pMemoryWidget->clear();
}

void MainWindow::onLog(const QString &msg)
{
	pConsole->append(msg);
}

void MainWindow::onPrintLog(const QString &msg)
{
	pLog->append(msg);
}

void MainWindow::onPrintError(const QString &msg)
{
	pError->append(msg);
}

void MainWindow::onPrintDebug(const QString &msg)
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

void MainWindow::onStart()
{
	resetWidgets();
	this->setWindowTitle(QString("%0 - %1").arg(mTitle).arg(tr("Link Up")));
}

void MainWindow::onStop()
{
	this->setWindowTitle(QString("%0 - %1").arg(mTitle).arg(tr("Link Down")));
}

MainWindow::~MainWindow()
{
	delete ui;
}
