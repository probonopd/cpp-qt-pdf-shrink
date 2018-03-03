#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
private:
    void createActions();
    void loadSettings();
    void saveSettings();
    void open();
    void processFile(QString filename);
protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
private slots:
    void preferences();
    void startProcessFile();
    void finishProcessFile(int exitCode, QProcess::ExitStatus);
    void about();
private:
	QToolButton *runButton;
    QLabel *statusLabel;
    QAction *openAction;

	QString settingsPath;
	QString ghostscriptPath{};
    QStringList ghostscriptOptions {
        "-dPDFSETTINGS=/prepress",
        "-dSAFER",
        "-dCompatibilityLevel=1.5",
        "-dNOPAUSE",
        "-dBATCH",
        "-sDEVICE=pdfwrite",
        "-sstdout=%stderr",
        "-dGrayImageResolution=600",
        "-dMonoImageResolution=1200",
        "-dColorImageResolution=300",
        "-sOutputFile=\"%1\"",
        "-c .setpdfwrite",
        "-f \"%2\""
    };
};
#endif

