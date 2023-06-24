#include "back.h"
#include "gifs.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QJsonArray>
#include <QDir>
#include <QSettings>
#include <QMessageBox>
#include <QObject>
#include <QApplication>

Background::Background()
    : m_userAction(-1), bsameagain(false), berror(false)
{
    m_gameWorld = std::make_unique<GameWorld>();
    if(!m_gameWorld){
        berror = true;
        return;
    }
    handleDir();
}


Background::~Background(){

    QSettings settings(---XXX---);
    settings.setValue(---XXX---, m_path);
}


bool Background::getError() const {
    return berror;
}


void Background::handleDir(){

    dEmO4free();

    QSettings settings(---XXX---);
    QString strDefault = QCoreApplication::applicationDirPath();
    GameWorld gw;
    QDir dir;

    if (!dir.exists(strDefault)) {
        dir.mkpath(strDefault);}

    if (!settings.contains(---XXX---)) {
        QVariant defaultValue = strDefault;
        settings.setValue(---XXX---, defaultValue);
    }

    if (!settings.contains(---XXX---)) {
        QVariant defaultValue = gw.cenemies;
        settings.setValue(---XXX---, defaultValue);
    }

    if (!settings.contains(---XXX---)) {
        QVariant defaultValue = gw.autosave;
        settings.setValue(---XXX---, defaultValue);
    }

    if (!settings.contains(---XXX---)) {
        QVariant defaultValue = gw.dlevel;
        settings.setValue(---XXX---, defaultValue);
    }

    m_path = settings.value(---XXX---).toString();
    if(m_path.isEmpty()){
        m_path = strDefault;
    }

    m_gameWorld->cenemies = settings.value(---XXX---).toInt();
    m_gameWorld->dlevel = settings.value(---XXX---).toInt(); 
    m_gameWorld->autosave = settings.value(---XXX---).toInt(); 
}

void Background::setUserAction(int usnr){
    m_userAction = usnr;
}

int Background::getUserAction() const {
    return m_userAction;
}


QString Background::getPath() const {
    return m_path;
}


void Background::setSameAgain(bool sagain){
    bsameagain = sagain;
}


bool Background::getSameAgain() const {
    return bsameagain;
}


void Background::reload4SameAgain() const {

    m_gameWorld->energy = m_copy4sameagain.energy;
    m_gameWorld->level = m_copy4sameagain.level;
    m_gameWorld->lives = m_copy4sameagain.lives;
    m_gameWorld->score = m_copy4sameagain.score;
    m_gameWorld->currentenemies = m_copy4sameagain.currentenemies; 
    m_gameWorld->cevent = m_copy4sameagain.cevent;
    //don't touch the other options here
}


bool Background::getFileStatus(int index) const {

    bool bfile = false;

    //at least 1 exists
    if(index == -1){

        for(int i=0; i<3; i++){
            QString tail = QString::number(i) + ".json";
            QString path = m_path + "/gameData" + tail;
            if (QFile::exists(path)) {
                bfile = true;
                break;
            }
        }

        //case CurrentData
        QString tail = "/gameDataCurrent.json";
        QString path = m_path + tail;
        if (QFile::exists(path)) {
            bfile = true;
        }
        return bfile;
    }
    //with specific index exists
    else{

        QString tail = QString::number(index) + ".json";
        QString path = m_path + "/gameData" + tail;
        if (QFile::exists(path)) {
            bfile = true;
        }

    }
    return bfile;
}


void Background::set4NewGame(){

    GameWorld gw;
    m_gameWorld->autosave = getautoSaveNew();
    m_gameWorld->dlevel = getdLevelNew();
    m_gameWorld->cenemies = getnEnemiesNew();
    m_gameWorld->currentenemies = getnEnemiesNew();

    m_gameWorld->energy = gw.energy;
    m_gameWorld->level = gw.level;
    m_gameWorld->lives = gw.lives;
    m_gameWorld->score = gw.score;
    m_gameWorld->cevent = gw.cevent;

    m_copy4sameagain = *m_gameWorld.get(); 

}


void Background::setPlayerdata(GIFs& player ){

    player.setEnergy(m_gameWorld->energy);
    player.setLevel(m_gameWorld->level);
    player.setLives(m_gameWorld->lives);
}


void  Background::setautoSaveNew(int autsve){

    QSettings settings(---XXX---);
    QVariant value = autsve;
    settings.setValue(---XXX---, value);
}

void  Background::setdLevelNew(int dlev){

    QSettings settings(---XXX---);
    QVariant value = dlev;
    settings.setValue(---XXX---, value);
}

void Background::setnEnemiesNew(int nenem){

    QSettings settings(---XXX---);
    QVariant value = nenem;
    settings.setValue(---XXX---, value);
}


