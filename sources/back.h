#ifndef BACK_H
#define BACK_H
#include <QString>

struct GameWorld
{
 ---XXX---
};


class GIFs;

class Background{

public:

    Background();
    ~Background();

    GameWorld* const getGameWorld() const;
    
    //permanent
    void saveGameWorld() const;
    void loadGameWorld();
    void saveContinuePoint();
    
    bool getFileStatus(int index) const;
    QString getPath() const;
    void setPath(QString& path);
    void setFileTail(QString& tail);

    void setGameWorld(GameWorld& data);
    void set4NewGame();
    void setPlayerdata(GIFs& player);
    void reload4SameAgain() const;

    int  getCurrentEnemies() const ; 
    int  getlevel() const;
    
    int  getnEnemiesStart() const;
    int  getautoSaveStart() const ;
    int  getdLevelStart() const;

    //only for new game
    int  getautoSaveNew() const ;
    int  getdLevelNew() const;
    int  getnEnemiesNew() const ;

    void  setautoSaveNew(int autsve);
    void  setdLevelNew(int dlev);
    void  setnEnemiesNew(int nenem);

    //flags
    void setSameAgain(bool sagain);
    bool getSameAgain() const;
    void setUserAction(int usnr);
    int getUserAction() const;

    bool getError() const;

private:

    std::unique_ptr<GameWorld> m_gameWorld;
    GameWorld m_copy4sameagain;

    QString m_path;
    QString m_filetail = "/gameDataCurrent.json";

    int m_userAction;
    
    bool bsameagain;
    bool berror;
    
    void handleDir();
    void dEmO4free();
};
#endif //BACK_H



