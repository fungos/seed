#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTextEdit>

namespace Ui {
	class MainWindow;
}

struct PacketAllocationInfo;
struct PacketFreeInfo;

class MessageListener;
class MemoryWidget;
class MemoryModel;

class MainWindow : public QMainWindow
{
		Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

		void resetWidgets();

	public slots:
		void onLog(const QString &msg);
		void onPrintLog(const QString &msg);
		void onPrintError(const QString &msg);
		void onPrintDebug(const QString &msg);
		void onAllocation(const PacketAllocationInfo *msg);
		void onFree(const PacketFreeInfo *msg);
		void onStart();
		void onStop();
		void onTabChanged(int index);

	private:
		Ui::MainWindow *ui;
		QStatusBar *pDefaultStatusBar;
		QTabWidget *pMainWidget;
		QTextEdit *pConsole;
		QTextEdit *pLog;
		QTextEdit *pError;
		QTextEdit *pDebug;
		MemoryWidget *pMemoryWidget;
		MemoryModel *pMemoryModel;

		MessageListener *pMessageListener;

		QString mTitle;
};

#endif // MAINWINDOW_H
