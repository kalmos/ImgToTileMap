//
// MainWindow.cpp for ImgConvert
// 
// Made by lucas stefas
// Login   stefas_l
// 
// Started on 20/05/2014 14:25 2014 by lucas stefas
//

#include "moc/MainWindow.h"
#include <qfiledialog.h>
#include <fstream>
#include <iostream>
#include <math.h>

#define TILESIZE 16

bool compareVec8X8(const unsigned int* v1, const unsigned int* v2);
unsigned int getMaxSquare(unsigned int elem);

MainWindow::MainWindow(const QString& path, QWidget* parent):
QMainWindow(parent)
{
	(void)path;
	this->imgPreCut = NULL;
	this->tileMap = NULL;
	this->ui.setupUi(this);
	
	this->ui.tableWidget->setColumnWidth(0, 40);
	this->ui.tableWidget->setIconSize(QSize(40, 40));
	
}

void MainWindow::on_ImportButton_clicked()
{
	QString imgFile = QFileDialog::getOpenFileName(this);
	
	if (imgFile != "")
	{
		ext = QFileInfo(imgFile).suffix();
		importImage(imgFile);
		populateQTableWidget();
	}
}

void MainWindow::on_Generate_clicked()
{
	if (this->imgPreCut)
	{
		if (updateTileMap())
		{
			generateTileSetToSave();
			generateTileMapToSave();
			save();
		}
	}
}

void MainWindow::on_ImgToTile_clicked()
{
	unsigned int x = 0;
	unsigned int y = 0;
	int sX = 0;
	int sY = 0;
	unsigned int i = 0;
	int black = 0;
	unsigned int ii = 0;
	std::list<unsigned int*> tileCutImg;
	std::list<unsigned int*>::iterator it1;
	std::list<unsigned int*>::iterator it2;
	unsigned int *tmp = NULL;
	
	while (i < this->origPal.size())
	{
		if (this->origPal[i] == 0xFF000000)
		{
			black = i;
			break;
		}
		i += 1;
	}
	if (i == this->origPal.size())
	{
		this->origPal[i] = 0xFF000000;
		black = i;
	}
	
	while (x < this->width)
	{
		y = 0;
		while (y < this->height)
		{
			i = 0;
			sX = 0;
			tileCutImg.push_back(new unsigned int[TILESIZE * TILESIZE]);
			tmp = tileCutImg.back();
			while (sX < TILESIZE)
			{
				sY = 0;
				while (sY < TILESIZE)
				{
					if ((x + sX) >= this->width || (y + sY) >= this->height)
						tmp[i] = black;
					else
						tmp[i] = this->imgPreCut[x + sX + ((y + sY) * this->width)];
					sY += 1;
					i += 1;
				}
				sX += 1;
			}
			y += TILESIZE;
		}
		x += TILESIZE;
	}
	
	std::cout << "TileNum " << tileCutImg.size() << std::endl;
	
	if (tileCutImg.size() > 1)
	{
		it1 = tileCutImg.begin();
		it2 = tileCutImg.begin();
		it2++;
	
		while (it2 != tileCutImg.end())
		{
			if (it1 != it2 && compareVec8X8(*it1, *it2))
			{
				tileCutImg.erase(it1);
				it1 = tileCutImg.begin();
				it2 = tileCutImg.begin();
				if (tileCutImg.size() > 1)
					it2++;
			}
			else
			{
				it1++;
				it2++;
			}
		}
		if (tileCutImg.size() > 1)
		{
			it2 = tileCutImg.begin();
			if (it1 != it2 && compareVec8X8(*it1, *it2))
				tileCutImg.erase(it1);
		}
	}
	std::cout << "TileNum " << tileCutImg.size() << std::endl;
	
	this->tileMap = new unsigned int[tileCutImg.size() * TILESIZE * TILESIZE];
	while (tileCutImg.size())
	{
		tmp = tileCutImg.front();
		i = 0;
		while (i < TILESIZE * TILESIZE)
		{
			
			this->tileMap[ii] = tmp[i];
			i += 1;
			ii += 1;
		}
		delete tmp;
		tileCutImg.pop_front();
	}
	
	//QImage img = QImage(this->width, this->height, this->form);
	QImage img = QImage(TILESIZE, TILESIZE * 12, this->form);
	
	x = 0;
	while (x < TILESIZE)//this->width)
	{
		y = 0;
		while (y < TILESIZE * 12)//this->height)
		{
			img.setPixel(x, y, this->origPal[this->tileMap[x + y * TILESIZE/*this->width*/]]);
			y += 1;
		}
		x += 1;
	}
		
	img = img.scaled(this->ui.label->width(), this->ui.label->height());
	this->ui.label->setPixmap(QPixmap::fromImage(img));
}

