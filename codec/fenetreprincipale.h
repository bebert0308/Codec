#ifndef DEF_MAFENETRE
#define DEF_MAFENETRE

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <fstream>
#include <string>
#include <string.h>
#include <QFile>
using namespace std;

class MaFenetre : public QWidget
{
    Q_OBJECT

    public:
    MaFenetre();

    public slots:
    void chargerFichier();
    void chargerMatrice();
    void cryptage();
    void decryptage();
    char crypt(std::string file_content);
    char decrypt();
    void matrice();




    private:
    QPushButton *m_chargerFichier;
    QPushButton *m_chargerMatrice;
    QPushButton *m_cryptage;
    QPushButton *m_decryptage;
    QLabel *m_text;

};

#endif
