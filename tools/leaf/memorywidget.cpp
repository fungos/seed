#include "memorywidget.h"

#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QTreeView>
#include <QDebug>
#include <QCheckBox>

#include "memorysortfiltermodel.h"
#include "memorymodel.h"
#include "filterwidget.h"

MemoryWidget::MemoryWidget(QWidget *parent)
	: QWidget(parent)
{
	pProxyModel = new MemorySortFilterModel(this);

	filterWidget = new FilterWidget;
	filterWidget->setText("");

	filterPatternLabel = new QLabel(tr("&Pattern:"));
	filterPatternLabel->setBuddy(filterWidget);

	fromFrame = new QLineEdit;
	fromFrameLabel = new QLabel(tr("From &Frame:"));
	fromFrameLabel->setBuddy(fromFrame);

	toFrame = new QLineEdit;
	toFrameLabel = new QLabel(tr("To:"));
	toFrameLabel->setBuddy(toFrame);

	fromAddr = new QLineEdit;
	fromAddrLabel = new QLabel(tr("From &Address:"));
	fromAddrLabel->setBuddy(fromAddr);

	toAddr = new QLineEdit;
	toAddrLabel = new QLabel(tr("To:"));
	toAddrLabel->setBuddy(toAddr);

	showHexAddrCheck = new QCheckBox;
	showHexAddrCheck->setChecked(true);
	showHexAddrLabel = new QLabel(tr("Hex Addresses"));
	showHexAddrLabel->setBuddy(showHexAddrCheck);

	showFreedCheck = new QCheckBox;
	showFreedCheck->setChecked(false);
	showFreedLabel = new QLabel(tr("Show Freed"));
	showFreedLabel->setBuddy(showFreedCheck);

	connect(filterWidget,     SIGNAL(filterChanged()),      this, SLOT(textFilterChanged()));
	connect(filterWidget,     SIGNAL(textChanged(QString)), this, SLOT(textFilterChanged()));
	connect(fromFrame,        SIGNAL(textChanged(QString)), this, SLOT(frameFilterChanged()));
	connect(toFrame,          SIGNAL(textChanged(QString)), this, SLOT(frameFilterChanged()));
	connect(fromAddr,         SIGNAL(textChanged(QString)), this, SLOT(addrFilterChanged()));
	connect(toAddr,           SIGNAL(textChanged(QString)), this, SLOT(addrFilterChanged()));
	connect(showFreedCheck,   SIGNAL(stateChanged(int)),    this, SLOT(showFreedChanged(int)));
	connect(showHexAddrCheck, SIGNAL(stateChanged(int)),    this, SLOT(showHexAddrChanged(int)));

	pProxyView = new QTreeView;
	pProxyView->setSortingEnabled(true);
	pProxyView->setRootIsDecorated(false);
	pProxyView->setAlternatingRowColors(true);
	pProxyView->setModel(pProxyModel);
	pProxyView->sortByColumn(0, Qt::DescendingOrder);

	QGridLayout *proxyLayout = new QGridLayout;
	proxyLayout->addWidget(pProxyView, 0, 0, 1, 4);
	proxyLayout->addWidget(filterPatternLabel, 1, 0);
	proxyLayout->addWidget(filterWidget, 1, 1, 1, 4);
	proxyLayout->addWidget(fromFrameLabel, 3, 0);
	proxyLayout->addWidget(fromFrame, 3, 1);
	proxyLayout->addWidget(toFrameLabel, 3, 2);
	proxyLayout->addWidget(toFrame, 3, 3);
	proxyLayout->addWidget(fromAddrLabel, 4, 0);
	proxyLayout->addWidget(fromAddr, 4, 1);
	proxyLayout->addWidget(toAddrLabel, 4, 2);
	proxyLayout->addWidget(toAddr, 4, 3);
	proxyLayout->addWidget(showFreedCheck, 5, 0);
	proxyLayout->addWidget(showFreedLabel, 5, 1);
	proxyLayout->addWidget(showHexAddrCheck, 5, 2);
	proxyLayout->addWidget(showHexAddrLabel, 5, 3);

	proxyGroupBox = new QGroupBox(tr("Allocations"));
	proxyGroupBox->setLayout(proxyLayout);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(proxyGroupBox);
	setLayout(mainLayout);
}

void MemoryWidget::setModel(MemoryModel *model)
{
	pModel = model;
	pProxyModel->setSourceModel(model);
	pProxyView->setModel(pProxyModel);

	pProxyView->setColumnHidden(7, true); // line
	pProxyView->setColumnHidden(8, true); // freed
}

void MemoryWidget::frameFilterChanged()
{
	pProxyModel->setFilterMinimumFrame(fromFrame->text().toUInt());
	pProxyModel->setFilterMaximumFrame(toFrame->text().toUInt());
}

void MemoryWidget::addrFilterChanged()
{
	pProxyModel->setFilterMinimumAddr(fromAddr->text().toUInt());
	pProxyModel->setFilterMaximumAddr(toAddr->text().toUInt());
}

void MemoryWidget::textFilterChanged()
{
	QRegExp regExp(filterWidget->text(), filterWidget->caseSensitivity(), filterWidget->patternSyntax());
	pProxyModel->setFilterRegExp(regExp);
}

void MemoryWidget::showFreedChanged(int state)
{
	pProxyModel->hideFreed(state == Qt::Unchecked);
}

void MemoryWidget::showHexAddrChanged(int state)
{
	pModel->setHexadecimalAddress(state == Qt::Checked);
}