void	MainWindow::createTileSetTileMap()
{
	std::list<std::vector<unsigned int> >::iterator itG = this->tileset.begin();
	std::list<unsigned int>::iterator itM;
	std::list<Tile>::iterator itS;
	int i = 0;
	
	while (itG != this->tileset.end())
	{
		itS = this->tileSetImage.begin();
		i = 0;
		while (itS != this->tileSetImage.end())
		{
			if (*itG == (*itS).data)
			{
				this->tileMapImage.push_back(i);
				break;
			}
			i += 1;
			itS++;
		}
		if (itS == this->tileSetImage.end())
		{
			this->tileSetImage.push_back(Tile(*itG));
			this->tileMapImage.push_back(i);
		}
		itG++;
	}
	
	std::cout << tileMapImage.size() << std::endl;
	std::cout << tileSetImage.size() << std::endl;
}

void	MainWindow::importImage(QString imgFile)
{
	QImage	img;
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int x2 = 0;
	unsigned int y2 = 0;
	std::vector<unsigned int> tile(TILESIZEX*TILESIZEY, 0);
	
	img.load(imgFile);
	
	this->form = img.format();
	this->width = img.width();
	this->height = img.height();
	this->imgPreCut = new unsigned int[this->width * this->height];
	
	while (x < this->width)
	{
		y = 0;
		while (y < this->height)
		{
			
			x2 = 0;
			while (x2 < TILESIZEX)
			{
				y2 = 0;
				while (y2 < TILESIZEY)
				{
					if (x + x2 < this->width && y + y2 < this->height)
						tile[x2 + (y2 * TILESIZEX)] = img.pixel(x + x2, y + y2);
					else
						tile[x2 + (y2 * TILESIZEX)] = 0;
					y2 += 1;
				}
				x2 += 1;
			}
			this->tileset.push_back(tile);
			y += TILESIZEY;
		}
		x += TILESIZEX;
	}
	
	img = img.scaled(this->ui.label->width(), this->ui.label->height());
	
	this->ui.label->setPixmap(QPixmap::fromImage(img));
}

void	MainWindow::populateQTableWidget()
{
	QImage	miniImg;
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int i = 0;
	std::list<Tile>::iterator it;
	QTableWidgetItem item1;
	QTableWidgetItem item2;
	
	createTileSetTileMap();
	it = this->tileSetImage.begin();
	miniImg = QImage(TILESIZEX, TILESIZEY, this->form);
	while (it != this->tileSetImage.end())
	{
		miniImg = QImage(TILESIZEX, TILESIZEY, this->form);
		x = 0;
		while (x < TILESIZEX)
		{
			y = 0;
			while (y < TILESIZEY)
			{
				miniImg.setPixel(x, y, (*it).data[x + (y * TILESIZEX)]);
				y += 1;
			}
			x += 1;
		}
		it++;
		miniImg = miniImg.scaled(40, 40);
		item1.setSizeHint(QSize(40, 40));
		item1.setIcon(QIcon(QPixmap().fromImage(miniImg)));
		item1.setFlags(Qt::ItemIsEnabled);
		ui.tableWidget->insertRow(i);
		this->ui.tableWidget->setRowHeight(i, 40);
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(item1));
		item2.setText("1");
		item2.setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
		ui.tableWidget->setItem(i, 1, new QTableWidgetItem(item2));
		i += 1;
	}
}

