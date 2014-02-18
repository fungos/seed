#include "memorywidget.h"

#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QTreeView>
#include <QDebug>

#include "memorysortfiltermodel.h"
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

	connect(filterWidget, SIGNAL(filterChanged()),      this, SLOT(textFilterChanged()));
	connect(filterWidget, SIGNAL(textChanged(QString)), this, SLOT(textFilterChanged()));
	connect(fromFrame,    SIGNAL(textChanged(QString)), this, SLOT(frameFilterChanged()));
	connect(toFrame,      SIGNAL(textChanged(QString)), this, SLOT(frameFilterChanged()));
	connect(fromAddr,     SIGNAL(textChanged(QString)), this, SLOT(addrFilterChanged()));
	connect(toAddr,       SIGNAL(textChanged(QString)), this, SLOT(addrFilterChanged()));

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

	proxyGroupBox = new QGroupBox(tr("Allocations"));
	proxyGroupBox->setLayout(proxyLayout);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(proxyGroupBox);
	setLayout(mainLayout);
}

void MemoryWidget::setModel(QAbstractItemModel *model)
{
	pProxyModel->setSourceModel(model);
	pProxyView->setModel(model);

	pProxyView->setColumnHidden(0, true);
	pProxyView->setColumnHidden(8, true);
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

