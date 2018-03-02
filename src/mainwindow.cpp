#include "mainwindow.h"

#include <QtWidgets>
#include <QDebug>
#include <QString>


#include <QtWidgets>
#include <QVBoxLayout>

#include <QUrl>
#include <QDragEnterEvent>

#include <QApplication>
#include <QSettings>

#include <QFileInfo>

MainWindow::MainWindow() :
    settingsPath{QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)+"/pdf-shrink.conf"}
{
    this->setFixedSize(200, 400);

	runButton = new QToolButton(this);
    QSize iconSize(200, 200);
    runButton->setIconSize(iconSize);
	runButton->setIcon(QIcon(":download.png"));
	runButton->setText("Process &File");

	QPushButton *quitButton = new QPushButton("Quit");
    connect(quitButton, SIGNAL (released()),this, SLOT(close()));

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(runButton);
	layout->addWidget(quitButton);

	QWidget *window = new QWidget();
	window->setLayout(layout);

	setCentralWidget(window);



    setAcceptDrops(true);
	loadSettings();

    createActions();
}

void MainWindow::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QAction *exitAction = fileMenu->addAction(tr("E&xit"), this, &QWidget::close);
    exitAction->setShortcuts(QKeySequence::Quit);

    QAction *openAction = fileMenu->addAction(tr("Process &File..."), this, &MainWindow::open);
	runButton->setDefaultAction(openAction);
	runButton->setIcon(QIcon(":download.png")); // setDefaultAction takes the icon from the action...

    QAction *preferencesAct = fileMenu->addAction(tr("&Preferences"), this, &MainWindow::preferences);
    preferencesAct->setShortcuts(QKeySequence::Preferences);

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    // add an help entry that responds to F1
}

void MainWindow::loadSettings()
{
	QSettings settings(settingsPath, QSettings::NativeFormat);
	ghostscriptPath = settings.value("path", "gs").toString();
}

void MainWindow::saveSettings()
{
    QSettings settings(settingsPath, QSettings::NativeFormat);
    settings.setValue("path", ghostscriptPath);
    settings.sync();
}

void MainWindow::open()
{
	QString processFilename = QFileDialog::getOpenFileName(this,
		tr("Open file to be shrinked"),
		"",
		tr("Pdf files (*.pdf);;All Files (*)")
	);

	if (processFilename.isEmpty()) {
		return;
	}

	processFile(processFilename);
}

/**
 * TODO:
 * - Move to app?
 */
void MainWindow::processFile(QString inputfile)
{

        QFileInfo inputinfo{inputfile};
        // qDebug() << inputinfo.absoluteDir().absolutePath();

        QString targetfile = QFileDialog::getSaveFileName(this,
            tr("Save shrinked file"),
            inputinfo.absoluteDir().absolutePath(),
            tr("Pdf files (*.pdf);;All Files (*)")
        );

        if (targetfile.isEmpty()) {
            return;
        }

        QProcess *process = new QProcess(this);
        QString command = ghostscriptPath + " "+
            ghostscriptOptions.join(" ").arg(targetfile, inputfile);

        // qDebug() << command;
        process->start(command);
        if (!process->waitForFinished()) {
            process->close();
            // qDebug() << "failed";
        }
        // qDebug() << process->readAllStandardError();
        // qDebug() << "success";
}

void MainWindow::preferences()
{
    bool ok;
    QString path = QInputDialog::getText(this,
		tr("Preferences"),
		tr("Path to Ghostscript:"), QLineEdit::Normal,
		ghostscriptPath,
	&ok);
    if (ok && !path.isEmpty()) {
		qDebug() << path;
		ghostscriptPath = path;
		saveSettings();
    }
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Pdf Shrink"),
        tr("Shrink a Pdf by calling Ghostscript"));
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *e)
{
    foreach (const QUrl &url, e->mimeData()->urls()) {
        QString filename = url.toLocalFile();
        qDebug() << "Dropped file:" << filename;

        processFile(filename);
    }
}