void	MainWindow::generateTileSetToSave()
{
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int i = 0;
	unsigned int tsSize = 0;
	std::list<Tile>::iterator it;
	
	it = this->tileSetImageUpdated.begin();
	tsSize = getMaxSquare(this->tileSetImageUpdated.size());
	std::cout << "zaeaze" << this->tileSetImageUpdated.size() << std::endl;
	tileSetToSave = QImage(tsSize * TILESIZEX, tsSize * TILESIZEY, this->form);
	tileSetToSave.fill(0);
	while (it != this->tileSetImageUpdated.end())
	{
		x = 0;
		while (x < TILESIZEX)
		{
			y = 0;
			while (y < TILESIZEY)
			{
				tileSetToSave.setPixel(x + (TILESIZEX * (i % tsSize)), y + (TILESIZEY * (i - (i % tsSize)) / tsSize), (*it).data[x + (y * TILESIZEX)]);
				y += 1;
			}
			x += 1;
		}
		it++;
		i += 1;
	}

}

void	MainWindow::generateTileMapToSave()
{
	unsigned int x = 0;
	unsigned int y = 0;
	int xx = 0;
	int yy = 0;
	int fram = 0;
	std::list<unsigned int>::iterator it = this->tileMapImage.begin();
	
	tileMapToSave = QImage(this->width / TILESIZEX, this->height / TILESIZEY, this->form);
	while (it != this->tileMapImage.end())
	{
		x = 0;
		while (x < this->width / TILESIZEX)
		{
			y = 0;
			while (y < this->height / TILESIZEY)
			{
				getTileInfo(*it, xx, yy, fram);
				tileMapToSave.setPixel(x, y, qRgba(fram, yy, xx, 255));
				it++;
				y += 1;
			}
			x += 1;
		}
		
	}
}

void	MainWindow::getTileInfo(unsigned int id, int& x, int& y, int& fram)
{
	std::list<Tile>::iterator it = tileSetImageUpdated.begin();
	unsigned int tsSize = getMaxSquare(tileSetImageUpdated.size());
	int i = 0;
	
	while (it != tileSetImageUpdated.end())
	{
		if ((*it).id == id)
		{
			fram = (*it).ocur;
			x = i % tsSize;
			y = (i - (i % tsSize)) / tsSize;
			return;
		}
		it++;
		i += 1;
	}
}

bool	MainWindow::updateTileMap()
{
	std::list<Tile>::iterator it = tileSetImage.begin();
	int i = 0;
	unsigned int ii = 0;
	bool ok;
	unsigned int ocur;
	
	while (it != tileSetImage.end())
	{
		ocur = ui.tableWidget->item(ii, 1)->text().toInt(&ok);
		if (!ok || ocur == 0 || ocur > 256)
			return (false);
		(*it).ocur = ocur;
		i = ocur - 1;
		while (i > -1)
		{
			tileSetImageUpdated.push_back(Tile((*it).data, ocur, ii));
			i -= 1;
		}
		ii += 1;
		it++;
	}
	std::cout << tileSetImage.size() << " " << tileSetImageUpdated.size() << std::endl;
	
	return (true);
}

void	MainWindow::reCreateTileSetTileMap()
{
	std::list<std::vector<unsigned int> >::iterator itG = this->tileset.begin();
	std::list<unsigned int>::iterator itM;
	std::list<Tile>::iterator itS;
	int i = 0;
	
	while (itG != this->tileset.end())
	{
		itS = this->tileSetImage.begin();
		i = 0;
		while (itS != this->tileSetImage.end())
		{
			if (*itG == (*itS).data)
			{
				this->tileMapImage.push_back(i);
				break;
			}
			i += 1;
			itS++;
		}
		if (itS == this->tileSetImage.end())
		{
			this->tileSetImage.push_back(Tile(*itG));
			this->tileMapImage.push_back(i);
		}
		itG++;
	}
	
	std::cout << tileMapImage.size() << std::endl;
	std::cout << tileSetImage.size() << std::endl;
}

void MainWindow::save()
{
	QString saveBase = QFileDialog::getSaveFileName(this);
	
	std::ofstream file;
	
	if (saveBase != "")
	{
		if (this->ui.BMPButton->isChecked())
		{
			tileSetToSave.save(saveBase + "_pal." + ext);
			tileMapToSave.save(saveBase + "_map." + ext);
		}
		else if (this->ui.CHButton->isChecked())
			writeCHFile(saveBase);
	}
}

