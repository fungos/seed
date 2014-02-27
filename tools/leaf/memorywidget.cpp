#include "memorywidget.h"

#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QTreeView>
#include <QDebug>
#include <QCheckBox>
#include <QStatusBar>

#include "memorysortfiltermodel.h"
#include "memorymodel.h"
#include "filterwidget.h"

MemoryWidget::MemoryWidget(QWidget *parent)
	: QWidget(parent)
	, iTotal(0)
	, iMaxTotal(0)
	, iMaxUnique(0)
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

	totalMemLabel = new QLabel(tr("Total: %1").arg(iTotal));
	maxTotalMemLabel = new QLabel(tr("Maximum: %1").arg(iMaxTotal));
	maxUniqueMemLabel = new QLabel(tr("Biggest: %1").arg(iMaxUnique));

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(proxyGroupBox);
	setLayout(mainLayout);
}

QStatusBar *MemoryWidget::statusBar()
{
	auto w = new QStatusBar;

	// FIXME: MainWindow status bar will keep ownership, so it will destroy sit and its children, here we recreate.
	// in theory we can crash updating status bar when changing tabs... 
	totalMemLabel = new QLabel(tr("Total: %1").arg(iTotal));
	maxTotalMemLabel = new QLabel(tr("Maximum: %1").arg(iMaxTotal));
	maxUniqueMemLabel = new QLabel(tr("Biggest: %1").arg(iMaxUnique));

	w->addWidget(totalMemLabel);
	w->addWidget(maxTotalMemLabel);
	w->addWidget(maxUniqueMemLabel);

	return w;
}

void MemoryWidget::clear()
{
	if (pModel)
		pModel->clear();

	iTotal = 0;
	iMaxTotal = 0;
	iMaxUnique = 0;
}

void MemoryWidget::setModel(MemoryModel *model)
{
	pModel = model;
	pProxyModel->setSourceModel(model);
	pProxyView->setModel(pProxyModel);

	if (model)
	{
		connect(model, SIGNAL(onTotalsChanged(quint64, quint64, quint64)), this, SLOT(totalsChanged(quint64, quint64, quint64)));
	}
	else
	{
		disconnect(this, SLOT(totalsChanged(quint64, quint64, quint64)));
	}

	// FIXME: hardcoded column numbers
	pProxyView->setColumnHidden(8, true); // line
	pProxyView->setColumnHidden(9, true); // freed
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
	pModel->update(); // FIXME: HACK to cause an signal updating view
}

void MemoryWidget::showHexAddrChanged(int state)
{
	pModel->setHexadecimalAddress(state == Qt::Checked);
}

void MemoryWidget::totalsChanged(quint64 total, quint64 maxTotal, quint64 maxUnique)
{
	iTotal = total;
	iMaxTotal = maxTotal;
	iMaxUnique = maxUnique;

	totalMemLabel->setText(tr("Total: %1").arg(total));
	maxTotalMemLabel->setText(tr("Maximum: %1").arg(maxTotal));
	maxUniqueMemLabel->setText(tr("Biggest: %1").arg(maxUnique));
}