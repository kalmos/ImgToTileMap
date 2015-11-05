/*
** MainWindow.h for ImgConvert
**
** Made by lucas stefas
** Login   stefas_l
**
** Started on 20/05/2014 14:14 2014 by lucas stefas
*/

#ifndef		__MainWindow_H__
#define		__MainWindow_H__

#include <ui_imgConvert.h>
#include <qfiledialog.h>

#define	TILESIZEX 16
#define	TILESIZEY 16

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	MainWindow(const QString&, QWidget* parent = 0);
	
	void	createTileSetTileMap();
	void	importImage(QString imgFile);
	void	populateQTableWidget();
	void	generateTileSetToSave();
	void	generateTileMapToSave();
	void	reCreateTileSetTileMap();
	void	save();
	void	getTileInfo(unsigned int id, int& x, int& y, int& fram);
	bool	updateTileMap();
	bool	writeCHFile(QString fileName);
	
	
	
	bool	writeColor(QFile& file, QRgb color, int width, int height);
	
	~MainWindow();
	
	private slots:
	
	void on_ImportButton_clicked();
	void on_Generate_clicked();
	void on_ImgToTile_clicked();
	/*void on_OutputB_clicked();
	void on_resX_textChanged();
	void on_resY_textChanged();
	void on_ratioX_textChanged();
	void on_ratioY_textChanged();
	void on_sColorB_clicked();
	void on_bColorB_clicked();
	void on_sColor_textChanged();
	void on_bColor_textChanged();
	void on_sColor_lostFocus();
	void on_bColor_lostFocus();
	void on_convertB_clicked();
	void on_abordB_clicked();
	void updateProgressBar();
	void on_actionEnglish_triggered();
	void on_actionFrench_triggered();
	void on_actionSpanish_triggered();
	*/
private:
	
	class Tile {
	public:
		Tile(std::vector<unsigned int>& d) {data = d; ocur = 1;};
		Tile(std::vector<unsigned int>& d, unsigned int o, unsigned int i) {data = d; ocur = o; id = i;};
		std::vector<unsigned int> data;
		unsigned int ocur;
		unsigned int id;
		
	};
	
	/*
	bool	isValid(const QString&, const QString&);
	Color	strToColor(const QString&);
	QString colorToStr(const Color);
	*/
	
	Ui_MainWindow ui;
	unsigned int width;
	unsigned int height;
	unsigned int* imgPreCut;
	QImage::Format form;
	std::map<unsigned int, QRgb> origPal;
	unsigned int* tileMap;
	unsigned int* tileMapImage1;
	unsigned int* tileSetImage1;
	std::list<unsigned int> tileMapImage;
	std::list<Tile> tileSetImage;
	std::list<Tile> tileSetImageUpdated;
	std::list<std::vector<unsigned int> > tileset;
	QImage tileSetToSave;
	QImage tileMapToSave;
	QString ext;
	
	/*
	QTimer* timer;
	QFuture<void> future;
	QString resX;
	QString resY;
	QString ratioX;
	QString ratioY;
	QString sColor;
	QString bColor;
	QString path;
	QTranslator* tra;
	 */
};

#endif