bool MainWindow::writeColor(QFile& file, QRgb color, int width, int height)
{
	QByteArray conv;
	//QRgb c = color.rgba();
	
	file.write("0x");
	
	//std::cout << qRed(c) << " " << qGreen(c) << " " << qBlue(c) << " " << qAlpha(c) << " " <<
	//color.red() << " " << color.green() << " " << color.blue() << " " << color.alpha() << std::endl;
	
	
	
	//conv = QByteArray::number(color.alpha(), 16);
	conv = QByteArray::number(qAlpha(color), 16);
	if (conv.size() == 1)
		file.write(QString("0" + conv).toUtf8().constData());
	else
		file.write(QString(conv).toUtf8().constData());
	//conv = QByteArray::number(color.blue(), 16);
	conv = QByteArray::number(qBlue(color), 16);
	if (conv.size() == 1)
		file.write(QString("0" + conv).toUtf8().constData());
	else
		file.write(QString(conv).toUtf8().constData());
	//conv = QByteArray::number(color.green(), 16);
	conv = QByteArray::number(qGreen(color), 16);
	if (conv.size() == 1)
		file.write(QString("0" + conv).toUtf8().constData());
	else
		file.write(QString(conv).toUtf8().constData());
	//conv = QByteArray::number(color.red(), 16);
	conv = QByteArray::number(qRed(color), 16);
	if (conv.size() == 1)
		file.write(QString("0" + conv).toUtf8().constData());
	else
		file.write(QString(conv).toUtf8().constData());
	
	if (width == tileSetToSave.width() - 1 && height == tileSetToSave.height() - 1)
	{
	}
	else
		file.write(",");
	return (true);
}

bool MainWindow::writeCHFile(QString fileName)
{
	QFile fileC(fileName + ".c");
	QFile fileH(fileName + ".h");
	QString arrayName;
	int width = 0;
	int height = 0;
	
	fileC.open(QIODevice::WriteOnly);
	arrayName = QFileInfo(fileC).baseName();
	fileC.write("#include <stdint.h>\n");
	fileC.write(("extern const uint32_t " + arrayName + "Pal[" + QString::number(tileSetToSave.width()) + "*" + QString::number(tileSetToSave.height()) + "] = {\n").toUtf8().constData());
	
	while (width < tileSetToSave.width())
	{
		height = 0;
		while (height < tileSetToSave.height())
		{
			writeColor(fileC, tileSetToSave.pixel(height, width), width, height);
			height += 1;
		}
		fileC.write("\n");
		width += 1;
	}
	
	fileC.write("};\n\n");
	width = 0;
	fileC.write(("extern const uint32_t " + arrayName + "Map[" + QString::number(tileMapToSave.width()) + "*" + QString::number(tileMapToSave.height()) + "] = {\n").toUtf8().constData());
	
	
	//while (width < tileMapToSave.width())
	//{
	//	height = 0;
	//	while (height < tileMapToSave.height())
	//	{
	//		writeColor(fileC, tileMapToSave.pixel(width, height) /*& 0XFFFFFF*/, width, height);
	//		height += 1;
	//	}
	//	fileC.write("\n");
	//	width += 1;
	//}
	
	height = 0;
	while (height < tileMapToSave.height())
	{
		width = 0;
		while (width < tileMapToSave.width())
		{
			writeColor(fileC, tileMapToSave.pixel(width, height) /*& 0XFFFFFF*/, width, height);
			width += 1;
		}
		fileC.write("\n");
		height += 1;
	}
	
	
	fileC.write("};\n");
	fileC.close();
	
	fileH.open(QIODevice::WriteOnly);
	fileH.write("#include <stdint.h>\n");
	fileH.write(("extern const uint32_t " + arrayName + "Pal[" + QString::number(tileSetToSave.width()) + "*" + QString::number(tileSetToSave.height()) + "];\n").toUtf8().constData());
	fileH.write(("extern const uint32_t " + arrayName + "Map[" + QString::number(tileMapToSave.width()) + "*" + QString::number(tileMapToSave.height()) + "];\n").toUtf8().constData());
	fileH.close();
	return (0);
}

MainWindow::~MainWindow()
{
	
}



bool compareVec8X8(const unsigned int* v1, const unsigned int* v2)
{
	int i = 0;
	while (i < TILESIZE * TILESIZE)
	{
		if (v1[i] != v2[i])
			return (false);
		i += 1;
	}
	return (true);
}

unsigned int getMaxSquare(unsigned int elem)
{
	float fval = sqrtf(elem);
	unsigned int ival = (unsigned int)fval;
	
	if (ival < fval)
		return (ival + 1);
	else
		return (ival);
}