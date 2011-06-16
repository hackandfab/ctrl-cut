#include "MainWindow.h"
#include <QtGui>

#include "FileParser.h"
#include "vector/CutModel.h"
#include "vector/Geometry.h"

#include "LpdClient.h"

#include <assert.h>

MainWindow::MainWindow()
{
  this->lpdclient = new LpdClient(this);
  this->lpdclient->setObjectName("lpdclient");

  setupUi(this);
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_fileOpenAction_triggered()
{
  QString filename = QFileDialog::getOpenFileName(this, "Open File", "", "Supported files (*.ps *.vector)");
  if (!filename.isEmpty()) {
    CutModel *cut = NULL;


    QFileInfo finfo(filename);
    QString suffix = finfo.suffix();
    if (suffix == "ps") {
      PostscriptParser *psparser = new PostscriptParser(LaserConfig::inst());
      switch (LaserConfig::inst().raster_dithering) {
      case LaserConfig::DITHER_DEFAULT:
        psparser->setRasterFormat(PostscriptParser::BITMAP);
        break;
      case LaserConfig::DITHER_THRESHOLD:
      case LaserConfig::DITHER_BAYER:
      case LaserConfig::DITHER_FLOYD_STEINBERG:
      case LaserConfig::DITHER_FLOYD_JARVIS:
      case LaserConfig::DITHER_FLOYD_BURKE:
      case LaserConfig::DITHER_FLOYD_STUCKI:
      case LaserConfig::DITHER_FLOYD_SIERRA2:
      case LaserConfig::DITHER_FLOYD_SIERRA3:
        psparser->setRasterFormat(PostscriptParser::GRAYSCALE);
        break;
        
      default:
        assert(false);
      }
      cups_file_t *input_file = cupsFileOpen(filename.toLocal8Bit(), "r");
      if (!input_file) {
        LOG_ERR("Error opening postscript file");
        return;
      }
      LaserConfig::inst().basename = finfo.baseName().toStdString();
      if (!psparser->parse(input_file)) {
        LOG_FATAL("Error processing postscript");
        return;
      }

      cut = CutModel::load(psparser->getVectorData());
      if (!cut) {
        fprintf(stderr, "Error: Unable to open postscript file\n");
        return;
      }
    }
    else if (suffix == "vector") {
      cut = CutModel::load(filename.toStdString());
      if (!cut) {
        fprintf(stderr, "Error: Unable to open vector file\n");
        return;
      }
    }

    if (cut) {
      for (CutModel::SegmentIter iter = cut->beginSegments(); iter != cut->endSegments(); iter++) {
        const Segment &segment = **iter;
        this->graphicsView->scene()->addLine(segment[0][0], segment[0][1], segment[1][0], segment[1][1]);
      }
    }
  }
}

void MainWindow::on_filePrintAction_triggered()
{
  QString filename = QFileDialog::getOpenFileName(this, "Open RTL File", "", "Supported files (*.prn *.raw)");
  if (!filename.isEmpty()) {
    printf("RTL file: %s\n", filename.toLocal8Bit().data());

    QFile rtlfile(filename);
    if (!rtlfile.open(QIODevice::ReadOnly)) {
      fprintf(stderr, "Unable to open RTL file\n");
      return;
    }

    this->lpdclient->print("MyDocument", rtlfile.readAll());
    rtlfile.close();

  }
}

void MainWindow::on_lpdclient_done(bool error)
{
  if (error) fprintf(stderr, "LPD error\n");
  else printf("LPD done\n");
}

void MainWindow::on_lpdclient_progress(int done, int total)
{
  printf("Progress: %.0f%%\n", 100.0f*done/total);
}



