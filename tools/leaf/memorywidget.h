#ifndef MEMORYWIDGET_H
#define MEMORYWIDGET_H

#include <QWidget>

class QGroupBox;
class QLabel;
class QAbstractItemModel;
class QLineEdit;
class QTreeView;
class QCheckBox;
class QStatusBar;

class MemorySortFilterModel;
class MemoryView;
class MemoryModel;
class FilterWidget;

class MemoryWidget : public QWidget
{
		Q_OBJECT
	public:
		explicit MemoryWidget(QWidget *parent = 0);
		void setModel(MemoryModel *model);
		void clear();
		QStatusBar *statusBar();

	signals:

	public slots:
		void textFilterChanged();
		void frameFilterChanged();
		void addrFilterChanged();
		void showFreedChanged(int state);
		void showHexAddrChanged(int state);
		void totalsChanged(quint64 total, quint64 maxTotal, quint64 maxUnique);

	private:
		MemoryModel *pModel;
		MemorySortFilterModel *pProxyModel;
		QTreeView *pProxyView;

		// FIXME: names...
		QGroupBox *proxyGroupBox;
		QLabel *filterPatternLabel;
		FilterWidget *filterWidget;
		QLabel *fromFrameLabel;
		QLabel *toFrameLabel;
		QLineEdit *fromFrame;
		QLineEdit *toFrame;
		QLabel *fromAddrLabel;
		QLabel *toAddrLabel;
		QLineEdit *fromAddr;
		QLineEdit *toAddr;
		QLabel *showHexAddrLabel;
		QCheckBox *showHexAddrCheck;
		QLabel *showFreedLabel;
		QCheckBox *showFreedCheck;
		QLabel *totalMemLabel;
		QLabel *maxTotalMemLabel;
		QLabel *maxUniqueMemLabel;

		quint64 iTotal;
		quint64 iMaxTotal;
		quint64 iMaxUnique;
};

#endif // MEMORYWIDGET_H
