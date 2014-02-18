#ifndef MEMORYWIDGET_H
#define MEMORYWIDGET_H

#include <QWidget>

class QGroupBox;
class QLabel;
class QAbstractItemModel;
class QLineEdit;
class QTreeView;
class MemorySortFilterModel;
class MemoryView;
class FilterWidget;

class MemoryWidget : public QWidget
{
		Q_OBJECT
	public:
		explicit MemoryWidget(QWidget *parent = 0);
		void setModel(QAbstractItemModel *model);

	signals:

	public slots:
		void textFilterChanged();
		void frameFilterChanged();
		void addrFilterChanged();

	private:
		MemorySortFilterModel *pProxyModel;
		QTreeView *pProxyView;

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
};

#endif // MEMORYWIDGET_H
