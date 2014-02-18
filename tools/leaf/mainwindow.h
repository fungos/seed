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

	public slots:
		void onLog(const QString &msg);
		void onLeafPrintLog(const QString &msg);
		void onLeafPrintError(const QString &msg);
		void onLeafPrintDebug(const QString &msg);
		void onAllocation(const PacketAllocationInfo *msg);
		void onFree(const PacketFreeInfo *msg);

	private:
		Ui::MainWindow *ui;
		QTabWidget *pMainWidget;
		QTextEdit *pConsole;
		QTextEdit *pLog;
		QTextEdit *pError;
		QTextEdit *pDebug;
		MemoryWidget *pMemoryWidget;
		MemoryModel *pMemoryModel;

		MessageListener *pMessageListener;
};

#endif // MAINWINDOW_H