int Background::getlevel() const {
    return m_gameWorld->level;
}


int Background::getCurrentEnemies() const {
    return m_gameWorld->currentenemies;
}


int Background::getnEnemiesStart() const {
    
    if(m_userAction == 0){
        return getnEnemiesNew();
    }
    else{
        return m_gameWorld->cenemies;
    }
}


int Background::getnEnemiesNew() const {

    QSettings settings(---XXX---);
    return settings.value(---XXX---).toInt();
}


int Background::getautoSaveStart() const {
    
    if(m_userAction == 0){
        return getautoSaveNew();
    }
    else{
        return m_gameWorld->autosave;
    }
}



int Background::getautoSaveNew() const {

    QSettings settings(---XXX---);
    return settings.value(---XXX---).toInt();
}


int Background::getdLevelStart() const {
    
    if(m_userAction == 0){
        
        return getdLevelNew();
    }
    else{
        
        return m_gameWorld->dlevel;
    }
}


int Background::getdLevelNew() const {

    QSettings settings(---XXX---);
    return settings.value(---XXX---).toInt();
}


GameWorld* const Background::getGameWorld() const {
    return m_gameWorld.get();
}


void Background::saveContinuePoint(){

    m_copy4sameagain = *m_gameWorld.get();
    if(m_gameWorld->autosave == 1){
        m_filetail = "/gameDataCurrent.json";
        saveGameWorld();
    }
}


void Background::setGameWorld(GameWorld& data){

    m_gameWorld->energy = data.energy;
    m_gameWorld->level = data.level;
    m_gameWorld->lives = data.lives;
    m_gameWorld->score = data.score;
    m_gameWorld->currentenemies = data.currentenemies;  
}

void Background::saveGameWorld() const {

    QJsonObject gameworld;

    QJsonArray gameState;
    gameState.append(QString::number(m_gameWorld->energy));
    gameState.append(QString::number(m_gameWorld->level));
    gameState.append(QString::number(m_gameWorld->lives));
    gameState.append(QString::number(m_gameWorld->currentenemies));
    gameState.append(QString().asprintf("%0.5f",m_gameWorld->score));
    gameState.append(QString::number(m_gameWorld->cevent));

    QJsonArray optionen;

    if(m_userAction == 0){

        optionen.append(QString::number(getautoSaveNew()));
        optionen.append(QString::number(getnEnemiesNew()));
        optionen.append(QString::number(getdLevelNew()));
    }
    else
    {
        optionen.append(QString::number(m_gameWorld->autosave));
        optionen.append(QString::number(m_gameWorld->cenemies));
        optionen.append(QString::number(m_gameWorld->dlevel));
    }

    gameworld["Optionen"] = optionen;
    gameworld["GameState"] = gameState;

    QString path = m_path + m_filetail;
    QJsonDocument saveDoc(gameworld);
    QFile saveFile(path);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        QString info{};
        info = QObject::tr("Die Datei ") + path + QObject::tr(" konnte nicht gespeichert werden. ");
        QMessageBox::critical(nullptr, QObject::tr("Fehler #ba01"), info);
        return;
    }

    saveFile.write(saveDoc.toJson());
    saveFile.close();
}


void Background::loadGameWorld(){

    QString path = m_path + m_filetail;

    QFile loadFile(path);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        QString info{};
        info = QObject::tr("Die Datei ") + path + QObject::tr(" konnte nicht geladen werden. ");
        QMessageBox::critical(nullptr, QObject::tr("Fehler #ba02"), info);
        return;
    }

    QJsonDocument loadDoc = QJsonDocument::fromJson(loadFile.readAll());
    loadFile.close();

    QJsonObject gameworld = loadDoc.object();

    QJsonArray gameState = gameworld["GameState"].toArray();
    m_gameWorld->energy = gameState[0].toString().toInt();
    m_gameWorld->level = gameState[1].toString().toInt();
    m_gameWorld->lives = gameState[2].toString().toInt();
    m_gameWorld->currentenemies = gameState[3].toString().toInt();
    m_gameWorld->score = gameState[4].toString().toDouble();
    m_gameWorld->cevent = gameState[5].toString().toInt();

    QJsonArray optionen = gameworld["Optionen"].toArray();
    m_gameWorld->autosave = optionen[0].toString().toInt();
    m_gameWorld->cenemies = optionen[1].toString().toInt();
    m_gameWorld->dlevel = optionen[2].toString().toInt();

    m_copy4sameagain = *m_gameWorld.get();
}


void Background::setFileTail(QString& tail){
    m_filetail = tail;
}


void Background::setPath(QString& path){
    m_path = path;
}

